/**
 * @file wineggx.cpp
 * @brief Windows版EGGX
 * @section Notes
 *  Copyright (C) 2007-2014 Team ODENS \n\n
 * @date 更新日時 更新者\n
 * 2014/10/16 升谷 保博「Windows版EGGX Version 1.0」を名乗る\n
 * 2014/03/12 升谷 保博\n
*/

/**
 * @addtogroup wineggx_back wineggx-WinEggXライブラリ
 * @brief Windowsでeggxライブラリの代わりをする
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
// CSelectWindowクラス
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
  WINDOW_CREATE = 1, //ウィンドウ作成フラグ
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
 * @brief   ウィンドウを管理するための構造体
 * @ingroup wineggx_back
 * @section Notes
 *  hWnd    ウィンドウハンドル\n
 *  hBitmap ビットマップオブジェクトハンドル\n
 *  hPen    ペンオブジェクトハンドル\n
 *  hBrush  ブラシオブジェクトハンドル\n
 *  hFont   フォントオブジェクトハンドル\n
 *  writeLayer 描画先レイヤー\n
 *  showLayer  表示レイヤー\n
 *  xs,ys      ウィンドウ左下の座標
 *  xe,ye      ウィンドウ右上の座標
 *  r,g,b      描画色
*/
struct EggXWindow{
  HWND    hWnd;
  HBITMAP hBitmap[8];//ウィンドゥ毎に8 枚のレイヤ
  HPEN    hPen;
  HPEN    hPenF; //塗りつぶし図形用のペン
  HBRUSH  hBrush;
  HBRUSH  hBrushB; //背景用のブラシ
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
 * @brief   フォント管理のための構造体
 * @ingroup wineggx_back
 * @section Notes
 *  hFont    フォントオブジェクト
 *  fontName フォント名
*/
struct MyFont{
  HFONT   hFont;
  char    fontName[32];
};


/**
 * @ingroup wineggx_back
 * @class CEggX
 * @brief Windows版EGGX
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
  // EGGXメッセージ処理
  void EggMessage(LONGLONG msg);

  // スレッドエントリーポイント
  static DWORD WINAPI ThreadEntry(void *param);
  DWORD ThreadProc();

  // ウィンドウメッセージ処理
  static  INT_PTR CALLBACK WindowProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
  virtual INT_PTR MsgProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);

  //座標変換
  int scaleX(EggXWindow &wnd, double x);
  int scaleY(EggXWindow &wnd, double y);
  int convertX(EggXWindow &wnd, double x);
  int convertY(EggXWindow &wnd, double y);
  //四捨五入 double → int
  int roundInt(double d)
  {
    return int(d+0.5-(d<0));
  }

protected:
  HANDLE    m_Thread; // スレッドハンドル
  HANDLE    m_Event;  // イベントハンドル
  HANDLE    m_Ack;
  HANDLE    m_Com;
  HINSTANCE m_hInst;
  int       m_NumWind;
  int       m_MaxWindow;
  vector<EggXWindow> m_Window;
  map<int,MyFont>    m_Font;
  list<LONGLONG>     m_MSG;
  int       m_createWindowNum;

  bool      m_run;//スレッド動作フラグ
  DWORD     m_SleepTime;
  LONGLONG  m_accesstable;
  bool      m_Init;
  bool      m_Close;
  bool      m_Nonblock;
  int       m_Key;
  HANDLE    m_KeyEvent;
};


// グローバル変数
CEggX gEggX;

/**
 * @brief      コンストラクタ
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
 * @brief      デストラクタ
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
 * @brief      Windowsでeggxと同じことをするための準備
 * @ingroup    CEggX
 * @param[in]  hInst インスタンスハンドル
 * @param[in]  AccessTableId アクセステーブルのID番号
 * @retval     1  すでに初期化完了
 * @retval     -1 エラー
 * @retval     0  成功
*/
int CEggX::Initialize(HINSTANCE hInst,LONGLONG AccessTableId)
{
  if(m_run)return 1;

  //ウィンドウクラス登録
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

  //スレッドを作成する
  return CreateThread();
}


/**
 * @brief      スレッドを作成する
 * @ingroup    CEggX
 * @retval     1  すでに作成済み
 * @retval     -1 エラー
 * @retval     0  成功
*/
int  CEggX::CreateThread()
{
  if( m_run || m_Thread ) return 1;

  // スレッド作成
  m_Thread = ::CreateThread(NULL,0,ThreadEntry,(DWORD*)this,0,NULL);
  if(!m_Thread)
  {
      return -1;
  }
  return 0;
}


/**
 * @brief      全てのウィンドウを破壊する
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
 * @brief      全てのウィンドウを破壊する
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
 * @brief      指定のウィンドウを破壊する
 * @ingroup    CEggX
 * @param[in]  wn ウィンドウ番号
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
 * @brief      ウィンドウを作成する
 * @ingroup    CEggX
 * @param[in]  xsize ウィンドウの横幅
 * @param[in]  ysize ウィンドウの縦幅
 * @retval     -1 エラー
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

  //ウィンドウスタイル
  DWORD Style   = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
  DWORD StyleEx = WS_EX_WINDOWEDGE;

  //ウィンドウ名
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
  sprintf(wnd.fontName,"ＭＳ ゴシック");
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

  //ウィンドウを最前面に表示する
  BringWindowToTop(FindWindow(EGGX_WINDOW_CLASS_NAME,windowname));

  //ウィンドウを常に最前面に表示する
  SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);

  m_NumWind++;

  m_SleepTime = 20;

  return num;
}


/**
 * @brief      任意のサイズのグラフィックス画面を開く
 * @ingroup    CEggX
 * @param[in]  xsize ウィンドウの横幅
 * @param[in]  ysize ウィンドウの縦幅
 * @return     返り値にはEGGXで使用する，ウィンドウ番号が返ってきます．
 * @retval     -1 エラー
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
 * @brief      グラフィックスウィンドウを閉じる
 * @ingroup    CEggX
 * @param[in]  wn ウィンドウ番号
*/
void CEggX::gclose( unsigned wn )
{
  if(wn>=(unsigned)m_MaxWindow)return;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return;

  ::PostMessage(wnd.hWnd,WM_USER_EXIT,0,0);
}


/**
 * @brief      すべてのグラフィックス用ウィンドゥを閉じる
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
 * @brief      ウィンドゥのタイトルを変更する
 * @ingroup    CEggX
 * @param[in]  wn タイトルを変更するウィンドウの番号
 * @param[in]  ... 設定するウィンドウの文字列
 * @return     設定したウィンドゥタイトルの文字列の長さ
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
 * @brief      座標系の変更
 * @ingroup    CEggX
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  xs 左下のX座標
 * @param[in]  ys 左下のY座標
 * @param[in]  xe 右上のX座標
 * @param[in]  ye 右上のY座標
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
 * @brief      レイヤの設定をする
 * @ingroup    CEggX
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  lys 表示レイヤー( 0 or 1 )
 * @param[in]  lyw 書き込み先レイヤー( 0 or 1 )
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
 * @brief      レイヤのコピーをする
 * @ingroup    CEggX
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  lysrc  コピー元レイヤー番号
 * @param[in]  lydest コピー先レイヤー番号
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
 * @brief      描画色の変更
 * @ingroup    CEggX
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  str 色を表す文字列
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
 * @brief      描画色の変更
 * @ingroup    CEggX
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  r  赤色 ( 0 to 255 )
 * @param[in]  g  緑色 ( 0 to 255 )
 * @param[in]  b  青色 ( 0 to 255 )
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
 * @brief      描画色の変更
 * @ingroup    CEggX
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  h  色相 ( 0 to 359 )
 * @param[in]  s  彩度 ( 0 to 255 )
 * @param[in]  v  明度 ( 0 to 255 )
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
 * @brief      ウィンドゥの背景色の変更
 * @ingroup    CEggX
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  str 色を表す文字列
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
 * @brief      ウィンドゥの背景色の変更
 * @ingroup    CEggX
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  r  赤色 ( 0 to 255 )
 * @param[in]  g  緑色 ( 0 to 255 )
 * @param[in]  b  青色 ( 0 to 255 )
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
 * @brief      描画レイヤの全消去
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
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
 * @brief      線幅の変更
 * @ingroup    CEggX
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  width 線幅 
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
 * @brief      線のスタイルの変更
 * @ingroup    CEggX
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  style 線のスタイル（LineSolid：実線，LineOnOffDash：点線） 
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
 * @brief      点の描画
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y 描画先座標
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
 * @brief      直線の描画
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x0,y0 端点の座標
 * @param[in]  x1,y1 端点の座標
*/
void CEggX::drawline( unsigned wn, double x0, double y0, double x1, double y1 )
{
	moveto(wn, x0, y0);
	lineto(wn, x1, y1);
}

/**
 * @brief      直線を描ための初期位置の設定
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y 描画先座標
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
 * @brief      連続的に直線を描く
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y 座標
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
 * @brief      直線の描画
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y 描画先座標
 * @param[in]  mode 描画モード (@ref PENDOWN @ref PENUP @ref PSET)
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
 * @brief      円の描画
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  xcen,ycen 円の中心座標
 * @param[in]  xrad X軸方向の半径
 * @param[in]  yrad Y軸方向の半径
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
 * @brief      中心座標，半径を与えて円を塗り潰す
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  xcen,ycen 円の中心座標
 * @param[in]  xrad X軸方向の半径
 * @param[in]  yrad Y軸方向の半径
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
 * @brief      円弧の描画
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  xcen,ycen 円の中心座標
 * @param[in]  xrad X軸方向の半径
 * @param[in]  yrad Y軸方向の半径
 * @param[in]  sang 描画開始角度 [degree]
 * @param[in]  eang 描画終了角度 [degree]
 * @param[in]  idir 描画方向 ( 1 左回り or -1 右回り)
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
 * @brief      円弧の描画(塗りつぶす)
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  xcen,ycen 円の中心座標
 * @param[in]  xrad X軸方向の半径
 * @param[in]  yrad Y軸方向の半径
 * @param[in]  sang 描画開始角度 [degree]
 * @param[in]  eang 描画終了角度 [degree]
 * @param[in]  idir 描画方向 ( 1 左回り or -1 右回り)
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
 * @brief      長方形の描画
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y  描画座標（長方形の左下）
 * @param[in]  w    長方形の横幅
 * @param[in]  h    長方形の縦幅
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
 * @brief      長方形の描画（塗りつぶす）
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y  描画座標（長方形の左下）
 * @param[in]  w    長方形の横幅
 * @param[in]  h    長方形の縦幅
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
 * @brief      文字列の描画
 * @ingroup    CEggX
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y   描画先座標（文字列を描画する位置の左下の座標）
 * @param[in]  size  描画する文字の大きさ
 * @param[in]  theta 文字列の回転角度 [degree] ( * 現在は無効 )
 * @param[in]  ...   文字列
 * @return     実際に描画した文字列の長さ
*/
int  CEggX::drawstr( unsigned wn, double x, double y, int size, double theta,const char *str )
{
  if(wn>=(unsigned)m_MaxWindow)return 0;

  EggXWindow &wnd = m_Window.at(wn);
  if(!wnd.hWnd)return 0;

  //フォント作成
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
      ::SetTextColor(hWDC,RGB(wnd.r,wnd.g,wnd.b));//文字色設定（NULL=Black）
      ::SetBkMode(hWDC,TRANSPARENT);              //文字背景を塗りつぶさない

      if(::GetTextExtentPoint32(hWDC,str,len,&psize))
      { //テキストサイズ分だけずらす
          rect.top-=psize.cy;
      }
      ::DrawText(hWDC,str,len,&rect,DT_TOP|DT_LEFT);
  }
  else
  {
      HDC hDC  = ::CreateCompatibleDC(hWDC);
      ::SelectObject(hDC,wnd.hBitmap[wnd.index]);
      ::SelectObject(hDC,*wnd.hFont);
      ::SetTextColor(hDC,RGB(wnd.r,wnd.g,wnd.b));//文字色設定（NULL=Black）
      ::SetBkMode(hDC,TRANSPARENT);              //文字背景を塗りつぶさない

      if(::GetTextExtentPoint32(hDC,str,len,&psize))
      { //テキストサイズ分だけずらす
          rect.top-=psize.cy;
      }

      ::DrawText(hDC,str,len,&rect,DT_TOP|DT_LEFT);

      ::DeleteDC(hDC);
  }
  ::ReleaseDC(wnd.hWnd,hWDC);
  return len;
}


/**
 * @brief      描画フォントの設定
 * @ingroup    CEggX
 * @param[in]  wn    変更するウィンドウの番号
 * @param[in]  ... フォントセットの名前
 * @retval     -1 エラー
 * @retval     0  取得成功
 * @retval     1  代替フォントの取得成功
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
 * @brief      キーボードから入力された文字を返す
 * @ingroup    CEggX
 * @param[in]  なし
 * @retval     <0 入力なし（ノンブロッキングモードの場合）
 * @retval     押されたキーの文字コード
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
 * @brief      ggetch()の動作モードを設定する
 * @ingroup    CEggX
 * @param[in]  ENABLE ノンブロッキングモードにする
 * @param[in]  DISABLE ブロッキングモードにする
 * @retval     なし
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
 * @brief      任意のサイズのグラフィックス画面を開く
 * @ingroup    wineggx
 * @param[in]  xsize ウィンドウの横幅
 * @param[in]  ysize ウィンドウの縦幅
 * @return     返り値にはEGGXで使用する，ウィンドウ番号が返ってきます．
 * @retval     -1 エラー
*/
int gopen(int xsize,int ysize)
{
  assert(xsize&&ysize&&"eggx set window size.");

  return gEggX.gopen(xsize,ysize);
}


/**
 * @brief      グラフィックスウィンドウを閉じる
 * @ingroup    wineggx
 * @param[in]  wn ウィンドウ番号
*/
void gclose( int wn )
{
  return gEggX.gclose(wn);
}


/**
 * @brief      すべてのグラフィックス用ウィンドゥを閉じる
 * @ingroup    wineggx
*/
void gcloseall( void )
{
  return gEggX.gcloseall();
}


/**
 * @brief      ウィンドゥのタイトルを変更する
 * @ingroup    wineggx
 * @param[in]  wn タイトルを変更するウィンドウの番号
 * @param[in]  ... 設定するウィンドウの文字列
 * @return     設定したウィンドゥタイトルの文字列の長さ
*/
int winname( int wn, const char *argsformat, ... )
{
  assert(argsformat&&"eggx set window names.");

  char str[256];

  va_list argptr;              //可変数引数リスト
  va_start(argptr,argsformat); //initalize va_ function
  int len = _vsnprintf(str,sizeof(str),argsformat,argptr);//
  va_end(argptr);              //close     va_ function

  if(len==-1)str[sizeof(str)-1]=NULL;//NULL文字付加
  else str[len]=NULL;

  return gEggX.winname(wn,str);
}


/**
 * @brief      座標系の変更
 * @ingroup    wineggx
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  xs 左下のX座標
 * @param[in]  ys 左下のY座標
 * @param[in]  xe 右上のX座標
 * @param[in]  ye 右上のY座標
*/
void window( int wn, double xs, double ys, double xe, double ye )
{
  return gEggX.window(wn,xs,ys,xe,ye);
}


/**
 * @brief      レイヤの設定をする
 * @ingroup    wineggx
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  lys 表示レイヤー( 0 or 1 )
 * @param[in]  lyw 書き込み先レイヤー( 0 or 1 )
 * @section Notes
 *  現在表示しているレイヤに対して(lys == lyw の場合に) 連続して描画関数を実行すると，良い描画パ
 *  フォーマンスが得られません．高速な描画が必要な場合には，現在表示していないレイヤに対して描画し，
 *  最後に表示レイヤをlayer() 関数で切り替えるか，copylayer() 関数(x2.5.10) で描画レイヤの画像を表
 *  示レイヤにコピーするようにします．\n
 *  デフォルトではlayer(wn,0,0) の状態となっています．
*/
void layer( int wn, int lys, int lyw )
{
  return gEggX.layer(wn,lys,lyw);
}


/**
 * @brief      レイヤのコピーをする
 * @ingroup    wineggx
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  lysrc  コピー元レイヤー番号
 * @param[in]  lydest コピー先レイヤー番号
 * @section Notes
 *  wn のウィンドウ番号の，レイヤlysrc の画像をレイヤlydest にそのままコピーします．
 *  このコピーは瞬時に行われるため，アニメーションの再生に使うことができます．
*/
void copylayer( int wn, int lysrc, int lydest )
{
   return gEggX.copylayer(wn,lysrc,lydest);
}

/**
 * @brief      描画色の変更
 * @ingroup    wineggx
 * @param[in]  wn 変更するウィンドウの番号
 *  wn で指定されたウィンドゥでの描画色を変更します．argsformat(とそれに続く引数)で指定される文字列を
 *  背景色に設定します．2つめの引数argsformat以降は，C 標準関数のprintf() 関数の場合と同様の可変引数
 *  となっています．この背景色の文字列には，Xウィンドウで定義されている色か，"#c0c0ff"のように，16進数
 *  のRed,Green,Blue を指定します．
*/
void newcolor( int wn, const char *argsformat, ... ) {
  char str[256];

  va_list argptr;              //可変数引数リスト
  va_start(argptr,argsformat); //initalize va_ function
  int len = _vsnprintf(str,sizeof(str),argsformat,argptr);//
  va_end(argptr);              //close     va_ function

  if(len==-1)str[sizeof(str)-1]=NULL;//NULL文字付加
  else str[len]=NULL;
  return gEggX.newcolor(wn, str);
}

/**
 * @brief      描画色の変更
 * @ingroup    wineggx
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  r  赤色 ( 0 to 255 )
 * @param[in]  g  緑色 ( 0 to 255 )
 * @param[in]  b  青色 ( 0 to 255 )
 * @section Notes
 *  wn で指定したウィンドゥでの描画色を変更します．r,g,b にはそれぞれRed,Green,Blue の輝度を
 *  256 段階の整数(0～255) で指定します．
*/
void newrgbcolor( int wn, int r, int g, int b )
{
  return gEggX.newrgbcolor(wn,r,g,b);
}
/**
 * @brief      描画色の変更
 * @ingroup    wineggx
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  h  色相 ( 0 to 359 )
 * @param[in]  s  彩度 ( 0 to 255 )
 * @param[in]  v  明度 ( 0 to 255 )
 * @section Notes
 *  wn で指定したウィンドゥでの描画色を変更します．h,s,v にはそれぞれ，Hue,Satulation,Valueを
 *  指定します)．sとvは256段階の整数(0～255)を，hは0～359までの整数(角度)を指定します．
*/
void newhsvcolor( int wn, int h, int s, int v )
{
  return gEggX.newhsvcolor(wn,h,s,v);
}

/**
 * @brief      ウィンドゥの背景色の変更
 * @ingroup    wineggx
 * @param[in]  wn 変更するウィンドウの番号
 *  wn で指定されたウィンドゥの背景色を変更します．argsformat(とそれに続く引数)で指定される文字列を
 *  背景色に設定します．2つめの引数argsformat以降は，C 標準関数のprintf() 関数の場合と同様の可変引数
 *  となっています．この背景色の文字列には，Xウィンドウで定義されている色か，"#c0c0ff"のように，16進数
 *  のRed,Green,Blue を指定します．
*/
void gsetbgcolor( int wn, const char *argsformat, ... )
{
  char str[256];

  va_list argptr;              //可変数引数リスト
  va_start(argptr,argsformat); //initalize va_ function
  int len = _vsnprintf(str,sizeof(str),argsformat,argptr);//
  va_end(argptr);              //close     va_ function

  if(len==-1)str[sizeof(str)-1]=NULL;//NULL文字付加
  else str[len]=NULL;
  return gEggX.gsetbgcolor(wn,str);
}

/**
 * @brief      ウィンドゥの背景色の変更【注意：オリジナルにはない関数】
 * @ingroup    wineggx
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  r  赤色 ( 0 to 255 )
 * @param[in]  g  緑色 ( 0 to 255 )
 * @param[in]  b  青色 ( 0 to 255 )
 * @section Notes
 *  wn で指定されたウィンドゥの背景色を変更します．r,g,b にはそれぞれRed,Green,Blue の輝度を
 *  256 段階の整数(0～255) で指定します．
*/
void gsetbgcolorrgb( int wn, int r, int g, int b )
{
  return gEggX.gsetbgcolorrgb(wn,r,g,b);
}

/**
 * @brief      描画レイヤの全消去
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @section Notes
 *  wn で指定したウィンドゥの描画レイヤを背景色に指定された色で初期化します．
*/
void gclr( int wn )
{
  return gEggX.gclr(wn);
}

/**
 * @brief      線幅の変更
 * @ingroup    wineggx
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  width 線幅
 * @section Notes
 *  wn で指定したウィンドゥで線を描く時の線幅を変更します．
 *  デフォルトでは幅1 が設定されています．
*/
void newlinewidth( int wn, int width )
{
  return gEggX.newlinewidth(wn,width);
}

/**
 * @brief      線のスタイルの変更
 * @ingroup    wineggx
 * @param[in]  wn 変更するウィンドウの番号
 * @param[in]  style 線のスタイル
 * @section Notes
 *  wn で指定したウィンドゥで線を描く時のスタイルを変更します．
 *  引数style に与える事ができる値は，LineSolid(実線) とLineOnOffDash(点線) です．
 *  デフォルトでは実線(LineSolid) が設定されています．
 *  Windows APIの仕様上，LineSolidは線幅1以下でないと有効でない．
*/
void newlinestyle( int wn, int style )
{
  return gEggX.newlinestyle(wn,style);
}

/**
 * @brief      点の描画
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y 描画先座標
 * @section Notes
 *  wn で指定したウィンドゥに点を描画します．
*/
void pset( int wn,double x,double y )
{
  return gEggX.pset(wn,x,y);
}

/**
 * @brief      直線の描画
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x0,y0 端点の座標
 * @param[in]  x1,y1 端点の座標
 * @section Notes
 *  wn で指定したウィンドゥの(x0, y0) から(x1, y1) に直線を描画します．
*/
void drawline( int wn, double x0, double y0, double x1, double y1 )
{
  return gEggX.drawline(wn,x0,y0,x1,y1);
}

/**
 * @brief      直線を描ための初期位置の設定
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y 描画先座標
 * @section Notes
 *  wn で指定したウィンドゥに，lineto() 関数のための初期位置を(x, y) に設定します．
*/
void moveto( int wn, double x, double y )
{
  return gEggX.moveto(wn,x,y);
}

/**
 * @brief      連続的に直線を描く
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y 座標
 * @section Notes
 *  wn で指定したウィンドゥに，以前moveto()またはlineto() が呼ばれた時に指定された座標から
 *  (x, y) へ直線を引きます．
*/
void lineto( int wn, double x, double y )
{
  return gEggX.lineto(wn,x,y);
}

/**
 * @brief      直線の描画
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y 描画先座標
 * @param[in]  mode 描画モード
 * @section Notes
 *  wn で指定したウィンドゥに直線を描画します．
 *  mode に @ref PENDOWN を指定すると以前line() 関数が呼ばれた点から，
 *  (x,y) へ直線を引きます．mode にPENUP を指定すると(x,y) を
 *  line() 関数の初期位置に設定します．mode=PENDOWN でペンを下ろして
 *  描画，mode=PENUP でペンを上げて移動と考えるとわかりやすいでしょう．
 *   また，mode=PSET の場合は(x,y) に点を描き，ペンの位置が更新されます．
*/
void line( int wn,double x,double y,int mode )
{
  return gEggX.line(wn,x,y,mode);
}


/**
 * @brief      円の描画
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  xcen,ycen 円の中心座標
 * @param[in]  xrad X軸方向の半径
 * @param[in]  yrad Y軸方向の半径
 * @section Notes
 *  wn で指定したウィンドウに，(xcen,ycen) を中心に
 *  横方向の半径xrad，縦方向の半径yrad の円を描きます．
*/
void circle( int wn, double xcen, double ycen, double xrad, double yrad )
{
  return gEggX.circle(wn,xcen,ycen,xrad,yrad);
}

/**
 * @brief      中心座標，半径を与えて円を塗り潰す
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  xcen,ycen 円の中心座標
 * @param[in]  xrad X軸方向の半径
 * @param[in]  yrad Y軸方向の半径
 * @section Notes
 *  wn で指定したウィンドウに，(xcen,ycen) を中心に
 *  横方向の半径xrad，縦方向の半径yrad の円を塗りつぶします．
*/
void fillcirc( int wn, double xcen, double ycen, double xrad, double yrad )
{
  return gEggX.fillcirc(wn,xcen,ycen,xrad,yrad);
}

/**
 * @brief      円弧の描画
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  xcen,ycen 円の中心座標
 * @param[in]  xrad X軸方向の半径
 * @param[in]  yrad Y軸方向の半径
 * @param[in]  sang 描画開始角度 [degree]
 * @param[in]  eang 描画終了角度 [degree]
 * @param[in]  idir 描画方向 ( 1 左回り or -1 右回り)
 * @section Notes
 *  wn で指定したウィンドゥに，(xcen,ycen) を中心に横方向の半径xrad，
 *  縦方向の半径yrad の円弧を描きます．sang は開始角，eang は終了角で，
 *  度で与えます．idir は円弧を描く方向で1 で左廻り，-1 で右廻りとなります．
*/
void drawarc( int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir )
{
  return gEggX.drawarc(wn,xcen,ycen,xrad,yrad,sang,eang,idir);
}


/**
 * @brief      円弧の描画(塗りつぶす)
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  xcen,ycen 円の中心座標
 * @param[in]  xrad X軸方向の半径
 * @param[in]  yrad Y軸方向の半径
 * @param[in]  sang 描画開始角度 [degree]
 * @param[in]  eang 描画終了角度 [degree]
 * @param[in]  idir 描画方向 ( 1 左回り or -1 右回り)
 * @section Notes
 *  wn で指定したウィンドゥで，(xcen,ycen) を中心に横方向の半径xrad，
 *  縦方向の半径yrad の円弧を塗り潰します．sang は開始角，eang は終了角で，
 *  度で与えます．idir は円弧を描く方向で1 で左廻り，-1で右廻りとなります．
*/
void fillarc( int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir )
{
  return gEggX.fillarc(wn,xcen,ycen,xrad,yrad,sang,eang,idir);
}


/**
 * @brief      長方形の描画
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y  描画座標（長方形の左下）
 * @param[in]  w    長方形の横幅
 * @param[in]  h    長方形の縦幅
 * @section Notes
 *  wn で指定したウィンドゥに，頂点(x,y) から正の方向に幅w，
 *  高さh の長方形を描きます．．
*/
void drawrect( int wn, double x, double y, double w, double h )
{
  return gEggX.drawrect(wn,x,y,w,h);
}


/**
 * @brief      長方形の描画（塗りつぶす）
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y  描画座標（長方形の左下）
 * @param[in]  w    長方形の横幅
 * @param[in]  h    長方形の縦幅
 * @section Notes
 *  wn で指定したウィンドゥに，頂点(x,y) から正の方向に幅w，
 *  高さh の長方形を描きます．．
*/
void fillrect( int wn, double x, double y, double w, double h )
{
  return gEggX.fillrect(wn,x,y,w,h);
}


/**
 * @brief      文字列の描画
 * @ingroup    wineggx
 * @param[in]  wn 描画するウィンドウの番号
 * @param[in]  x,y   描画先座標（文字列を描画する位置の左下の座標）
 * @param[in]  size  描画する文字の大きさ
 * @param[in]  theta 文字列の回転角度 [degree] ( * 現在は無効 )
 * @param[in]  ...   文字列
 * @return     実際に描画した文字列の長さ
 * @section Notes
 *  wn で指定したウィンドゥに，文字列を座標(x,y) から描きます．
 *  size は文字の大きさで，ドット単位で指定します．
 *  theta は文字列の回転を指定する引数ですが，現バージョンでは機能しません．
 *  文字列はargsformat に与えますが，この引数以降はprintf() 関数の引数と同様の
 *  フォーマットになっていますので，使用例のように変数の値などを描く事もできます．
 *  文字のサイズsize は1～24 の範囲で指定できます．
 *  size と実際のフォントとの関係は以下の表のようになっています．
 *  この場合，文字は半角英数字のみ描画できます．2 バイト文字(漢字) 
 *  を描画する場合は，size にはFONTSET を指定します．
 *  この場合のフォントの指定は，gsetfontset() 関数(x2.5.27) を利用します．
 *  gsetfontset() でのフォント指定がない場合は，デフォルトの14 ドットのフォントセットで描画されます．
*/
int drawstr( int wn, double x, double y, int size, double theta,const char *argsformat, ... )
{
  assert(argsformat&&"eggx draw strings.");

  char str[256];

  va_list argptr;              //可変数引数リスト
  va_start(argptr,argsformat); //initalize va_ function
  int len = _vsnprintf(str,sizeof(str),argsformat,argptr);//
  va_end(argptr);              //close     va_ function

  if(len==-1)str[sizeof(str)-1]=NULL;//NULL文字付加
  else str[len]=NULL;

  if(size<=0)size = 14;

  return gEggX.drawstr(wn,x,y,size,theta,str);
}


/**
 * @brief      描画フォントの設定
 * @ingroup    wineggx
 * @param[in]  wn    変更するウィンドウの番号
 * @param[in]  ... フォントセットの名前 \n null文字を含めて32Byte以下でなければならない．
 * @retval     -1 エラー
 * @retval     0  取得成功
 * @retval     1  代替フォントの取得成功
*/
int gsetfontset( int wn, const char *argsformat, ... )
{
  assert(argsformat&&"eggx set fontset.");

  char str[32];

  va_list argptr;                    //可変数引数リスト
  va_start(argptr,argsformat);       //initalize va_ function
  int len = _vsnprintf(str,sizeof(str),argsformat,argptr);//
  va_end(argptr);                    //close     va_ function

  if(len==-1)str[sizeof(str)-1]=NULL;//NULL文字付加
  else str[len]=NULL;

  return gEggX.gsetfontset(wn,str);
}

/**
 * @brief      キーボードから入力された文字を返す
 * @ingroup    wineggx
 * @param[in]  なし
 * @retval     <0 入力なし（ノンブロッキングモードの場合）
 * @retval     押されたキーの文字コード
*/
int ggetch()
{
  return gEggX.ggetch();
}

/**
 * @brief      ggetch()の動作モードを設定する
 * @ingroup    wineggx
 * @param[in]  flag ENABLE ノンブロッキングモード，DISABLE ブロッキングモード
 * @retval     なし
*/
void gsetnonblock( int flag )
{
  gEggX.gsetnonblock(flag);
}

/**
 * @brief      ミリ秒単位で実行を延期する
 * @ingroup    wineggx
 * @param[in]  msec 延期時間[msec]
 * @retval     なし
*/
void msleep( unsigned long msec ){
	Sleep(msec);
}

/**
 * @brief      EggXのメッセージ処理
 * @ingroup    CEggX
 * @param[in]  msg メッセージ
 * @return     スレッド終了パラメータ
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
 * @brief      スレッドのエントリーポイント
 * @ingroup    CEggX
 * @param[in]  param クラスのポインタ
 * @return     スレッド終了パラメータ
*/
DWORD CEggX::ThreadEntry(void *param)
{
  assert(param&&"eggx thread entry point.");
  return ((CEggX*)param)->ThreadProc();
}


/**
 * @brief      スレッドの処理
 * @ingroup    CEggX
 * @return     スレッド終了パラメータ
*/
DWORD CEggX::ThreadProc()
{
  m_run = true;
  MSG msg;
  //ショートカットキー処理
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
          // メイン メッセージ ループ :
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
 * @brief      ウィンドウメッセージを受け取る関数
 * @ingroup    CEggX
*/
INT_PTR CEggX::WindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  static CSelectWindow s_SelectWind;

  CEggX *This=(CEggX*)s_SelectWind.Serch(hWnd);
  if(uMsg==WM_CREATE)
  {//ウィンドウ初期化
      CREATESTRUCT *cre = (CREATESTRUCT*)(((CREATESTRUCT*)lParam)->lpCreateParams);

      s_SelectWind.Add(hWnd,(void*)cre->lpCreateParams);
      This = (CEggX*)cre->lpCreateParams;
      return 0;
  }
  else if(uMsg==WM_DESTROY)
  {//ウィンドウが破壊されたとき
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
 * @brief      ウィンドウメッセージ処理
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
      // TODO: 描画コードをここに追加してください...
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
 * @brief      座標変換
 * @ingroup    CEggX
*/
inline int CEggX::scaleX(EggXWindow &wnd, double x)
{
  return roundInt(x/((wnd.xe-wnd.xs)/wnd.cx));
}

/**
 * @brief      座標変換
 * @ingroup    CEggX
*/
inline int CEggX::scaleY(EggXWindow &wnd, double y)
{
  return roundInt(y/((wnd.ye-wnd.ys)/wnd.cy));
}

/**
 * @brief      座標変換
 * @ingroup    CEggX
*/
inline int CEggX::convertX(EggXWindow &wnd, double x)
{
  return scaleX(wnd, x-wnd.xs);
}

/**
 * @brief      座標変換
 * @ingroup    CEggX
*/
inline int CEggX::convertY(EggXWindow &wnd, double y)
{
  return wnd.cy-scaleY(wnd, y-wnd.ys);
}

//==================================================================================
// CSelectWindowのメンバ関数の定義
//==================================================================================

//全て削除する
void CSelectWindow::AllDel()
{
  m_buff.clear();
}

//ウィンドウ検索行列に追加する
void CSelectWindow::Add(HWND hWnd,void *window)
{
  if(!hWnd || !window)return;

  WindBuffer buf;
  buf.hWnd   = hWnd;
  buf.window = window;

  m_buff.push_front(buf);
}

//ウィンドウ検索行列から外す
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


//ウィンドウに対応したポインタを返す
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
