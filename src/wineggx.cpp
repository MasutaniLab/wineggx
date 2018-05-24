/**
 * @file wineggx.cpp
 * @brief Windows��EGGX
 * @section Notes
 *  Copyright (C) 2007-2014 Team ODENS \n\n
 * @date �X�V���� �X�V��\n
 * 2014/10/16 ���J �۔��uWindows��EGGX Version 1.0�v�𖼏��\n
 * 2014/03/12 ���J �۔�\n
*/

/**
 * @addtogroup wineggx_back wineggx-WinEggX���C�u����
 * @brief Windows��eggx���C�u�����̑��������
**/

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winbase.h>
#include <windowsx.h>
#include <windef.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <cassert>

#include "wineggx.h"

using namespace std;

//==================================================================================
// CSelectWindow�N���X
//==================================================================================
class CSelectWindow
{
public:
  void AllDel();
  void Add(HWND hWnd,void *window);
  void Delete(HWND hWnd);
  void *Serch(HWND hWnd);

private:
  struct WindBuffer
  {
      HWND  hWnd;
      void *window;
  };
  list<WindBuffer> m_buff;
};
//==================================================================================

#define EGGX_WINDOW_CLASS_NAME "eggx_window_class_name"
#define WM_USER_EXIT WM_USER+1

#define MAKE_EGGX_MSG(msg,data1,data2) ((((LONGLONG)(((data1&0xffff)<<16)+(data2&0xffff)))<<32)+(LONGLONG)(msg&0xffffffff))
#define EGGX_HIMSG(x) (((UINT)(((LONGLONG)(x))>>48))&0xffff)
#define EGGX_LOMSG(x) (((UINT)(((LONGLONG)(x))>>32))&0xffff)
#define EGGX_MSG(x) ((UINT)((LONGLONG)(x)&0xffffffff))

enum{
  WINDOW_CREATE = 1, //�E�B���h�E�쐬�t���O
};

//==================================================================================
// RGB
//==================================================================================
class rgb {
public:
	int r;
	int g;
	int b;
	rgb(){r=0; g=0; b=0;}
	rgb(int ar, int ag, int ab) {r=ar; g=ag; b=ab;}
};

map<string, rgb> rgbtable;
//==================================================================================


/**
 * @struct  EggXWindow
 * @brief   �E�B���h�E���Ǘ����邽�߂̍\����
 * @ingroup wineggx_back
 * @section Notes
 *  hWnd    �E�B���h�E�n���h��\n
 *  hBitmap �r�b�g�}�b�v�I�u�W�F�N�g�n���h��\n
 *  hPen    �y���I�u�W�F�N�g�n���h��\n
 *  hBrush  �u���V�I�u�W�F�N�g�n���h��\n
 *  hFont   �t�H���g�I�u�W�F�N�g�n���h��\n
 *  writeLayer �`��惌�C���[\n
 *  showLayer  �\�����C���[\n
 *  xs,ys      �E�B���h�E�����̍��W
 *  xe,ye      �E�B���h�E�E��̍��W
 *  r,g,b      �`��F
*/
struct EggXWindow{
  HWND    hWnd;
  HBITMAP hBitmap[8];//�E�B���h�D����8 ���̃��C��
  HPEN    hPen;
  HPEN    hPenF; //�h��Ԃ��}�`�p�̃y��
  HBRUSH  hBrush;
  HBRUSH  hBrushB; //�w�i�p�̃u���V
  HFONT  *hFont;
  int     index;
  int     fontsize;
  char    fontName[32];
  int     writeLayer;
  int     showLayer;
  double  xs,ys,xe,ye;
  int     r,g,b;
  int     rB, gB, bB;
  int	  width, style;
  int     cx,cy;
  int     lx,ly;
};

/**
 * @struct  MyFont
 * @brief   �t�H���g�Ǘ��̂��߂̍\����
 * @ingroup wineggx_back
 * @section Notes
 *  hFont    �t�H���g�I�u�W�F�N�g
 *  fontName �t�H���g��
*/
struct MyFont{
  HFONT   hFont;
  char    fontName[32];
};


/**
 * @ingroup wineggx_back
 * @class CEggX
 * @brief Windows��EGGX
*/
class CEggX{
public:
  CEggX();
  virtual ~CEggX();

  int  Initialize(HINSTANCE hInst,LONGLONG AccessTableId);

private:
    int  CreateThread();
    void Release();

public:
  void DestoroyAll();
  void Destoroy(int wn);

  int  CreateEggXWindow(int xsize,int ysize);

//eggx
  int  gopen(int xsize,int ysize);
  void gclose( unsigned wn );
  void gcloseall( void );
  int  winname( unsigned wn, const char *name );
  void window( unsigned wn, double xs, double ys, double xe, double ye );
  void layer( unsigned wn, int lys, int lyw );
  void copylayer( unsigned wn, int lysrc, int lydest );
  void newcolor( unsigned wn, char *str);
  void newrgbcolor( unsigned wn, int r, int g, int b );
  void newhsvcolor( unsigned wn, int h, int s, int v );
  void gsetbgcolor( unsigned wn, char *str );
  void gsetbgcolorrgb( unsigned wn, int r, int g, int b );
  void gclr( unsigned wn );
  void pset( unsigned wn,double x,double y );
  void drawline( unsigned wn, double x0, double y0, double x1, double y1 );
  void moveto( unsigned wn, double x, double y );
  void lineto( unsigned wn, double x, double y );
  void line( unsigned wn,double x,double y,int mode );
  void circle( unsigned wn, double xcen, double ycen, double xrad, double yrad );
  void fillcirc( unsigned wn, double xcen, double ycen, double xrad, double yrad );
  void drawarc( unsigned wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir );
  void fillarc( unsigned wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir );
  void drawrect( unsigned wn, double x, double y, double w, double h );
  void fillrect( unsigned wn, double x, double y, double w, double h );
  int  drawstr( unsigned wn, double x, double y, int size, double theta,const char *str );
  int  gsetfontset( unsigned wn, const char *name );
  void newlinewidth( unsigned wn, int width );
  void newlinestyle( unsigned wn, int style );
  int ggetch();
  void gsetnonblock( int flag );

private:
  // EGGX���b�Z�[�W����
  void EggMessage(LONGLONG msg);

  // �X���b�h�G���g���[�|�C���g
  static DWORD WINAPI ThreadEntry(void *param);
  DWORD ThreadProc();

  // �E�B���h�E���b�Z�[�W����
  static  INT_PTR CALLBACK WindowProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
  virtual INT_PTR MsgProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);

  //���W�ϊ�
  int scaleX(EggXWindow &wnd, double x);
  int scaleY(EggXWindow &wnd, double y);
  int convertX(EggXWindow &wnd, double x);
  int convertY(EggXWindow &wnd, double y);
  //�l�̌ܓ� double �� int
  int roundInt(double d)
  {
    return int(d+0.5-(d<0));
  }

protected:
  HANDLE    m_Thread; // �X���b�h�n���h��
  HANDLE    m_Event;  // �C�x���g�n���h��
  HANDLE    m_Ack;
  HANDLE    m_Com;
  HINSTANCE m_hInst;
  int       m_NumWind;
  int       m_MaxWindow;
  vector<EggXWindow> m_Window;
  map<int,MyFont>    m_Font;
  list<LONGLONG>     m_MSG;
  int       m_createWindowNum;

  bool      m_run;//�X���b�h����t���O
  DWORD     m_SleepTime;
  LONGLONG  m_accesstable;
  bool      m_Init;
  bool      m_Close;
  bool      m_Nonblock;
  int       m_Key;
  HANDLE    m_KeyEvent;
};


// �O���[�o���ϐ�
CEggX gEggX;

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
m_Key(-1)
{
  m_Event = ::CreateEvent(NULL,false,false,NULL);
  m_Ack   = ::CreateEvent(NULL,false,false,NULL);
  m_Com   = ::CreateEvent(NULL,false,false,NULL);

  m_KeyEvent = ::CreateEvent(NULL,false,false,NULL);

  m_Window.resize(m_MaxWindow);
  for(int i=0;i<m_MaxWindow;i++)
  {
      EggXWindow &wnd = m_Window.at(i);
      memset(&wnd,0,sizeof(EggXWindow));
  }
#include "rgbtable.h"
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
  sprintf(windowname,"%s%d","eggx window",num);


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
  sprintf(wnd.fontName,"�l�r �S�V�b�N");
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
 * @brief      �E�B���h�D�̃^�C�g����ύX����
 * @ingroup    CEggX
 * @param[in]  wn �^�C�g����ύX����E�B���h�E�̔ԍ�
 * @param[in]  ... �ݒ肷��E�B���h�E�̕�����
 * @return     �ݒ肵���E�B���h�D�^�C�g���̕�����̒���
*/
int  CEggX::winname( unsigned wn, const char *name )
{
  if(wn>=(unsigned)m_MaxWindow)return 0;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return 0;

  SetWindowText(wnd.hWnd,name);
  return (int)strlen(name);
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
void CEggX::newcolor( unsigned wn, char *str )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  rgb a;
  if ( str[0] == '#' ) {
	  if ( sscanf(str, "#%2x%2x%2x", &a.r, &a.g, &a.b) != 3 ) {
		  cerr << str << " cannot decode" << endl;
		  return;
	  }
  } else {
	  map<string, rgb>::iterator itr;
	  itr = rgbtable.find(str);
	  if ( itr == rgbtable.end() ) {
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
void CEggX::gsetbgcolor( unsigned wn, char *str )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

    rgb a;
  if ( str[0] == '#' ) {
	  if ( sscanf(str, "#%2x%2x%2x", &a.r, &a.g, &a.b) != 3 ) {
		  cerr << str << " cannot decode" << endl;
		  return;
	  }
  } else {
	  map<string, rgb>::iterator itr;
	  itr = rgbtable.find(str);
	  if ( itr == rgbtable.end() ) {
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
 * @param[in]  ...   ������
 * @return     ���ۂɕ`�悵��������̒���
*/
int  CEggX::drawstr( unsigned wn, double x, double y, int size, double theta,const char *str )
{
  if(wn>=(unsigned)m_MaxWindow)return 0;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return 0;

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
          strcpy(myfont.fontName,wnd.fontName);
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
 * @param[in]  ... �t�H���g�Z�b�g�̖��O
 * @retval     -1 �G���[
 * @retval     0  �擾����
 * @retval     1  ��փt�H���g�̎擾����
*/
int  CEggX::gsetfontset( unsigned wn, const char *name )
{
  if(wn>=(unsigned)m_MaxWindow)return -1;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return -1;

  strncpy(wnd.fontName,name,sizeof(wnd.fontName));
  wnd.fontsize = 0;

  return 0;
}
/**
 * @brief      �L�[�{�[�h������͂��ꂽ������Ԃ�
 * @ingroup    CEggX
 * @param[in]  �Ȃ�
 * @retval     <0 ���͂Ȃ��i�m���u���b�L���O���[�h�̏ꍇ�j
 * @retval     �����ꂽ�L�[�̕����R�[�h
*/
int CEggX::ggetch()
{
	if ( m_Nonblock == DISABLE ) {
		ResetEvent(m_KeyEvent);
		if( WaitForSingleObject(m_KeyEvent, INFINITE) != WAIT_OBJECT_0 ) {
			cerr << "Error of WaitForSingleObject()" << endl;
			return -99;
		}
	}	

    return m_Key;
}

/**
 * @brief      ggetch()�̓��샂�[�h��ݒ肷��
 * @ingroup    CEggX
 * @param[in]  ENABLE �m���u���b�L���O���[�h�ɂ���
 * @param[in]  DISABLE �u���b�L���O���[�h�ɂ���
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
 * @brief      �C�ӂ̃T�C�Y�̃O���t�B�b�N�X��ʂ��J��
 * @ingroup    wineggx
 * @param[in]  xsize �E�B���h�E�̉���
 * @param[in]  ysize �E�B���h�E�̏c��
 * @return     �Ԃ�l�ɂ�EGGX�Ŏg�p����C�E�B���h�E�ԍ����Ԃ��Ă��܂��D
 * @retval     -1 �G���[
*/
int gopen(int xsize,int ysize)
{
  assert(xsize&&ysize&&"eggx set window size.");

  return gEggX.gopen(xsize,ysize);
}


/**
 * @brief      �O���t�B�b�N�X�E�B���h�E�����
 * @ingroup    wineggx
 * @param[in]  wn �E�B���h�E�ԍ�
*/
void gclose( int wn )
{
  return gEggX.gclose(wn);
}


/**
 * @brief      ���ׂẴO���t�B�b�N�X�p�E�B���h�D�����
 * @ingroup    wineggx
*/
void gcloseall( void )
{
  return gEggX.gcloseall();
}


/**
 * @brief      �E�B���h�D�̃^�C�g����ύX����
 * @ingroup    wineggx
 * @param[in]  wn �^�C�g����ύX����E�B���h�E�̔ԍ�
 * @param[in]  ... �ݒ肷��E�B���h�E�̕�����
 * @return     �ݒ肵���E�B���h�D�^�C�g���̕�����̒���
*/
int winname( int wn, const char *argsformat, ... )
{
  assert(argsformat&&"eggx set window names.");

  char str[256];

  va_list argptr;              //�ϐ��������X�g
  va_start(argptr,argsformat); //initalize va_ function
  int len = _vsnprintf(str,sizeof(str),argsformat,argptr);//
  va_end(argptr);              //close     va_ function

  if(len==-1)str[sizeof(str)-1]=NULL;//NULL�����t��
  else str[len]=NULL;

  return gEggX.winname(wn,str);
}


/**
 * @brief      ���W�n�̕ύX
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  xs ������X���W
 * @param[in]  ys ������Y���W
 * @param[in]  xe �E���X���W
 * @param[in]  ye �E���Y���W
*/
void window( int wn, double xs, double ys, double xe, double ye )
{
  return gEggX.window(wn,xs,ys,xe,ye);
}


/**
 * @brief      ���C���̐ݒ������
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  lys �\�����C���[( 0 or 1 )
 * @param[in]  lyw �������ݐ惌�C���[( 0 or 1 )
 * @section Notes
 *  ���ݕ\�����Ă��郌�C���ɑ΂���(lys == lyw �̏ꍇ��) �A�����ĕ`��֐������s����ƁC�ǂ��`��p
 *  �t�H�[�}���X�������܂���D�����ȕ`�悪�K�v�ȏꍇ�ɂ́C���ݕ\�����Ă��Ȃ����C���ɑ΂��ĕ`�悵�C
 *  �Ō�ɕ\�����C����layer() �֐��Ő؂�ւ��邩�Ccopylayer() �֐�(x2.5.10) �ŕ`�惌�C���̉摜��\
 *  �����C���ɃR�s�[����悤�ɂ��܂��D\n
 *  �f�t�H���g�ł�layer(wn,0,0) �̏�ԂƂȂ��Ă��܂��D
*/
void layer( int wn, int lys, int lyw )
{
  return gEggX.layer(wn,lys,lyw);
}


/**
 * @brief      ���C���̃R�s�[������
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  lysrc  �R�s�[�����C���[�ԍ�
 * @param[in]  lydest �R�s�[�惌�C���[�ԍ�
 * @section Notes
 *  wn �̃E�B���h�E�ԍ��́C���C��lysrc �̉摜�����C��lydest �ɂ��̂܂܃R�s�[���܂��D
 *  ���̃R�s�[�͏u���ɍs���邽�߁C�A�j���[�V�����̍Đ��Ɏg�����Ƃ��ł��܂��D
*/
void copylayer( int wn, int lysrc, int lydest )
{
   return gEggX.copylayer(wn,lysrc,lydest);
}

/**
 * @brief      �`��F�̕ύX
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 *  wn �Ŏw�肳�ꂽ�E�B���h�D�ł̕`��F��ύX���܂��Dargsformat(�Ƃ���ɑ�������)�Ŏw�肳��镶�����
 *  �w�i�F�ɐݒ肵�܂��D2�߂̈���argsformat�ȍ~�́CC �W���֐���printf() �֐��̏ꍇ�Ɠ��l�̉ψ���
 *  �ƂȂ��Ă��܂��D���̔w�i�F�̕�����ɂ́CX�E�B���h�E�Œ�`����Ă���F���C"#c0c0ff"�̂悤�ɁC16�i��
 *  ��Red,Green,Blue ���w�肵�܂��D
*/
void newcolor( int wn, const char *argsformat, ... ) {
  char str[256];

  va_list argptr;              //�ϐ��������X�g
  va_start(argptr,argsformat); //initalize va_ function
  int len = _vsnprintf(str,sizeof(str),argsformat,argptr);//
  va_end(argptr);              //close     va_ function

  if(len==-1)str[sizeof(str)-1]=NULL;//NULL�����t��
  else str[len]=NULL;
  return gEggX.newcolor(wn, str);
}

/**
 * @brief      �`��F�̕ύX
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  r  �ԐF ( 0 to 255 )
 * @param[in]  g  �ΐF ( 0 to 255 )
 * @param[in]  b  �F ( 0 to 255 )
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ł̕`��F��ύX���܂��Dr,g,b �ɂ͂��ꂼ��Red,Green,Blue �̋P�x��
 *  256 �i�K�̐���(0�`255) �Ŏw�肵�܂��D
*/
void newrgbcolor( int wn, int r, int g, int b )
{
  return gEggX.newrgbcolor(wn,r,g,b);
}
/**
 * @brief      �`��F�̕ύX
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  h  �F�� ( 0 to 359 )
 * @param[in]  s  �ʓx ( 0 to 255 )
 * @param[in]  v  ���x ( 0 to 255 )
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ł̕`��F��ύX���܂��Dh,s,v �ɂ͂��ꂼ��CHue,Satulation,Value��
 *  �w�肵�܂�)�Ds��v��256�i�K�̐���(0�`255)���Ch��0�`359�܂ł̐���(�p�x)���w�肵�܂��D
*/
void newhsvcolor( int wn, int h, int s, int v )
{
  return gEggX.newhsvcolor(wn,h,s,v);
}

/**
 * @brief      �E�B���h�D�̔w�i�F�̕ύX
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 *  wn �Ŏw�肳�ꂽ�E�B���h�D�̔w�i�F��ύX���܂��Dargsformat(�Ƃ���ɑ�������)�Ŏw�肳��镶�����
 *  �w�i�F�ɐݒ肵�܂��D2�߂̈���argsformat�ȍ~�́CC �W���֐���printf() �֐��̏ꍇ�Ɠ��l�̉ψ���
 *  �ƂȂ��Ă��܂��D���̔w�i�F�̕�����ɂ́CX�E�B���h�E�Œ�`����Ă���F���C"#c0c0ff"�̂悤�ɁC16�i��
 *  ��Red,Green,Blue ���w�肵�܂��D
*/
void gsetbgcolor( int wn, const char *argsformat, ... )
{
  char str[256];

  va_list argptr;              //�ϐ��������X�g
  va_start(argptr,argsformat); //initalize va_ function
  int len = _vsnprintf(str,sizeof(str),argsformat,argptr);//
  va_end(argptr);              //close     va_ function

  if(len==-1)str[sizeof(str)-1]=NULL;//NULL�����t��
  else str[len]=NULL;
  return gEggX.gsetbgcolor(wn,str);
}

/**
 * @brief      �E�B���h�D�̔w�i�F�̕ύX�y���ӁF�I���W�i���ɂ͂Ȃ��֐��z
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  r  �ԐF ( 0 to 255 )
 * @param[in]  g  �ΐF ( 0 to 255 )
 * @param[in]  b  �F ( 0 to 255 )
 * @section Notes
 *  wn �Ŏw�肳�ꂽ�E�B���h�D�̔w�i�F��ύX���܂��Dr,g,b �ɂ͂��ꂼ��Red,Green,Blue �̋P�x��
 *  256 �i�K�̐���(0�`255) �Ŏw�肵�܂��D
*/
void gsetbgcolorrgb( int wn, int r, int g, int b )
{
  return gEggX.gsetbgcolorrgb(wn,r,g,b);
}

/**
 * @brief      �`�惌�C���̑S����
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�̕`�惌�C����w�i�F�Ɏw�肳�ꂽ�F�ŏ��������܂��D
*/
void gclr( int wn )
{
  return gEggX.gclr(wn);
}

/**
 * @brief      �����̕ύX
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  width ����
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�Ő���`�����̐�����ύX���܂��D
 *  �f�t�H���g�ł͕�1 ���ݒ肳��Ă��܂��D
*/
void newlinewidth( int wn, int width )
{
  return gEggX.newlinewidth(wn,width);
}

/**
 * @brief      ���̃X�^�C���̕ύX
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  style ���̃X�^�C��
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�Ő���`�����̃X�^�C����ύX���܂��D
 *  ����style �ɗ^���鎖���ł���l�́CLineSolid(����) ��LineOnOffDash(�_��) �ł��D
 *  �f�t�H���g�ł͎���(LineSolid) ���ݒ肳��Ă��܂��D
 *  Windows API�̎d�l��CLineSolid�͐���1�ȉ��łȂ��ƗL���łȂ��D
*/
void newlinestyle( int wn, int style )
{
  return gEggX.newlinestyle(wn,style);
}

/**
 * @brief      �_�̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y �`�����W
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɓ_��`�悵�܂��D
*/
void pset( int wn,double x,double y )
{
  return gEggX.pset(wn,x,y);
}

/**
 * @brief      �����̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x0,y0 �[�_�̍��W
 * @param[in]  x1,y1 �[�_�̍��W
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D��(x0, y0) ����(x1, y1) �ɒ�����`�悵�܂��D
*/
void drawline( int wn, double x0, double y0, double x1, double y1 )
{
  return gEggX.drawline(wn,x0,y0,x1,y1);
}

/**
 * @brief      ������`���߂̏����ʒu�̐ݒ�
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y �`�����W
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɁClineto() �֐��̂��߂̏����ʒu��(x, y) �ɐݒ肵�܂��D
*/
void moveto( int wn, double x, double y )
{
  return gEggX.moveto(wn,x,y);
}

/**
 * @brief      �A���I�ɒ�����`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y ���W
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɁC�ȑOmoveto()�܂���lineto() ���Ă΂ꂽ���Ɏw�肳�ꂽ���W����
 *  (x, y) �֒����������܂��D
*/
void lineto( int wn, double x, double y )
{
  return gEggX.lineto(wn,x,y);
}

/**
 * @brief      �����̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y �`�����W
 * @param[in]  mode �`�惂�[�h
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɒ�����`�悵�܂��D
 *  mode �� @ref PENDOWN ���w�肷��ƈȑOline() �֐����Ă΂ꂽ�_����C
 *  (x,y) �֒����������܂��Dmode ��PENUP ���w�肷���(x,y) ��
 *  line() �֐��̏����ʒu�ɐݒ肵�܂��Dmode=PENDOWN �Ńy�������낵��
 *  �`��Cmode=PENUP �Ńy�����グ�Ĉړ��ƍl����Ƃ킩��₷���ł��傤�D
 *   �܂��Cmode=PSET �̏ꍇ��(x,y) �ɓ_��`���C�y���̈ʒu���X�V����܂��D
*/
void line( int wn,double x,double y,int mode )
{
  return gEggX.line(wn,x,y,mode);
}


/**
 * @brief      �~�̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  xcen,ycen �~�̒��S���W
 * @param[in]  xrad X�������̔��a
 * @param[in]  yrad Y�������̔��a
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�E�ɁC(xcen,ycen) �𒆐S��
 *  �������̔��axrad�C�c�����̔��ayrad �̉~��`���܂��D
*/
void circle( int wn, double xcen, double ycen, double xrad, double yrad )
{
  return gEggX.circle(wn,xcen,ycen,xrad,yrad);
}

/**
 * @brief      ���S���W�C���a��^���ĉ~��h��ׂ�
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  xcen,ycen �~�̒��S���W
 * @param[in]  xrad X�������̔��a
 * @param[in]  yrad Y�������̔��a
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�E�ɁC(xcen,ycen) �𒆐S��
 *  �������̔��axrad�C�c�����̔��ayrad �̉~��h��Ԃ��܂��D
*/
void fillcirc( int wn, double xcen, double ycen, double xrad, double yrad )
{
  return gEggX.fillcirc(wn,xcen,ycen,xrad,yrad);
}

/**
 * @brief      �~�ʂ̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  xcen,ycen �~�̒��S���W
 * @param[in]  xrad X�������̔��a
 * @param[in]  yrad Y�������̔��a
 * @param[in]  sang �`��J�n�p�x [degree]
 * @param[in]  eang �`��I���p�x [degree]
 * @param[in]  idir �`����� ( 1 ����� or -1 �E���)
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɁC(xcen,ycen) �𒆐S�ɉ������̔��axrad�C
 *  �c�����̔��ayrad �̉~�ʂ�`���܂��Dsang �͊J�n�p�Ceang �͏I���p�ŁC
 *  �x�ŗ^���܂��Didir �͉~�ʂ�`��������1 �ō����C-1 �ŉE���ƂȂ�܂��D
*/
void drawarc( int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir )
{
  return gEggX.drawarc(wn,xcen,ycen,xrad,yrad,sang,eang,idir);
}


/**
 * @brief      �~�ʂ̕`��(�h��Ԃ�)
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  xcen,ycen �~�̒��S���W
 * @param[in]  xrad X�������̔��a
 * @param[in]  yrad Y�������̔��a
 * @param[in]  sang �`��J�n�p�x [degree]
 * @param[in]  eang �`��I���p�x [degree]
 * @param[in]  idir �`����� ( 1 ����� or -1 �E���)
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ŁC(xcen,ycen) �𒆐S�ɉ������̔��axrad�C
 *  �c�����̔��ayrad �̉~�ʂ�h��ׂ��܂��Dsang �͊J�n�p�Ceang �͏I���p�ŁC
 *  �x�ŗ^���܂��Didir �͉~�ʂ�`��������1 �ō����C-1�ŉE���ƂȂ�܂��D
*/
void fillarc( int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir )
{
  return gEggX.fillarc(wn,xcen,ycen,xrad,yrad,sang,eang,idir);
}


/**
 * @brief      �����`�̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y  �`����W�i�����`�̍����j
 * @param[in]  w    �����`�̉���
 * @param[in]  h    �����`�̏c��
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɁC���_(x,y) ���琳�̕����ɕ�w�C
 *  ����h �̒����`��`���܂��D�D
*/
void drawrect( int wn, double x, double y, double w, double h )
{
  return gEggX.drawrect(wn,x,y,w,h);
}


/**
 * @brief      �����`�̕`��i�h��Ԃ��j
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y  �`����W�i�����`�̍����j
 * @param[in]  w    �����`�̉���
 * @param[in]  h    �����`�̏c��
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɁC���_(x,y) ���琳�̕����ɕ�w�C
 *  ����h �̒����`��`���܂��D�D
*/
void fillrect( int wn, double x, double y, double w, double h )
{
  return gEggX.fillrect(wn,x,y,w,h);
}


/**
 * @brief      ������̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y   �`�����W�i�������`�悷��ʒu�̍����̍��W�j
 * @param[in]  size  �`�悷�镶���̑傫��
 * @param[in]  theta ������̉�]�p�x [degree] ( * ���݂͖��� )
 * @param[in]  ...   ������
 * @return     ���ۂɕ`�悵��������̒���
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɁC����������W(x,y) ����`���܂��D
 *  size �͕����̑傫���ŁC�h�b�g�P�ʂŎw�肵�܂��D
 *  theta �͕�����̉�]���w�肷������ł����C���o�[�W�����ł͋@�\���܂���D
 *  �������argsformat �ɗ^���܂����C���̈����ȍ~��printf() �֐��̈����Ɠ��l��
 *  �t�H�[�}�b�g�ɂȂ��Ă��܂��̂ŁC�g�p��̂悤�ɕϐ��̒l�Ȃǂ�`�������ł��܂��D
 *  �����̃T�C�Ysize ��1�`24 �͈̔͂Ŏw��ł��܂��D
 *  size �Ǝ��ۂ̃t�H���g�Ƃ̊֌W�͈ȉ��̕\�̂悤�ɂȂ��Ă��܂��D
 *  ���̏ꍇ�C�����͔��p�p�����̂ݕ`��ł��܂��D2 �o�C�g����(����) 
 *  ��`�悷��ꍇ�́Csize �ɂ�FONTSET ���w�肵�܂��D
 *  ���̏ꍇ�̃t�H���g�̎w��́Cgsetfontset() �֐�(x2.5.27) �𗘗p���܂��D
 *  gsetfontset() �ł̃t�H���g�w�肪�Ȃ��ꍇ�́C�f�t�H���g��14 �h�b�g�̃t�H���g�Z�b�g�ŕ`�悳��܂��D
*/
int drawstr( int wn, double x, double y, int size, double theta,const char *argsformat, ... )
{
  assert(argsformat&&"eggx draw strings.");

  char str[256];

  va_list argptr;              //�ϐ��������X�g
  va_start(argptr,argsformat); //initalize va_ function
  int len = _vsnprintf(str,sizeof(str),argsformat,argptr);//
  va_end(argptr);              //close     va_ function

  if(len==-1)str[sizeof(str)-1]=NULL;//NULL�����t��
  else str[len]=NULL;

  if(size<=0)size = 14;

  return gEggX.drawstr(wn,x,y,size,theta,str);
}


/**
 * @brief      �`��t�H���g�̐ݒ�
 * @ingroup    wineggx
 * @param[in]  wn    �ύX����E�B���h�E�̔ԍ�
 * @param[in]  ... �t�H���g�Z�b�g�̖��O \n null�������܂߂�32Byte�ȉ��łȂ���΂Ȃ�Ȃ��D
 * @retval     -1 �G���[
 * @retval     0  �擾����
 * @retval     1  ��փt�H���g�̎擾����
*/
int gsetfontset( int wn, const char *argsformat, ... )
{
  assert(argsformat&&"eggx set fontset.");

  char str[32];

  va_list argptr;                    //�ϐ��������X�g
  va_start(argptr,argsformat);       //initalize va_ function
  int len = _vsnprintf(str,sizeof(str),argsformat,argptr);//
  va_end(argptr);                    //close     va_ function

  if(len==-1)str[sizeof(str)-1]=NULL;//NULL�����t��
  else str[len]=NULL;

  return gEggX.gsetfontset(wn,str);
}

/**
 * @brief      �L�[�{�[�h������͂��ꂽ������Ԃ�
 * @ingroup    wineggx
 * @param[in]  �Ȃ�
 * @retval     <0 ���͂Ȃ��i�m���u���b�L���O���[�h�̏ꍇ�j
 * @retval     �����ꂽ�L�[�̕����R�[�h
*/
int ggetch()
{
  return gEggX.ggetch();
}

/**
 * @brief      ggetch()�̓��샂�[�h��ݒ肷��
 * @ingroup    wineggx
 * @param[in]  flag ENABLE �m���u���b�L���O���[�h�CDISABLE �u���b�L���O���[�h
 * @retval     �Ȃ�
*/
void gsetnonblock( int flag )
{
  gEggX.gsetnonblock(flag);
}

/**
 * @brief      �~���b�P�ʂŎ��s����������
 * @ingroup    wineggx
 * @param[in]  msec ��������[msec]
 * @retval     �Ȃ�
*/
void msleep( unsigned long msec ){
	Sleep(msec);
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

  switch (uMsg) 
  {
  case WM_USER_EXIT:
      {
          int num = -1;
          for(int i=0;i<m_MaxWindow;i++)
          {
              if(m_Window.at(i).hWnd == hWnd)
              {
                  num=i;
                  break;
              }
          }
          if(num!=-1)    Destoroy(num);
          ::DestroyWindow(hWnd);
      }
      break;
  case WM_PAINT:
      {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      // TODO: �`��R�[�h�������ɒǉ����Ă�������...
      int num = -1;
      for(int i=0;i<m_MaxWindow;i++)
      {
          if(m_Window.at(i).hWnd == hWnd)
          {
              num=i;
              break;
          }
      }
      if(num==-1)return DefWindowProc(hWnd, uMsg, wParam, lParam);
      EggXWindow &wnd = m_Window.at(num);

      HDC hDCsrc  = ::CreateCompatibleDC(hdc);
      int index = wnd.index -1;
      if(index<0)index = (sizeof(wnd.hBitmap)/sizeof(HBITMAP))-1;
      ::SelectObject(hDCsrc, wnd.hBitmap[index]);

      ::BitBlt(hdc,0,0,wnd.cx,wnd.cy,hDCsrc,0,0,SRCCOPY);

      ::DeleteDC(hDCsrc);
      EndPaint(hWnd, &ps);
      }
      break;
  case WM_DESTROY:
      PostQuitMessage(0);
      break;
  case WM_CLOSE:
      ::SendMessage(hWnd,WM_USER_EXIT,0,0);
      m_Close = true;
      break;
  case WM_CHAR:
	  m_Key = wParam;
	  //cout << "WM_CHAR: m_Key = " << m_Key << endl;
	  if ( m_Nonblock == DISABLE ) {
		  SetEvent(m_KeyEvent);
	  }
	  break;
  case WM_KEYUP:
	  m_Key = -1;
	  //cout << "WM_KEYUP: m_Key = " << m_Key << endl;
	  break;
  default:
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  return 0;
}

/**
 * @brief      ���W�ϊ�
 * @ingroup    CEggX
*/
inline int CEggX::scaleX(EggXWindow &wnd, double x)
{
  return roundInt(x/((wnd.xe-wnd.xs)/wnd.cx));
}

/**
 * @brief      ���W�ϊ�
 * @ingroup    CEggX
*/
inline int CEggX::scaleY(EggXWindow &wnd, double y)
{
  return roundInt(y/((wnd.ye-wnd.ys)/wnd.cy));
}

/**
 * @brief      ���W�ϊ�
 * @ingroup    CEggX
*/
inline int CEggX::convertX(EggXWindow &wnd, double x)
{
  return scaleX(wnd, x-wnd.xs);
}

/**
 * @brief      ���W�ϊ�
 * @ingroup    CEggX
*/
inline int CEggX::convertY(EggXWindow &wnd, double y)
{
  return wnd.cy-scaleY(wnd, y-wnd.ys);
}

//==================================================================================
// CSelectWindow�̃����o�֐��̒�`
//==================================================================================

//�S�č폜����
void CSelectWindow::AllDel()
{
  m_buff.clear();
}

//�E�B���h�E�����s��ɒǉ�����
void CSelectWindow::Add(HWND hWnd,void *window)
{
  if(!hWnd || !window)return;

  WindBuffer buf;
  buf.hWnd   = hWnd;
  buf.window = window;

  m_buff.push_front(buf);
}

//�E�B���h�E�����s�񂩂�O��
void CSelectWindow::Delete(HWND hWnd)
{
  if(!hWnd||m_buff.empty())return;

  list<WindBuffer>::iterator itr = m_buff.begin();
  list<WindBuffer>::iterator end = m_buff.end();

  for(;itr!=end;itr++)
  {
      if(itr->hWnd == hWnd)
      {
          m_buff.erase(itr);
          if(m_buff.empty())return;
          itr = m_buff.begin();
          end = m_buff.end();
      }
  }
}


//�E�B���h�E�ɑΉ������|�C���^��Ԃ�
void *CSelectWindow::Serch(HWND hWnd)
{
  if(!hWnd||m_buff.empty())return NULL;

  list<WindBuffer>::iterator itr = m_buff.begin();
  list<WindBuffer>::iterator end = m_buff.end();

  for(;itr!=end;itr++)
  {
      if(itr->hWnd == hWnd)
      {
          return itr->window;
      }
  }
  return NULL;
}

//====================================================================================
