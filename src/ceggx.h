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
  /**
  * @struct  MyFont
  * @brief   �t�H���g�Ǘ��̂��߂̍\����
  * @ingroup wineggx_back
  * @section Notes
  *  hFont    �t�H���g�I�u�W�F�N�g
  *  fontName �t�H���g��
  */
  struct MyFont {
    HFONT   hFont;
    char    fontName[32];
  };
  HANDLE    m_Thread; // �X���b�h�n���h��
  HANDLE    m_Event;  // �C�x���g�n���h��
  HANDLE    m_Ack;
  HANDLE    m_Com;
  HINSTANCE m_hInst;
  int       m_NumWind;
  int       m_MaxWindow;
  std::vector<EggXWindow> m_Window;
  std::map<int,MyFont>    m_Font;
  std::list<LONGLONG>     m_MSG;
  int       m_createWindowNum;

  bool      m_run;//�X���b�h����t���O
  DWORD     m_SleepTime;
  LONGLONG  m_accesstable;
  bool      m_Init;
  bool      m_Close;
  bool      m_Nonblock;
  int       m_Key;
  HANDLE    m_KeyEvent;

private:
  class rgb {
  public:
    int r;
    int g;
    int b;
    rgb() { r = 0; g = 0; b = 0; }
    rgb(int ar, int ag, int ab) { r = ar; g = ag; b = ab; }
  };

  std::map<std::string, rgb> rgbtable;

  const static int WINDOW_CREATE = 1;

};

#ifdef CEGGX_MAIN
#define EXTERN
#else
#define EXTERN extern
#endif

// �O���[�o���ϐ�
EXTERN CEggX gEggX;
