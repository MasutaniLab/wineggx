#define CEGGX_MAIN
#define _USE_MATH_DEFINES
#include <wineggx.h>
#include "ceggx.h"
#include "cselectwindow.h"

using namespace std;

#define EGGX_WINDOW_CLASS_NAME "eggx_window_class_name"
#define WM_USER_EXIT WM_USER+1

#define MAKE_EGGX_MSG(msg,data1,data2) ((((LONGLONG)(((data1&0xffff)<<16)+(data2&0xffff)))<<32)+(LONGLONG)(msg&0xffffffff))
#define EGGX_HIMSG(x) (((UINT)(((LONGLONG)(x))>>48))&0xffff)
#define EGGX_LOMSG(x) (((UINT)(((LONGLONG)(x))>>32))&0xffff)
#define EGGX_MSG(x) ((UINT)((LONGLONG)(x)&0xffffffff))


/**
 * @brief      �R���X�g���N�^
 * @ingroup    CEggX
*/
CEggX::CEggX():
m_Thread(NULL),
m_Event(NULL),
m_hInst(NULL),
m_NumWind(0),
m_MaxWindow(10),
m_createWindowNum(-1),
m_run(false),
m_SleepTime(INFINITE),
m_accesstable(NULL),
m_Init(false),
m_Close(false),
m_Nonblock(false),
m_eventKey(-1)
{
  m_Event = ::CreateEvent(NULL,false,false,NULL);
  m_Ack   = ::CreateEvent(NULL,false,false,NULL);
  m_Com   = ::CreateEvent(NULL,false,false,NULL);

  m_eventHandle = ::CreateEvent(NULL,false,false,NULL);

  m_Window.resize(m_MaxWindow);
  for(int i=0;i<m_MaxWindow;i++)
  {
      EggXWindow &wnd = m_Window.at(i);
      memset(&wnd,0,sizeof(EggXWindow));
  }

#include "rgbtable.h"

  keytable[VK_HOME] = 0x01;
  keytable[VK_PRIOR] = 0x02; //PageUp
  keytable[VK_PAUSE] = 0x03;
  keytable[VK_END] = 0x05;
  keytable[VK_NEXT] = 0x06; //PageDown
  keytable[VK_BACK] = 0x08; //BackSpace
  keytable[VK_TAB] = 0x09;
  keytable[VK_RETURN] = 0x0d; //Enter
  keytable[VK_RIGHT] = 0x1c;//��
  keytable[VK_LEFT] = 0x1d;//��
  keytable[VK_UP] = 0x1e;//��
  keytable[VK_DOWN] = 0x1f;//��
  keytable[VK_DELETE] = 0x7f;
}


/**
 * @brief      �f�X�g���N�^
 * @ingroup    CEggX
*/
CEggX::~CEggX()
{
  gcloseall();
  Release();
  m_Init = false;

  if(m_Event)
  {
      ::CloseHandle(m_Event);
      m_Event = NULL;
  }
  if(m_Ack)
  {
      ::CloseHandle(m_Ack);
      m_Ack = NULL;
  }
  if(m_Com)
  {
      ::CloseHandle(m_Com);
      m_Com = NULL;
  }

  if(!m_Font.empty())
  {
      map<int,MyFont>::iterator itr = m_Font.begin();
      map<int,MyFont>::iterator end = m_Font.end();
      for(;itr!=end;itr++)
      {
          MyFont& myfont = itr->second;
          if(myfont.hFont)
          {
              ::DeleteObject(myfont.hFont);
              myfont.hFont = NULL;
          }
      }
      m_Font.clear();
  }
}


/**
 * @brief      Windows��eggx�Ɠ������Ƃ����邽�߂̏���
 * @ingroup    CEggX
 * @param[in]  hInst �C���X�^���X�n���h��
 * @param[in]  AccessTableId �A�N�Z�X�e�[�u����ID�ԍ�
 * @retval     1  ���łɏ���������
 * @retval     -1 �G���[
 * @retval     0  ����
*/
int CEggX::Initialize(HINSTANCE hInst,LONGLONG AccessTableId)
{
  if(m_run)return 1;

  //�E�B���h�E�N���X�o�^
  WNDCLASSEX wcex;
  wcex.cbSize        = sizeof(WNDCLASSEX); 
  wcex.style         = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc   = (WNDPROC)WindowProc;
  wcex.cbClsExtra    = 0;
  wcex.cbWndExtra    = 0;
  wcex.hInstance         = hInst;
  wcex.hIcon         = NULL;
  wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wcex.lpszMenuName  = NULL;
  wcex.lpszClassName = EGGX_WINDOW_CLASS_NAME;
  wcex.hIconSm       = NULL;
  ::RegisterClassEx(&wcex);

  m_hInst       = hInst;
  m_accesstable = AccessTableId;

  //�X���b�h���쐬����
  return CreateThread();
}


/**
 * @brief      �X���b�h���쐬����
 * @ingroup    CEggX
 * @retval     1  ���łɍ쐬�ς�
 * @retval     -1 �G���[
 * @retval     0  ����
*/
int  CEggX::CreateThread()
{
  if( m_run || m_Thread ) return 1;

  // �X���b�h�쐬
  m_Thread = ::CreateThread(NULL,0,ThreadEntry,(DWORD*)this,0,NULL);
  if(!m_Thread)
  {
      return -1;
  }
  return 0;
}


/**
 * @brief      �S�ẴE�B���h�E��j�󂷂�
 * @ingroup    CEggX
*/
void CEggX::Release()
{
  if( !m_run || !m_Thread )return;

  m_run = false;
  ::SetEvent(m_Event);
  ::WaitForSingleObject(m_Thread,INFINITE);

  m_Thread = NULL;
}


/**
 * @brief      �S�ẴE�B���h�E��j�󂷂�
 * @ingroup    CEggX
*/
void CEggX::DestoroyAll()
{
  for(int i=0;i<m_MaxWindow;i++)
  {
      Destoroy(i);
  }
}


/**
 * @brief      �w��̃E�B���h�E��j�󂷂�
 * @ingroup    CEggX
 * @param[in]  wn �E�B���h�E�ԍ�
*/
void CEggX::Destoroy(int wn)
{
  EggXWindow &wnd = m_Window.at(wn);
  if(wnd.hBrush)
  {
      ::DeleteObject(wnd.hBrush);
      wnd.hBrush = NULL;
  }
  if(wnd.hBrushB)
  {
      ::DeleteObject(wnd.hBrushB);
      wnd.hBrushB = NULL;
  }
  if(wnd.hPen)
  {
      ::DeleteObject(wnd.hPen);
      wnd.hPen = NULL;
  }
   if(wnd.hPenF)
  {
      ::DeleteObject(wnd.hPenF);
      wnd.hPenF = NULL;
  }
 for(int i=0;i<(sizeof(wnd.hBitmap)/sizeof(HBITMAP));i++)
  {
      if(wnd.hBitmap[i])
      {
          ::DeleteObject(wnd.hBitmap[i]);
          wnd.hBitmap[i] = NULL;
      }
  }

  if(wnd.hWnd)
  {
      m_NumWind--;
      if(!m_NumWind)m_SleepTime=INFINITE;
  }

  wnd.hWnd = NULL;
  wnd.hFont= NULL;
  wnd.writeLayer = 0;
  wnd.showLayer  = 0;
  wnd.xs = 0;
  wnd.ys = 0;
  wnd.xe = 0;
  wnd.ye = 0;
  wnd.r  = 255;
  wnd.b  = 255;
  wnd.g  = 255;
  wnd.rB = 0;
  wnd.gB = 0;
  wnd.bB = 0;
  wnd.width = 0;
  wnd.style = 0;
  wnd.cx = 0;
  wnd.cy = 0;
  wnd.lx = 0;
  wnd.ly = 0;
  wnd.fontsize    = 0;
  wnd.fontName[0] = NULL;
}


/**
 * @brief      �E�B���h�E���쐬����
 * @ingroup    CEggX
 * @param[in]  xsize �E�B���h�E�̉���
 * @param[in]  ysize �E�B���h�E�̏c��
 * @retval     -1 �G���[
*/
int CEggX::CreateEggXWindow(int xsize,int ysize)
{
  if(m_NumWind>=m_MaxWindow||!m_Init)return -1;
  if(xsize<=0||ysize<=0)return -1;

  int num = -1;
  for(int i=0;i<m_MaxWindow;i++)
  {
      if(!m_Window.at(i).hWnd)
      {
          num = i;
          break;
      }
  }
  if(num==-1)return -1;

  //�E�B���h�E�X�^�C��
  DWORD Style   = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
  DWORD StyleEx = WS_EX_WINDOWEDGE;

  //�E�B���h�E��
  char windowname[64];
  sprintf_s(windowname, sizeof(windowname), "%s%d", "eggx window", num);


  CREATESTRUCT createstr;
  createstr.lpCreateParams = (void*)this;
  createstr.hInstance      = m_hInst;
  createstr.hMenu          = NULL;
  createstr.hwndParent     = NULL;
  createstr.cx             = xsize;
  createstr.cy             = ysize;
  createstr.x              = CW_USEDEFAULT;
  createstr.y              = 0;
  createstr.style          = Style;
  createstr.lpszName       = windowname;
  createstr.lpszClass      = EGGX_WINDOW_CLASS_NAME;
  createstr.dwExStyle      = StyleEx;

  int a = sizeof(CREATESTRUCT);
  void *lp = &createstr;
  void *lp2 = &createstr.lpCreateParams;
  HWND hWnd;
  hWnd = CreateWindowEx(StyleEx,EGGX_WINDOW_CLASS_NAME, windowname, Style,
                        CW_USEDEFAULT, 0, xsize, ysize, NULL, NULL, m_hInst, &createstr);
  if (!hWnd)
  {
      return FALSE;
  }
  RECT rect;
  ::GetClientRect(hWnd,&rect);
  int width  = xsize + (xsize - rect.right);
  int height = ysize + (ysize - rect.bottom);
  ::SetWindowPos(hWnd,NULL,0,0,width,height,SWP_NOMOVE|SWP_NOZORDER);

  HDC hDC      = ::GetDC(hWnd);
  EggXWindow &wnd = m_Window.at(num);
  wnd.hWnd     = hWnd;
  sprintf_s(wnd.fontName, sizeof(wnd.fontName), "�l�r �S�V�b�N");
  wnd.cx       = xsize;
  wnd.cy       = ysize;
  wnd.xs       = 0;
  wnd.ys       = 0;
  wnd.xe       = xsize-1;
  wnd.ye       = ysize-1;
  wnd.index    = 0;
  wnd.r        = 255;
  wnd.g        = 255;
  wnd.b        = 255;
  wnd.rB       = 0;
  wnd.gB       = 0;
  wnd.bB       = 0;
  wnd.width    = 1;
  wnd.style    = PS_SOLID;
  wnd.writeLayer = 0;
  wnd.showLayer  = 0;

  for(int i=0;i<(sizeof(wnd.hBitmap)/sizeof(HBITMAP));i++)
      wnd.hBitmap[i] = ::CreateCompatibleBitmap(hDC,xsize,ysize);

  ::ReleaseDC(hWnd,hDC);

  if(wnd.hBrush)::DeleteObject(wnd.hBrush);
  LOGBRUSH brush;
  brush.lbColor = RGB(wnd.r,wnd.g,wnd.b);
  brush.lbStyle = BS_SOLID;
  brush.lbHatch = NULL;
  wnd.hBrush = ::CreateBrushIndirect(&brush);

  if(wnd.hBrushB)::DeleteObject(wnd.hBrushB);
  brush.lbColor = RGB(wnd.rB,wnd.gB,wnd.bB);
  brush.lbStyle = BS_SOLID;
  brush.lbHatch = NULL;
  wnd.hBrushB = ::CreateBrushIndirect(&brush);

  if(wnd.hPen)::DeleteObject(wnd.hPen);
  LOGPEN pen;
  pen.lopnColor   = RGB(wnd.r,wnd.g,wnd.b);
  pen.lopnStyle   = wnd.style;
  pen.lopnWidth.x = wnd.width;
  pen.lopnWidth.y = wnd.width;
  wnd.hPen = ::CreatePenIndirect(&pen);

  if(wnd.hPenF)::DeleteObject(wnd.hPenF);
  pen.lopnColor   = RGB(wnd.r,wnd.g,wnd.b);
  pen.lopnStyle   = PS_INSIDEFRAME;
  pen.lopnWidth.x = 0;
  pen.lopnWidth.y = 0;
  wnd.hPenF = ::CreatePenIndirect(&pen);

  //�E�B���h�E���őO�ʂɕ\������
  BringWindowToTop(FindWindow(EGGX_WINDOW_CLASS_NAME,windowname));

  //�E�B���h�E����ɍőO�ʂɕ\������
  SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);

  m_NumWind++;

  m_SleepTime = 20;

  return num;
}


/**
 * @brief      �C�ӂ̃T�C�Y�̃O���t�B�b�N�X��ʂ��J��
 * @ingroup    CEggX
 * @param[in]  xsize �E�B���h�E�̉���
 * @param[in]  ysize �E�B���h�E�̏c��
 * @return     �Ԃ�l�ɂ�EGGX�Ŏg�p����C�E�B���h�E�ԍ����Ԃ��Ă��܂��D
 * @retval     -1 �G���[
*/
int  CEggX::gopen(int xsize,int ysize)
{
  if(!m_Init) {
	int re = Initialize(GetModuleHandle(NULL), NULL);
	if ( re ) {
		return -1;
	} else {
		m_Init = true;
	}
  }
	  
  m_MSG.push_back(MAKE_EGGX_MSG(WINDOW_CREATE,xsize,ysize));
  ::SetEvent(m_Event);

  ::WaitForSingleObject(m_Com,INFINITE);
  int crWindow = m_createWindowNum;
  m_createWindowNum = -1;
  ::SetEvent(m_Ack);
  return crWindow;
}


/**
 * @brief      �O���t�B�b�N�X�E�B���h�E�����
 * @ingroup    CEggX
 * @param[in]  wn �E�B���h�E�ԍ�
*/
void CEggX::gclose( unsigned wn )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  ::PostMessage(wnd.hWnd,WM_USER_EXIT,0,0);
}


/**
 * @brief      ���ׂẴO���t�B�b�N�X�p�E�B���h�D�����
 * @ingroup    CEggX
*/
void CEggX::gcloseall( void )
{
  for(int i=0;i<m_MaxWindow;i++)
  {
      EggXWindow &wnd = m_Window.at(i);
      if(!wnd.hWnd)continue;
      ::PostMessage(wnd.hWnd,WM_USER_EXIT,0,0);
  }
}

/**
* @brief      �O���t�B�b�N�X�`��̈�̃T�C�Y�ύX���s�Ȃ�
* @ingroup    CEggX
* @param[in]  wn �E�B���h�E�ԍ�
* @param[in]  xsize �E�B���h�E�̉���
* @param[in]  ysize �E�B���h�E�̏c��
*/
void CEggX::gresize(unsigned wn, int xsize, int ysize)
{
  if (wn >= (unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if (!wnd.hWnd)return;

  wnd.cx = xsize;
  wnd.cy = ysize;

  RECT       rc;
  ::SetRect(&rc, 0, 0, xsize, ysize);
  ::AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, 0);
  ::SetWindowPos(wnd.hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

  HDC hDC = ::GetDC(wnd.hWnd);
  for (int i = 0; i < (sizeof(wnd.hBitmap) / sizeof(HBITMAP)); i++) {
    wnd.hBitmap[i] = ::CreateCompatibleBitmap(hDC, xsize, ysize);
  }
  ::ReleaseDC(wnd.hWnd, hDC);

}

/**
 * @brief      �E�B���h�D�̃^�C�g����ύX����
 * @ingroup    CEggX
 * @param[in]  wn �^�C�g����ύX����E�B���h�E�̔ԍ�
 * @param[in]  ... �ݒ肷��E�B���h�E�̕�����
 * @return     �ݒ肵���E�B���h�D�^�C�g���̕�����̒���
*/
int  CEggX::winname(unsigned wn, const char *argsformat, va_list argptr)
{
  if(wn>=(unsigned)m_MaxWindow)return 0;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return 0;

  char str[256];
  int len = _vsnprintf_s(str, sizeof(str), _TRUNCATE, argsformat, argptr);
  if (len == -1)str[sizeof(str) - 1] = '\0';
  else str[len] = '\0';

  SetWindowText(wnd.hWnd, str);
  return (int)strlen(str);
}


/**
 * @brief      ���W�n�̕ύX
 * @ingroup    CEggX
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  xs ������X���W
 * @param[in]  ys ������Y���W
 * @param[in]  xe �E���X���W
 * @param[in]  ye �E���Y���W
*/
void CEggX::window( unsigned wn, double xs, double ys, double xe, double ye )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;
  wnd.xs = xs;
  wnd.ys = ys;
  wnd.xe = xe;
  wnd.ye = ye;
}


/**
 * @brief      ���C���̐ݒ������
 * @ingroup    CEggX
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  lys �\�����C���[( 0 or 1 )
 * @param[in]  lyw �������ݐ惌�C���[( 0 or 1 )
*/
void CEggX::layer( unsigned wn, int lys, int lyw )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;
  wnd.showLayer  = lys;
  wnd.writeLayer = lyw;
}


/**
 * @brief      ���C���̃R�s�[������
 * @ingroup    CEggX
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  lysrc  �R�s�[�����C���[�ԍ�
 * @param[in]  lydest �R�s�[�惌�C���[�ԍ�
*/
void CEggX::copylayer( unsigned wn, int lysrc, int lydest )
{
  if(m_Close)exit(99);

  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;
  if(wnd.showLayer==wnd.writeLayer)return;
  if(wnd.showLayer!=lydest||wnd.writeLayer!=lysrc)return;
  int index = wnd.index + 1;
  if(index>=(sizeof(wnd.hBitmap)/sizeof(HBITMAP)))index = 0;
  wnd.index = index;

  ::InvalidateRect(wnd.hWnd,NULL,false);
}

/**
 * @brief      �`��F�̕ύX
 * @ingroup    CEggX
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  str �F��\��������
*/
void CEggX::newcolor( unsigned wn, const char *argsformat, va_list argptr)
{
  if (wn >= (unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if (!wnd.hWnd)return;

  char str[256];
  int len = _vsnprintf_s(str, sizeof(str), _TRUNCATE, argsformat, argptr);
  if (len == -1)str[sizeof(str) - 1] = '\0';
  else str[len] = '\0';

  rgb a;
  if (str[0] == '#') {
    if (sscanf_s(str, "#%2x%2x%2x", &a.r, &a.g, &a.b) != 3) {
      cerr << str << " cannot decode" << endl;
      return;
    }
  } else {
    map<string, rgb>::iterator itr;
    itr = rgbtable.find(str);
    if (itr == rgbtable.end()) {
      cerr << str << " not found" << endl;
      return;
    } else {
      a = itr->second;
    }
  }
  newrgbcolor(wn, a.r, a.g, a.b);
}

/**
 * @brief      �`��F�̕ύX
 * @ingroup    CEggX
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  r  �ԐF ( 0 to 255 )
 * @param[in]  g  �ΐF ( 0 to 255 )
 * @param[in]  b  �F ( 0 to 255 )
*/
void CEggX::newrgbcolor( unsigned wn, int r, int g, int b )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  if(wnd.hBrush)::DeleteObject(wnd.hBrush);
  LOGBRUSH brush;
  brush.lbColor = RGB(r,g,b);
  brush.lbStyle = BS_SOLID;
  brush.lbHatch = NULL;
  wnd.hBrush = ::CreateBrushIndirect(&brush);

  if(wnd.hPen)::DeleteObject(wnd.hPen);
  LOGPEN pen;
  pen.lopnColor   = RGB(r,g,b);
  pen.lopnStyle   = wnd.style;
  pen.lopnWidth.x = wnd.width;
  pen.lopnWidth.y = wnd.width;
  wnd.hPen = ::CreatePenIndirect(&pen);

  if(wnd.hPenF)::DeleteObject(wnd.hPenF);
  pen.lopnColor   = RGB(r,g,b);
  pen.lopnStyle   = PS_SOLID;
  pen.lopnWidth.x = 0;
  pen.lopnWidth.y = 0;
  wnd.hPenF = ::CreatePenIndirect(&pen);

  wnd.r = r;
  wnd.g = g;
  wnd.b = b;
}

/**
 * @brief      �`��F�̕ύX
 * @ingroup    CEggX
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  h  �F�� ( 0 to 359 )
 * @param[in]  s  �ʓx ( 0 to 255 )
 * @param[in]  v  ���x ( 0 to 255 )
*/
void CEggX::newhsvcolor( unsigned wn, int h, int s, int v )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  int r, g, b;
  if ( s == 0 ) {
	  r = g = b = v;
  } else {
	  int hi = (h/60)%6;
	  double f = h/60.0-hi;
	  double ss = s/255.0;
	  int p = int(v*(1-ss)+0.5);
	  int q = int(v*(1-f*ss)+0.5);
	  int t = int(v*(1-(1-f)*ss)+0.5);
	  switch ( hi ) {
	  case 0: r = v; g = t; b = p; break;
	  case 1: r = q; g = v; b = p; break;
	  case 2: r = p; g = v; b = t; break;
	  case 3: r = p; g = q; b = v; break;
	  case 4: r = t; g = p; b = v; break;
	  case 5: r = v; g = p; b = q; break;
	  }		  
  }
  newrgbcolor(wn, r, g, b);
}

/**
 * @brief      �E�B���h�D�̔w�i�F�̕ύX
 * @ingroup    CEggX
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  str �F��\��������
*/
void CEggX::gsetbgcolor(unsigned wn, const char *argsformat, va_list argptr)
{
  if (wn >= (unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if (!wnd.hWnd)return;

  char str[256];
  int len = _vsnprintf_s(str, sizeof(str), _TRUNCATE, argsformat, argptr);
  if (len == -1)str[sizeof(str) - 1] = '\0';
  else str[len] = '\0';

  rgb a;
  if (str[0] == '#') {
    if (sscanf_s(str, "#%2x%2x%2x", &a.r, &a.g, &a.b) != 3) {
      cerr << str << " cannot decode" << endl;
      return;
    }
  } else {
    map<string, rgb>::iterator itr;
    itr = rgbtable.find(str);
    if (itr == rgbtable.end()) {
      cerr << str << " not found" << endl;
      return;
    } else {
      a = itr->second;
    }
  }
  gsetbgcolorrgb(wn, a.r, a.g, a.b);
}

/**
 * @brief      �E�B���h�D�̔w�i�F�̕ύX
 * @ingroup    CEggX
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  r  �ԐF ( 0 to 255 )
 * @param[in]  g  �ΐF ( 0 to 255 )
 * @param[in]  b  �F ( 0 to 255 )
*/
void CEggX::gsetbgcolorrgb( unsigned wn, int r, int g, int b )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  if(wnd.hBrushB)::DeleteObject(wnd.hBrushB);
  LOGBRUSH brushB;
  brushB.lbColor = RGB(r,g,b);
  brushB.lbStyle = BS_SOLID;
  brushB.lbHatch = NULL;
  wnd.hBrushB = ::CreateBrushIndirect(&brushB);

  wnd.rB = r;
  wnd.gB = g;
  wnd.bB = b;
}
/**
 * @brief      �`�惌�C���̑S����
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
*/
void CEggX::gclr( unsigned wn )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);

  if(!wnd.hWnd)return;

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
      ::SelectObject(hWDC,wnd.hBrushB);

      ::PatBlt(hWDC,0,0,wnd.cx,wnd.cy,PATCOPY);
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
      ::SelectObject(hDC,wnd.hBrushB);

      ::PatBlt(hDC,0,0,wnd.cx,wnd.cy,PATCOPY);

      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
}

/**
 * @brief      �����̕ύX
 * @ingroup    CEggX
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  width ���� 
*/
void CEggX::newlinewidth( unsigned wn, int width )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  if(wnd.hPen)::DeleteObject(wnd.hPen);
  LOGPEN pen;
  pen.lopnColor   = RGB(wnd.r,wnd.g,wnd.b);
  pen.lopnStyle   = wnd.style;
  pen.lopnWidth.x = width;
  pen.lopnWidth.y = width;
  wnd.hPen = ::CreatePenIndirect(&pen);

  wnd.width = width;
}

/**
 * @brief      ���̃X�^�C���̕ύX
 * @ingroup    CEggX
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  style ���̃X�^�C���iLineSolid�F�����CLineOnOffDash�F�_���j 
*/
void CEggX::newlinestyle( unsigned wn, int style )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  if ( style == LineSolid ) {
	  wnd.style = PS_SOLID;
  } else if ( style == LineOnOffDash ) {
	  wnd.style = PS_DOT;
  } else {
	  cerr << "newlinestyle(): invalid line style" << endl;
	  return;
  }

  if(wnd.hPen)::DeleteObject(wnd.hPen);
  LOGPEN pen;
  pen.lopnColor   = RGB(wnd.r,wnd.g,wnd.b);
  pen.lopnStyle   = wnd.style;
  pen.lopnWidth.x = wnd.width;
  pen.lopnWidth.y = wnd.width;
  wnd.hPen = ::CreatePenIndirect(&pen);
}

/**
 * @brief      �_�̕`��
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y �`�����W
*/
void CEggX::pset( unsigned wn,double x,double y )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  int xx = convertX(wnd, x);
  int yy = convertY(wnd, y);

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
	  ::SetPixelV(hWDC,xx,yy,RGB(wnd.r,wnd.g,wnd.b));
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
	  ::SetPixelV(hDC,xx,yy,RGB(wnd.r,wnd.g,wnd.b));
      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
}


/**
 * @brief      �����̕`��
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x0,y0 �[�_�̍��W
 * @param[in]  x1,y1 �[�_�̍��W
*/
void CEggX::drawline( unsigned wn, double x0, double y0, double x1, double y1 )
{
	moveto(wn, x0, y0);
	lineto(wn, x1, y1);
}

/**
 * @brief      ������`���߂̏����ʒu�̐ݒ�
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y �`�����W
*/
void CEggX::moveto( unsigned wn, double x, double y )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  wnd.ly = convertY(wnd, y);
  wnd.lx = convertX(wnd, x);
}

/**
 * @brief      �A���I�ɒ�����`��
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y ���W
 * @section Notes
*/
void CEggX::lineto( unsigned wn, double x, double y ){
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  int yy = convertY(wnd, y);
  int xx = convertX(wnd, x);

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
      ::SelectObject(hWDC,wnd.hPen);
	  ::SetBkMode(hWDC, TRANSPARENT);

      ::MoveToEx(hWDC,wnd.lx,wnd.ly,NULL);
      ::LineTo(hWDC,xx,yy);
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
      ::SelectObject(hDC,wnd.hPen);
	  ::SetBkMode(hDC, TRANSPARENT);

      ::MoveToEx(hDC,wnd.lx,wnd.ly,NULL);
      ::LineTo(hDC,xx,yy);

      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
}

/**
 * @brief      �����̕`��
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y �`�����W
 * @param[in]  mode �`�惂�[�h (@ref PENDOWN @ref PENUP @ref PSET)
*/
void CEggX::line( unsigned wn,double x,double y,int mode )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  int yy = convertY(wnd, y);
  int xx = convertX(wnd, x);

  if(mode==PENUP||mode==PSET)
  {
      wnd.lx = xx;
      wnd.ly = yy;
      if(mode == PENUP)return;
  }

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
      ::SelectObject(hWDC,wnd.hPen);
	  ::SetBkMode(hWDC, TRANSPARENT);

      ::MoveToEx(hWDC,wnd.lx,wnd.ly,NULL);
      if(mode==PSET)::SetPixelV(hWDC,xx,yy,RGB(wnd.r,wnd.g,wnd.b));
      else          ::LineTo(hWDC,xx,yy);
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
      ::SelectObject(hDC,wnd.hPen);
	  ::SetBkMode(hDC, TRANSPARENT);

      ::MoveToEx(hDC,wnd.lx,wnd.ly,NULL);
      if(mode==PSET)::SetPixelV(hDC,xx,yy,RGB(wnd.r,wnd.g,wnd.b));
      else          ::LineTo(hDC,xx,yy);

      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
}


/**
 * @brief      �~�̕`��
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  xcen,ycen �~�̒��S���W
 * @param[in]  xrad X�������̔��a
 * @param[in]  yrad Y�������̔��a
*/
void CEggX::circle( unsigned wn, double xcen, double ycen, double xrad, double yrad )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  int xx = convertX(wnd, xcen);
  int yy = convertY(wnd, ycen);
  int xxrad = scaleX(wnd, xrad);
  int yyrad = scaleY(wnd, yrad);

  int x1 = xx + xxrad;
  int x2 = xx - xxrad;
  int y1 = yy + yyrad;
  int y2 = yy - yyrad;

  HBRUSH hBrushNull = (HBRUSH)::GetStockObject(NULL_BRUSH);

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
      ::SelectObject(hWDC,wnd.hPen);
	  ::SelectObject(hWDC,hBrushNull);
	  ::SetBkMode(hWDC, TRANSPARENT);

	  ::Ellipse(hWDC,x1,y1,x2,y2);
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
      ::SelectObject(hDC,wnd.hPen);
	  ::SelectObject(hDC,hBrushNull);
	  ::SetBkMode(hDC, TRANSPARENT);

      ::Ellipse(hDC,x1,y1,x2,y2);

      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
}

/**
 * @brief      ���S���W�C���a��^���ĉ~��h��ׂ�
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  xcen,ycen �~�̒��S���W
 * @param[in]  xrad X�������̔��a
 * @param[in]  yrad Y�������̔��a
*/
void CEggX::fillcirc( unsigned wn, double xcen, double ycen, double xrad, double yrad )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  int xx = convertX(wnd, xcen);
  int yy = convertY(wnd, ycen);
  int xxrad = scaleX(wnd, xrad);
  int yyrad = scaleY(wnd, yrad);

  int x1 = xx + xxrad;
  int x2 = xx - xxrad;
  int y1 = yy + yyrad;
  int y2 = yy - yyrad;

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
      ::SelectObject(hWDC,wnd.hPenF);
	  ::SelectObject(hWDC,wnd.hBrush);

      ::Ellipse(hWDC,x1,y1,x2,y2);
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
      ::SelectObject(hDC,wnd.hPenF);
      ::SelectObject(hDC,wnd.hBrush);

      ::Ellipse(hDC,x1,y1,x2,y2);

      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
}

/**
 * @brief      �~�ʂ̕`��
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  xcen,ycen �~�̒��S���W
 * @param[in]  xrad X�������̔��a
 * @param[in]  yrad Y�������̔��a
 * @param[in]  sang �`��J�n�p�x [degree]
 * @param[in]  eang �`��I���p�x [degree]
 * @param[in]  idir �`����� ( 1 ����� or -1 �E���)
*/
void CEggX::drawarc( unsigned wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  int xx = convertX(wnd, xcen);
  int yy = convertY(wnd, ycen);
  int xxrad = scaleX(wnd, xrad);
  int yyrad = scaleY(wnd, yrad);

  int x1 = xx + xxrad;
  int x2 = xx - xxrad;
  int y1 = yy + yyrad;
  int y2 = yy - yyrad;

  int cx1,cy1,cx2,cy2;
  if(fmod(sang,360)==fmod(eang,360))
  {
      cx1=cy1=cx2=cy2=0;
  }
  else
  {
      cx1 = xx + (int)(cos(sang*M_PI/180.0)*1000);
      cy1 = yy - (int)(sin(sang*M_PI/180.0)*1000);
      cx2 = xx + (int)(cos(eang*M_PI/180.0)*1000);
      cy2 = yy - (int)(sin(eang*M_PI/180.0)*1000);
  }

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
      ::SelectObject(hWDC,wnd.hPen);
 	  ::SetBkMode(hWDC, TRANSPARENT);
      if(idir==-1)
      {
          ::SetArcDirection(hWDC,AD_CLOCKWISE);
          ::Arc(hWDC,x1,y1,x2,y2,cx1,cy1,cx2,cy2);
      }
      else
      {
          ::SetArcDirection(hWDC,AD_COUNTERCLOCKWISE);
          ::Arc(hWDC,x1,y1,x2,y2,cx1,cy1,cx2,cy2);
      }
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
      ::SelectObject(hDC,wnd.hPen);
	  ::SetBkMode(hDC, TRANSPARENT);
      if(idir==-1)
      {
          ::SetArcDirection(hDC,AD_CLOCKWISE);
          ::Arc(hDC,x1,y1,x2,y2,cx1,cy1,cx2,cy2);
      }
      else
      {
          ::SetArcDirection(hDC,AD_COUNTERCLOCKWISE);
          ::Arc(hDC,x1,y1,x2,y2,cx1,cy1,cx2,cy2);
      }
      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
}


/**
 * @brief      �~�ʂ̕`��(�h��Ԃ�)
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  xcen,ycen �~�̒��S���W
 * @param[in]  xrad X�������̔��a
 * @param[in]  yrad Y�������̔��a
 * @param[in]  sang �`��J�n�p�x [degree]
 * @param[in]  eang �`��I���p�x [degree]
 * @param[in]  idir �`����� ( 1 ����� or -1 �E���)
*/
void CEggX::fillarc( unsigned wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  int xx = convertX(wnd, xcen);
  int yy = convertY(wnd, ycen);
  int xxrad = scaleX(wnd, xrad);
  int yyrad = scaleY(wnd, yrad);

  int x1 = xx + xxrad;
  int x2 = xx - xxrad;
  int y1 = yy + yyrad;
  int y2 = yy - yyrad;

  int cx1,cy1,cx2,cy2;
  if(fmod(sang,360)==fmod(eang,360))
  {
      cx1=cy1=cx2=cy2=0;
  }
  else
  {
      cx1 = xx + (int)(cos(sang*M_PI/180.0)*1000);
      cy1 = yy - (int)(sin(sang*M_PI/180.0)*1000);
      cx2 = xx + (int)(cos(eang*M_PI/180.0)*1000);
      cy2 = yy - (int)(sin(eang*M_PI/180.0)*1000);
  }

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
      ::SelectObject(hWDC,wnd.hPenF);
      ::SelectObject(hWDC,wnd.hBrush);
      if(idir==-1)
      {
          ::SetArcDirection(hWDC,AD_CLOCKWISE);
          ::Pie(hWDC,x1,y1,x2,y2,cx1,cy1,cx2,cy2);
      }
      else
      {
          ::SetArcDirection(hWDC,AD_COUNTERCLOCKWISE);
          ::Pie(hWDC,x1,y1,x2,y2,cx1,cy1,cx2,cy2);
      }
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
      ::SelectObject(hDC,wnd.hPenF);
      ::SelectObject(hDC,wnd.hBrush);
      if(idir==-1)
      {
          ::SetArcDirection(hDC,AD_CLOCKWISE);
          ::Pie(hDC,x1,y1,x2,y2,cx1,cy1,cx2,cy2);
      }
      else
      {
          ::SetArcDirection(hDC,AD_COUNTERCLOCKWISE);
          ::Pie(hDC,x1,y1,x2,y2,cx1,cy1,cx2,cy2);
      }
      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
}


/**
 * @brief      �����`�̕`��
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y  �`����W�i�����`�̍����j
 * @param[in]  w    �����`�̉���
 * @param[in]  h    �����`�̏c��
*/
void CEggX::drawrect( unsigned wn, double x, double y, double w, double h )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  int xx = convertX(wnd, x);
  int yy = convertY(wnd, y);
  int ww = scaleX(wnd, w);
  int hh = scaleY(wnd, h);

  HBRUSH hBrushNull = (HBRUSH)::GetStockObject(NULL_BRUSH);

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
	  ::SelectObject(hWDC,hBrushNull);
      ::SelectObject(hWDC,wnd.hPen);
	  ::SetBkMode(hWDC, TRANSPARENT);

      ::Rectangle(hWDC,xx,yy,xx+ww,yy-hh);
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
	  ::SelectObject(hDC,hBrushNull);
      ::SelectObject(hDC,wnd.hPen);
	  ::SetBkMode(hDC, TRANSPARENT);

      ::Rectangle(hDC,xx,yy,xx+ww,yy-hh);

      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
}


/**
 * @brief      �����`�̕`��i�h��Ԃ��j
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y  �`����W�i�����`�̍����j
 * @param[in]  w    �����`�̉���
 * @param[in]  h    �����`�̏c��
*/
void CEggX::fillrect( unsigned wn, double x, double y, double w, double h )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  int xx = convertX(wnd, x);
  int yy = convertY(wnd, y);
  int ww = scaleX(wnd, w);
  int hh = scaleY(wnd, h);

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
      ::SelectObject(hWDC,wnd.hPenF);
      ::SelectObject(hWDC,wnd.hBrush);

      ::Rectangle(hWDC,xx,yy,xx+ww,yy-hh);
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
      ::SelectObject(hDC,wnd.hPenF);
      ::SelectObject(hDC,wnd.hBrush);

	  ::Rectangle(hDC,xx,yy,xx+ww,yy-hh);

      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
}

/**
 * @brief      ������̕`��
 * @ingroup    CEggX
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y   �`�����W�i�������`�悷��ʒu�̍����̍��W�j
 * @param[in]  size  �`�悷�镶���̑傫��
 * @param[in]  theta ������̉�]�p�x [degree] ( * ���݂͖��� )
 * @param[in]  argsformat, ...   ������
 * @return     ���ۂɕ`�悵��������̒���
*/
int  CEggX::drawstr(unsigned wn, double x, double y, int size, double theta, const char *argsformat, va_list argptr)
{
  if(wn>=(unsigned)m_MaxWindow)return 0;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return 0;

  char str[256];
  int len0 = _vsnprintf_s(str, sizeof(str), _TRUNCATE, argsformat, argptr);
  if (len0 == -1)str[sizeof(str) - 1] = '\0';
  else str[len0] = '\0';

  if (size <= 0)size = 14;

  //�t�H���g�쐬
  if(!wnd.fontsize||size!=wnd.fontsize||!wnd.hFont)
  {
      bool find = false;
      wnd.fontsize = size;

      map<int,MyFont>::iterator itr;
      itr = m_Font.find(size);
      if(itr!=m_Font.end())
      {
          MyFont &myfont = itr->second;
          if(!strcmp(myfont.fontName,wnd.fontName))
          {
              wnd.hFont = &myfont.hFont;
              find  = true;
          }
          else
          {
              DeleteObject(myfont.hFont);
              m_Font.erase(itr);
          }
      }
      if(!find)
      {
          MyFont myfont;
          myfont.hFont = ::CreateFont(size,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
                         OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                         DEFAULT_PITCH|FF_DONTCARE,wnd.fontName);
          strcpy_s(myfont.fontName, sizeof(myfont.fontName), wnd.fontName);
          m_Font.insert(pair<int, MyFont>(size,myfont));
          wnd.hFont = &m_Font[size].hFont;
      }
  }

  int xx = convertX(wnd, x);
  int yy = convertY(wnd, y);
  RECT rect = { xx, yy , wnd.cx, wnd.cy };
  int len = (int)strlen(str);
  SIZE psize = {0,0};

  HDC hWDC = ::GetDC(wnd.hWnd);
  if(wnd.showLayer==wnd.writeLayer)
  {
      ::SelectObject(hWDC,*wnd.hFont);
      ::SetTextColor(hWDC,RGB(wnd.r,wnd.g,wnd.b));//�����F�ݒ�iNULL=Black�j
      ::SetBkMode(hWDC,TRANSPARENT);              //�����w�i��h��Ԃ��Ȃ�

      if(::GetTextExtentPoint32(hWDC,str,len,&psize))
      { //�e�L�X�g�T�C�Y���������炷
          rect.top-=psize.cy;
      }
      ::DrawText(hWDC,str,len,&rect,DT_TOP|DT_LEFT);
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
      ::SelectObject(hDC,*wnd.hFont);
      ::SetTextColor(hDC,RGB(wnd.r,wnd.g,wnd.b));//�����F�ݒ�iNULL=Black�j
      ::SetBkMode(hDC,TRANSPARENT);              //�����w�i��h��Ԃ��Ȃ�

      if(::GetTextExtentPoint32(hDC,str,len,&psize))
      { //�e�L�X�g�T�C�Y���������炷
          rect.top-=psize.cy;
      }

      ::DrawText(hDC,str,len,&rect,DT_TOP|DT_LEFT);

      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
  return len;
}

/**
 * @brief      �`��t�H���g�̐ݒ�
 * @ingroup    CEggX
 * @param[in]  wn    �ύX����E�B���h�E�̔ԍ�
 * @param[in]  argsformat, ... �t�H���g�Z�b�g�̖��O
 * @retval     -1 �G���[
 * @retval     0  �擾����
 * @retval     1  ��փt�H���g�̎擾����
*/
int  CEggX::gsetfontset( unsigned wn, const char *argsformat, va_list argptr)
{
  if(wn>=(unsigned)m_MaxWindow)return -1;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return -1;

  char str[32];
  int len = _vsnprintf_s(str, sizeof(str), _TRUNCATE, argsformat, argptr);
  if (len == -1)str[sizeof(str) - 1] = '\0';
  else str[len] = '\0';

  strncpy_s(wnd.fontName, sizeof(wnd.fontName), str, sizeof(wnd.fontName));
  wnd.fontsize = 0;

  return 0;
}

/**
* @brief �܂����`��
* @ingroup    CEggX
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] �܂���̊e�_��x���W
* @param[in]  y[] �܂���̊e�_��y���W
* @param[in]  n �_�̐�
* @retval      �Ȃ�
*/
void CEggX::drawlines(unsigned wn, const double x[], const double y[], int n)
{
  if (wn >= (unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if (!wnd.hWnd)return;

  POINT *p = new POINT[n];
  for (int i = 0; i < n; i++) {
    p[i].x = convertX(wnd, x[i]);
    p[i].y = convertY(wnd, y[i]);
  }

  HBRUSH hBrushNull = (HBRUSH)::GetStockObject(NULL_BRUSH);

  HDC hWDC = ::GetDC(wnd.hWnd);
  if (wnd.showLayer == wnd.writeLayer)
  {
    ::SelectObject(hWDC, hBrushNull);
    ::SelectObject(hWDC, wnd.hPen);
    ::SetBkMode(hWDC, TRANSPARENT);

    ::Polyline(hWDC, p, n);
  } else
  {
    HDC hDC = ::CreateCompatibleDC(hWDC);
    ::SelectObject(hDC, wnd.hBitmap[wnd.index]);
    ::SelectObject(hDC, hBrushNull);
    ::SelectObject(hDC, wnd.hPen);
    ::SetBkMode(hDC, TRANSPARENT);

    ::Polyline(hDC, p, n);
    ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd, hWDC);
  delete p;
}

/**
* @brief ���p�`��`��
* @ingroup    CEggX
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] ���p�`�̊e�_��x���W
* @param[in]  y[] ���p�`�̊e�_��y���W
* @param[in]  n �_�̐�
* @retval      �Ȃ�
*/
void CEggX::drawpoly(unsigned wn, const double x[], const double y[], int n)
{
  if (wn >= (unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if (!wnd.hWnd)return;

  POINT *p = new POINT[n];
  for (int i = 0; i < n; i++) {
    p[i].x = convertX(wnd, x[i]);
    p[i].y = convertY(wnd, y[i]);
  }

  HBRUSH hBrushNull = (HBRUSH)::GetStockObject(NULL_BRUSH);

  HDC hWDC = ::GetDC(wnd.hWnd);
  if (wnd.showLayer == wnd.writeLayer)
  {
    ::SelectObject(hWDC, hBrushNull);
    ::SelectObject(hWDC, wnd.hPen);
    ::SetBkMode(hWDC, TRANSPARENT);

    ::Polygon(hWDC, p, n);
  } else
  {
    HDC hDC = ::CreateCompatibleDC(hWDC);
    ::SelectObject(hDC, wnd.hBitmap[wnd.index]);
    ::SelectObject(hDC, hBrushNull);
    ::SelectObject(hDC, wnd.hPen);
    ::SetBkMode(hDC, TRANSPARENT);

    ::Polygon(hDC, p, n);
    ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd, hWDC);
  delete p;
}

/**
* @brief ���p�`��h��ׂ�
* @ingroup    CEggX
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] ���p�`�̊e�_��x���W
* @param[in]  y[] ���p�`�̊e�_��y���W
* @param[in]  n �_�̐�
* @param[in]  i �h��ׂ����̌`��
* @retval      �Ȃ�
*/
void CEggX::fillpoly(unsigned wn, const double x[], const double y[], int n, int i)
{
  if (wn >= (unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if (!wnd.hWnd)return;

  POINT *p = new POINT[n];
  for (int i = 0; i < n; i++) {
    p[i].x = convertX(wnd, x[i]);
    p[i].y = convertY(wnd, y[i]);
  }

  HBRUSH hBrushNull = (HBRUSH)::GetStockObject(NULL_BRUSH);

  int mode;
  if (i == 0) {
    mode = ALTERNATE;
  } else {
    mode = WINDING;
  }

  HDC hWDC = ::GetDC(wnd.hWnd);
  if (wnd.showLayer == wnd.writeLayer)
  {
    ::SelectObject(hWDC, wnd.hPenF);
    ::SelectObject(hWDC, wnd.hBrush);
    ::SetPolyFillMode(hWDC, mode);

    ::Polygon(hWDC, p, n);
  } else
  {
    HDC hDC = ::CreateCompatibleDC(hWDC);
    ::SelectObject(hDC, wnd.hBitmap[wnd.index]);
    ::SelectObject(hDC, wnd.hPenF);
    ::SelectObject(hDC, wnd.hBrush);
    ::SetPolyFillMode(hDC, mode);

    ::Polygon(hDC, p, n);
    ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd, hWDC);
  delete p;
}

/**
* @brief �����̓_��`��
* @ingroup    CEggX
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] �e�_��x���W
* @param[in]  y[] �e�_��y���W
* @param[in]  n �_�̐�
* @retval      �Ȃ�
*/
void CEggX::drawpts(unsigned wn, const double x[], const double y[], int n)
{
  if (wn >= (unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if (!wnd.hWnd)return;

  COLORREF rgb = RGB(wnd.r, wnd.g, wnd.b);
  HDC hWDC = ::GetDC(wnd.hWnd);
  if (wnd.showLayer == wnd.writeLayer)
  {
    for (int i = 0; i < n; i++) {
      int xx = convertX(wnd, x[i]);
      int yy = convertY(wnd, y[i]);
      ::SetPixelV(hWDC, xx, yy, rgb);
    }
  } else {
    HDC hDC = ::CreateCompatibleDC(hWDC);
    ::SelectObject(hDC, wnd.hBitmap[wnd.index]);
    for (int i = 0; i < n; i++) {
      int xx = convertX(wnd, x[i]);
      int yy = convertY(wnd, y[i]);
      ::SetPixelV(hDC, xx, yy, rgb);
    }
    ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd, hWDC);
}

/**
 * @brief      �L�[�{�[�h������͂��ꂽ������Ԃ�
 * @ingroup    CEggX
 * @retval     <0 ���͂Ȃ��i�m���u���b�L���O���[�h�̏ꍇ�j
 * @retval     �����ꂽ�L�[�̕����R�[�h
*/
int CEggX::ggetch()
{
	if ( m_Nonblock == DISABLE ) {
        do {
            ResetEvent(m_eventHandle);
            if (WaitForSingleObject(m_eventHandle, INFINITE) != WAIT_OBJECT_0) {
                cerr << "Error of WaitForSingleObject()" << endl;
                return -99;
            }
        } while (m_eventType != KeyPress);
	}	

    return m_eventKey;
}

/**
 * @brief      �C�x���g�擾�̓��샂�[�h��ݒ肷��
 * @ingroup    CEggX
 * @param[in]  flag ENABLE �m���u���b�L���O���[�h�CDISABLE �u���b�L���O���[�h
 * @retval     �Ȃ�
*/
void CEggX::gsetnonblock( int flag )
{
  if ( flag == ENABLE ) {
	  m_Nonblock = true;
  } else if ( flag == DISABLE ) {
	  m_Nonblock = false;
  }
}

/**
* @brief      �S�ẴE�B���h�E�̃}�E�X��L�[�{�[�h�̓��͏���Ԃ��D
* @ingroup    CEggX
* @param[out]  type �C�x���g�̎��
* @param[out]  button �{�^���̔ԍ��i�}�E�X�̏ꍇ�j�C�L�[�R�[�h�i�L�[�{�[�h�̏ꍇ�j
* @param[out]  x �}�E�X�|�C���^��x���W�i�A�v���P�[�V�������W�n�j
* @param[out]  y �}�E�X�|�C���^��y���W�i�A�v���P�[�V�������W�n�j
* @retval      ���͂̍������E�B���h�E�ԍ�
*/
int CEggX::ggetevent(int *type, int *button, double *x, double *y)
{
    if (m_Nonblock == DISABLE) {
        ResetEvent(m_eventHandle);
        if (WaitForSingleObject(m_eventHandle, INFINITE) != WAIT_OBJECT_0) {
            cerr << "Error of WaitForSingleObject()" << endl;
            return -99;
        }
    }

    if (type != NULL) {
        *type = m_eventType;
    }
    if (button != NULL) {
        if (m_eventType == KeyPress) {
            *button = m_eventKey;
        } else {
            *button = m_eventButton;
        }
    }
    if (x != NULL) {
        *x = invertX(m_Window.at(m_eventWinNum), m_eventX);
    }
    if (y != NULL) {
        *y = invertY(m_Window.at(m_eventWinNum), m_eventY);
    }
    return m_eventWinNum;
}

/**
* @brief      �S�ẴE�B���h�E�̃}�E�X��L�[�{�[�h�̓��͏���Ԃ��i�L�[�ƃ{�^���̂݁j�D
* @ingroup    CEggX
* @param[out]  type �C�x���g�̎��
* @param[out]  button �{�^���̔ԍ��i�}�E�X�̏ꍇ�j�C�L�[�R�[�h�i�L�[�{�[�h�̏ꍇ�j
* @param[out]  x �}�E�X�|�C���^��x���W�i�A�v���P�[�V�������W�n�j
* @param[out]  y �}�E�X�|�C���^��y���W�i�A�v���P�[�V�������W�n�j
* @retval      ���͂̍������E�B���h�E�ԍ�
*/
int CEggX::ggetxpress(int *type, int *button, double *x, double *y)
{
    if (m_Nonblock == DISABLE) {
        do {
            ResetEvent(m_eventHandle);
            if (WaitForSingleObject(m_eventHandle, INFINITE) != WAIT_OBJECT_0) {
                cerr << "Error of WaitForSingleObject()" << endl;
                return -99;
            }
        } while (m_eventType != KeyPress && m_eventType != ButtonPress);
    }

    if (type != NULL) {
        *type = m_eventType;
    }
    if (button != NULL) {
        if (m_eventType == KeyPress) {
            *button = m_eventKey;
        } else {
            *button = m_eventButton;
        }
    }
    if (x != NULL) {
        *x = invertX(m_Window.at(m_eventWinNum), m_eventX);
    }
    if (y != NULL) {
        *y = invertY(m_Window.at(m_eventWinNum), m_eventY);
    }
    return m_eventWinNum;
}


/**
* @brief      �C�x���g�̌��ʂ��������ސ�̃|�C���^��ݒ肷��D
* @ingroup    CEggX
* @param[in]  pwin WindowId���������ސ�̃|�C���^
* @param[in]  px MouseX���������ސ�̃|�C���^
* @param[in]  py MouseY���������ސ�̃|�C���^
* @param[in]  pbutton MouseButton���������ސ�̃|�C���^
* @param[in]  ppressed MousePressed���������ސ�̃|�C���^
* @retval      �Ȃ�
*/
void CEggX::setmouse(int *pwin, double *px, double *py, int *pbutton, bool *ppressed)
{
    m_pWindowId = pwin;
    m_pMouseX = px;
    m_pMouseY = py;
    m_pMouseButton = pbutton;
    m_pMousePressed = ppressed;
}


/**
 * @brief      EggX�̃��b�Z�[�W����
 * @ingroup    CEggX
 * @param[in]  msg ���b�Z�[�W
 * @return     �X���b�h�I���p�����[�^
*/
void CEggX::EggMessage(LONGLONG msg)
{
  UINT msgID = EGGX_MSG(msg);
  switch(msgID)
  {
  case WINDOW_CREATE:
      {
          UINT x = EGGX_HIMSG(msg);
          UINT y = EGGX_LOMSG(msg);
          m_createWindowNum = CreateEggXWindow(x,y);
          ::SetEvent(m_Com);
          ::WaitForSingleObject(m_Ack,INFINITE);
      }
      break;
  }
}


/**
 * @brief      �X���b�h�̃G���g���[�|�C���g
 * @ingroup    CEggX
 * @param[in]  param �N���X�̃|�C���^
 * @return     �X���b�h�I���p�����[�^
*/
DWORD CEggX::ThreadEntry(void *param)
{
  assert(param&&"eggx thread entry point.");
  return ((CEggX*)param)->ThreadProc();
}


/**
 * @brief      �X���b�h�̏���
 * @ingroup    CEggX
 * @return     �X���b�h�I���p�����[�^
*/
DWORD CEggX::ThreadProc()
{
  m_run = true;
  MSG msg;
  //�V���[�g�J�b�g�L�[����
  HACCEL hAccelTable = LoadAccelerators(m_hInst, (LPCTSTR)m_accesstable);

  while(1)
  {
      ::WaitForSingleObject(m_Event,m_SleepTime);
      if(!m_run)break;

      while(!m_MSG.empty())
      {
          LONGLONG data = m_MSG.front();
          m_MSG.pop_front();
          EggMessage(data);
      }
      int num = m_NumWind;
      for(int i=0;i<m_MaxWindow&&num;i++)
      {
          if(!m_Window.at(i).hWnd)continue;
          num--;
          EggXWindow &wnd = m_Window.at(i);
          // ���C�� ���b�Z�[�W ���[�v :
          while (::PeekMessage(&msg, wnd.hWnd, 0, 0, PM_REMOVE)) 
          {
              if(hAccelTable)
              {
                  if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
                  {
                      TranslateMessage(&msg);
                      DispatchMessage(&msg);
                  }
              }
              else
              {
                  TranslateMessage(&msg);
                  DispatchMessage(&msg);
              }
          }
      }
  }

  return 0;
}


/**
 * @brief      �E�B���h�E���b�Z�[�W���󂯎��֐�
 * @ingroup    CEggX
*/
INT_PTR CEggX::WindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  static CSelectWindow s_SelectWind;

  CEggX *This=(CEggX*)s_SelectWind.Serch(hWnd);
  if(uMsg==WM_CREATE)
  {//�E�B���h�E������
      CREATESTRUCT *cre = (CREATESTRUCT*)(((CREATESTRUCT*)lParam)->lpCreateParams);

      s_SelectWind.Add(hWnd,(void*)cre->lpCreateParams);
      This = (CEggX*)cre->lpCreateParams;
      return 0;
  }
  else if(uMsg==WM_DESTROY)
  {//�E�B���h�E���j�󂳂ꂽ�Ƃ�
      if(This)s_SelectWind.Delete(hWnd);
  }
  else if(uMsg==WM_USER_EXIT)
  {
      if(This)s_SelectWind.Delete(hWnd);
  }
  if(!This)
  {
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  return This->MsgProc(hWnd, uMsg, wParam, lParam);
}


/**
 * @brief      �E�B���h�E���b�Z�[�W����
 * @ingroup    CEggX
*/
INT_PTR CEggX::MsgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  //int wmId, wmEvent;
  if(!m_NumWind) return DefWindowProc(hWnd, uMsg, wParam, lParam);

  int winNum = -1;
  for (int i = 0; i<m_MaxWindow; i++) {
      if (m_Window.at(i).hWnd == hWnd) {
          winNum = i;
          break;
      }
  }
  EggXWindow window;
  if (winNum != -1)  window = m_Window.at(winNum);
  switch (uMsg)
  {
  case WM_USER_EXIT:
      if (winNum != -1)    Destoroy(winNum);
      ::DestroyWindow(hWnd);
      break;
  case WM_PAINT:
  {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      if (winNum == -1)return DefWindowProc(hWnd, uMsg, wParam, lParam);
      EggXWindow &wnd = m_Window.at(winNum);

      HDC hDCsrc = ::CreateCompatibleDC(hdc);
      int index = wnd.index - 1;
      if (index < 0)index = (sizeof(wnd.hBitmap) / sizeof(HBITMAP)) - 1;
      ::SelectObject(hDCsrc, wnd.hBitmap[index]);

      ::BitBlt(hdc, 0, 0, wnd.cx, wnd.cy, hDCsrc, 0, 0, SRCCOPY);

      ::DeleteDC(hDCsrc);
      EndPaint(hWnd, &ps);
  }
  break;
  case WM_DESTROY:
      PostQuitMessage(0);
      break;
  case WM_CLOSE:
      ::SendMessage(hWnd, WM_USER_EXIT, 0, 0);
      m_Close = true;
      break;
  case WM_KEYDOWN:
      if (keytable.find(wParam) != keytable.end()) {
          m_eventKey = keytable[wParam];
          m_eventType = KeyPress;
          m_eventWinNum = winNum;
          //cout << "WM_CHAR: m_eventKey = " << m_eventKey << endl;
          if (m_Nonblock == DISABLE) {
              SetEvent(m_eventHandle);
          }
      } else {
          return DefWindowProc(hWnd, uMsg, wParam, lParam);
      }
      break;
  case WM_CHAR:
      m_eventKey = wParam;
      m_eventType = KeyPress;
      m_eventWinNum = winNum;
      //cout << "WM_CHAR: m_eventKey = " << m_eventKey << endl;
      if (m_Nonblock == DISABLE) {
          SetEvent(m_eventHandle);
      }
      break;
  case WM_KEYUP:
      m_eventKey = -1;
      //cout << "WM_KEYUP: m_eventKey = " << m_eventKey << endl;
      break;
  case WM_LBUTTONDOWN:
      m_eventType = ButtonPress;
      m_eventButton = 1;
      m_eventWinNum = winNum;
      m_eventX = LOWORD(lParam);
      m_eventY = HIWORD(lParam);
      if (winNum != -1) {
          *m_pWindowId = winNum;
          *m_pMouseX = invertX(window, m_eventX);
          *m_pMouseY = invertY(window, m_eventY);
          *m_pMouseButton = m_eventButton;
          *m_pMousePressed = true;
      }
      //cout << "WM_LBUTTONDOWN: " << winNum << ", " << LOWORD(lParam) << ", " << HIWORD(lParam) << endl;
      if (m_Nonblock == DISABLE) {
          SetEvent(m_eventHandle);
      }
      break;
  case WM_MBUTTONDOWN:
      m_eventType = ButtonPress;
      m_eventButton = 2;
      m_eventWinNum = winNum;
      m_eventX = LOWORD(lParam);
      m_eventY = HIWORD(lParam);
      if (winNum != -1) {
          *m_pWindowId = winNum;
          *m_pMouseX = invertX(window, m_eventX);
          *m_pMouseY = invertY(window, m_eventY);
          *m_pMouseButton = m_eventButton;
          *m_pMousePressed = true;
      }
      //cout << "WM_MBUTTONDOWN: " << winNum << ", " << LOWORD(lParam) << ", " << HIWORD(lParam) << endl;
      if (m_Nonblock == DISABLE) {
          SetEvent(m_eventHandle);
      }
      break;
  case WM_RBUTTONDOWN:
      m_eventType = ButtonPress;
      m_eventButton = 3;
      m_eventWinNum = winNum;
      m_eventX = LOWORD(lParam);
      m_eventY = HIWORD(lParam);
      if (winNum != -1) {
          *m_pWindowId = winNum;
          *m_pMouseX = invertX(window, m_eventX);
          *m_pMouseY = invertY(window, m_eventY);
          *m_pMouseButton = m_eventButton;
          *m_pMousePressed = true;
      }
      //cout << "WM_RBUTTONDOWN: " << winNum << ", " << LOWORD(lParam) << ", " << HIWORD(lParam) << endl;
      if (m_Nonblock == DISABLE) {
          SetEvent(m_eventHandle);
      }
      break;
  case WM_LBUTTONUP:
  case WM_MBUTTONUP:
  case WM_RBUTTONUP:
      m_eventButton = 0;
      *m_pMousePressed = false;
      break;
  case WM_MOUSEMOVE:
      m_eventType = MotionNotify;
      m_eventButton = 0;
      m_eventWinNum = winNum;
      m_eventX = LOWORD(lParam);
      m_eventY = HIWORD(lParam);
      if (winNum != -1) {
          *m_pWindowId = winNum;
          *m_pMouseX = invertX(window, m_eventX);
          *m_pMouseY = invertY(window, m_eventY);
      }
      //cout << "WM_MOUSEMOVE: " << winNum << ", " << LOWORD(lParam) << ", " << HIWORD(lParam) << endl;
      if (m_Nonblock == DISABLE) {
          SetEvent(m_eventHandle);
      }
      break;
  default:
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  return 0;
}

/**
* @brief      �X�P�[���ϊ�
* @ingroup    CEggX
*/
inline int CEggX::scaleX(EggXWindow &wnd, double x)
{
    return roundInt(x / ((wnd.xe - wnd.xs) / wnd.cx));
}

/**
* @brief      �X�P�[���ϊ�
* @ingroup    CEggX
*/
inline int CEggX::scaleY(EggXWindow &wnd, double y)
{
    return roundInt(y / ((wnd.ye - wnd.ys) / wnd.cy));
}

/**
* @brief      ���W�ϊ�
* @ingroup    CEggX
*/
inline int CEggX::convertX(EggXWindow &wnd, double x)
{
    return scaleX(wnd, x - wnd.xs);
}

/**
* @brief      ���W�ϊ�
* @ingroup    CEggX
*/
inline int CEggX::convertY(EggXWindow &wnd, double y)
{
    return wnd.cy - scaleY(wnd, y - wnd.ys);
}

/**
* @brief      �t�X�P�[���ϊ�
* @ingroup    CEggX
*/
inline double CEggX::inverseScaleX(EggXWindow &wnd, int x)
{
    return x * ((wnd.xe - wnd.xs) / wnd.cx);
}

/**
* @brief      �t�X�P�[���ϊ�
* @ingroup    CEggX
*/
inline double CEggX::inverseScaleY(EggXWindow &wnd, int y)
{
    return y * ((wnd.ye - wnd.ys) / wnd.cy);
}

/**
* @brief      �t���W�ϊ�
* @ingroup    CEggX
*/
inline double CEggX::invertX(EggXWindow &wnd, int x)
{
    return inverseScaleX(wnd, x) + wnd.xs;
}

/**
* @brief      �t�W�ϊ�
* @ingroup    CEggX
*/
inline double CEggX::invertY(EggXWindow &wnd, int y)
{
    return inverseScaleX(wnd, wnd.cy - y) + wnd.ys;
}