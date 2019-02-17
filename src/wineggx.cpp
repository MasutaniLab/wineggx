/**
* @file wineggx.cpp
* @brief Windows版EGGX
* @section Notes
*  Copyright (C) 2007-2014 Team ODENS \n\n
* @date 更新日時 更新者\n
* 2019/02/XX 升谷 保博 Version 2.0\n
* 2014/10/16 升谷 保博「Windows版EGGX Version 1.0」を名乗る\n
* 2014/03/12 升谷 保博\n
*/

/**
* @addtogroup wineggx_back wineggx-WinEggXライブラリ
* @brief Windowsでeggxライブラリの代わりをする
**/

#define WINEGGX_MAIN
#include "wineggx.h"
#include "ceggx.h"

using namespace std;
bool first = true;

/**
* @brief      任意のサイズのグラフィックス画面を開く
* @ingroup    wineggx
* @param[in]  xsize ウィンドウの横幅
* @param[in]  ysize ウィンドウの縦幅
* @return     返り値にはEGGXで使用する，ウィンドウ番号が返ってきます．
* @retval     -1 エラー
*/
int gopen(int xsize, int ysize)
{
  assert(xsize&&ysize&&"eggx set window size.");
  if (first) {
    gEggX.setmouse(&windowId, &mouseX, &mouseY, &mouseButton, &mousePressed);
    first = false;
  }
  return gEggX.gopen(xsize, ysize);
}

/**
* @brief      グラフィックスウィンドウを閉じる
* @ingroup    wineggx
* @param[in]  wn ウィンドウ番号
*/
void gclose(int wn)
{
  return gEggX.gclose(wn);
}

/**
* @brief      すべてのグラフィックス用ウィンドゥを閉じる
* @ingroup    wineggx
*/
void gcloseall(void)
{
  return gEggX.gcloseall();
}

/**
* @brief      ウィンドゥのタイトルを変更する
* @ingroup    wineggx
* @param[in]  wn タイトルを変更するウィンドウの番号
* @param[in]  argsformat, ... 設定するウィンドウの文字列
* @return     設定したウィンドゥタイトルの文字列の長さ
*/
int winname(int wn, const char *argsformat, ...)
{
  assert(argsformat&&"eggx set window names.");

  va_list argptr;              //可変数引数リスト
  va_start(argptr,argsformat); //initalize va_ function
  int len = gEggX.winname(wn, argsformat, argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      【wineggx独自】ウィンドゥのタイトルを変更する（string版）
* @ingroup    wineggx
* @param[in]  wn タイトルを変更するウィンドウの番号
* @param[in]  ... 設定するウィンドウの文字列
* @return     設定したウィンドゥタイトルの文字列の長さ
*/
int winname(int wn, const std::string argsformat, ...)
{
  va_list argptr;              //可変数引数リスト
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.winname(wn, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

//未実装
//void coordinate(int wn, int xw, int yw, double xa, double ya, double xscale, double yscale)
//{
//}

/**
* @brief      座標系の変更
* @ingroup    wineggx
* @param[in]  wn 変更するウィンドウの番号
* @param[in]  xs 左下のX座標
* @param[in]  ys 左下のY座標
* @param[in]  xe 右上のX座標
* @param[in]  ye 右上のY座標
*/
void window(int wn, double xs, double ys, double xe, double ye)
{
  return gEggX.window(wn, xs, ys, xe, ye);
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
void layer(int wn, int lys, int lyw)
{
  return gEggX.layer(wn, lys, lyw);
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
void copylayer(int wn, int lysrc, int lydest)
{
  return gEggX.copylayer(wn, lysrc, lydest);
}

/**
* @brief      ウィンドゥの背景色の変更
* @ingroup    wineggx
* @param[in]  wn 変更するウィンドウの番号
* @param[in]  argsformat, ... 色を表す文字列
*  wn で指定されたウィンドゥの背景色を変更します．argsformat(とそれに続く引数)で指定される文字列を
*  背景色に設定します．2つめの引数argsformat以降は，C 標準関数のprintf() 関数の場合と同様の可変引数
*  となっています．この背景色の文字列には，Xウィンドウで定義されている色か，"#c0c0ff"のように，16進数
*  のRed,Green,Blue を指定します．
*/
void gsetbgcolor(int wn, const char *argsformat, ...)
{
  assert(argsformat&&"eggx set background color.");

  va_list argptr;              //可変数引数リスト
  va_start(argptr, argsformat); //initalize va_ function
  gEggX.gsetbgcolor(wn, argsformat, argptr);//
  va_end(argptr);              //close     va_ function
}

/**
* @brief      【wineggx独自】ウィンドゥの背景色の変更（string版）
* @ingroup    wineggx
* @param[in]  wn 変更するウィンドウの番号
* @param[in]  argsformat, ... 色を表す文字列
*  wn で指定されたウィンドゥの背景色を変更します．argsformat(とそれに続く引数)で指定される文字列を
*  背景色に設定します．2つめの引数argsformat以降は，C 標準関数のprintf() 関数の場合と同様の可変引数
*  となっています．この背景色の文字列には，Xウィンドウで定義されている色か，"#c0c0ff"のように，16進数
*  のRed,Green,Blue を指定します．
*/
void gsetbgcolor(int wn, const std::string argsformat, ...)
{
  va_list argptr;              //可変数引数リスト
  va_start(argptr, argsformat); //initalize va_ function
  gEggX.gsetbgcolor(wn, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function
}

/**
* @brief      【wineggx独自】ウィンドゥの背景色の変更
* @ingroup    wineggx
* @param[in]  wn 変更するウィンドウの番号
* @param[in]  r  赤色 ( 0 to 255 )
* @param[in]  g  緑色 ( 0 to 255 )
* @param[in]  b  青色 ( 0 to 255 )
* @section Notes
*  wn で指定されたウィンドゥの背景色を変更します．r,g,b にはそれぞれRed,Green,Blue の輝度を
*  256 段階の整数(0～255) で指定します．
*/
void gsetbgcolorrgb(int wn, int r, int g, int b)
{
  return gEggX.gsetbgcolorrgb(wn, r, g, b);
}

/**
* @brief      描画レイヤの全消去
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @section Notes
*  wn で指定したウィンドゥの描画レイヤを背景色に指定された色で初期化します．
*/
void gclr(int wn)
{
  return gEggX.gclr(wn);
}

//未実装
//void tclr(void)
//{
//}

//未実装
//void newpen(int wn, int cn)
//{
//}

/**
* @brief      描画色の変更
* @ingroup    wineggx
* @param[in]  wn 変更するウィンドウの番号
* @param[in]  argsformat, ... 色を表す文字列
*  wn で指定されたウィンドゥでの描画色を変更します．argsformat(とそれに続く引数)で指定される文字列を
*  背景色に設定します．2つめの引数argsformat以降は，C 標準関数のprintf() 関数の場合と同様の可変引数
*  となっています．この背景色の文字列には，Xウィンドウで定義されている色か，"#c0c0ff"のように，16進数
*  のRed,Green,Blue を指定します．
*/
void newcolor( int wn, const char *argsformat, ... )
{
  assert(argsformat&&"eggx set new color.");

  va_list argptr;              //可変数引数リスト
  va_start(argptr, argsformat); //initalize va_ function
  gEggX.newcolor(wn, argsformat, argptr);//
  va_end(argptr);              //close     va_ function
}

/**
* @brief      【wineggx独自】描画色の変更（string版）
* @ingroup    wineggx
* @param[in]  wn 変更するウィンドウの番号
* @param[in]  argsformat, ... 色を表す文字列
*  wn で指定されたウィンドゥでの描画色を変更します．argsformat(とそれに続く引数)で指定される文字列を
*  背景色に設定します．2つめの引数argsformat以降は，C 標準関数のprintf() 関数の場合と同様の可変引数
*  となっています．この背景色の文字列には，Xウィンドウで定義されている色か，"#c0c0ff"のように，16進数
*  のRed,Green,Blue を指定します．
*/
void newcolor(int wn, const std::string argsformat, ...)
{
  va_list argptr;              //可変数引数リスト
  va_start(argptr, argsformat); //initalize va_ function
  gEggX.newcolor(wn, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function
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
void newrgbcolor(int wn, int r, int g, int b)
{
  return gEggX.newrgbcolor(wn, r, g, b);
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
void newhsvcolor(int wn, int h, int s, int v)
{
  return gEggX.newhsvcolor(wn, h, s, v);
}

//未実装
//int makecolor(int cmode, double dmin, double dmax, double data, int *r, int *g, int *b)
//{
//}

/**
* @brief      線幅の変更
* @ingroup    wineggx
* @param[in]  wn 変更するウィンドウの番号
* @param[in]  width 線幅
* @section Notes
*  wn で指定したウィンドゥで線を描く時の線幅を変更します．
*  デフォルトでは幅1 が設定されています．
*/
void newlinewidth(int wn, int width)
{
  return gEggX.newlinewidth(wn, width);
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
void newlinestyle(int wn, int style)
{
  return gEggX.newlinestyle(wn, style);
}

//未実装
//void newgcfunction(int wn, int fnc)
//{
//}

/**
* @brief      点の描画
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x,y 描画先座標
* @section Notes
*  wn で指定したウィンドゥに点を描画します．
*/
void pset(int wn, double x, double y)
{
  return gEggX.pset(wn, x, y);
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
void drawline(int wn, double x0, double y0, double x1, double y1)
{
  return gEggX.drawline(wn, x0, y0, x1, y1);
}

/**
* @brief      直線を描くための初期位置の設定
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x,y 描画先座標
* @section Notes
*  wn で指定したウィンドゥに，lineto() 関数のための初期位置を(x, y) に設定します．
*/
void moveto(int wn, double x, double y)
{
  return gEggX.moveto(wn, x, y);
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
void lineto(int wn, double x, double y)
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
void line(int wn, double x, double y, int mode)
{
  return gEggX.line(wn,x,y,mode);
}

/**
* @brief      複数の点を描く（double版）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x[] 各点のx座標
* @param[in]  y[] 各点のy座標
* @param[in]  n 点の数
* @section Notes
*  n個の点を描く．
*  x，yはn個の実数の一次元配列で，x[0]～x[n-1]，y[0]～y[n-1]に各点の座標を入れておく．
*/
void drawpts(int wn, const double x[], const double y[], int n)
{
  gEggX.drawpts(wn, x, y, n);

}

/**
* @brief      複数の点を描く（float版）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x[] 各点のx座標
* @param[in]  y[] 各点のy座標
* @param[in]  n 点の数
* @section Notes
*  n個の点を描く．
*  x，yはn個の実数の一次元配列で，x[0]～x[n-1]，y[0]～y[n-1]に各点の座標を入れておく．
*/
void drawpts(int wn, const float x[], const float y[], int n)
{
  double *xd = new double[n];
  double *yd = new double[n];
  for (int i = 0; i < n; i++) {
    xd[i] = x[i];
    yd[i] = y[i];
  }
  gEggX.drawpts(wn, xd, yd, n);
  delete xd;
  delete yd;
}

/**
* @brief      折れ線を描く（double版）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x[] 折れ線の各点のx座標
* @param[in]  y[] 折れ線の各点のy座標
* @param[in]  n 点の数
* @section Notes
*  wnで指定したウィンドゥで，折れ線を描く．
*  x，yはn個の実数の一次元配列で，x[0]～x[n-1]，y[0]～y[n-1]に折れ線の
*  各点の座標を入れておく．
*/
void drawlines(int wn, const double x[], const double y[], int n)
{
  gEggX.drawlines(wn, x, y, n);
}

/**
* @brief      折れ線を描く（float版）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x[] 折れ線の各点のx座標
* @param[in]  y[] 折れ線の各点のy座標
* @param[in]  n 点の数
* @section Notes
*  wnで指定したウィンドゥで，折れ線を描く．
*  x，yはn個の実数の一次元配列で，x[0]～x[n-1]，y[0]～y[n-1]に折れ線の
*  各点の座標を入れておく．
*/
void drawlines(int wn, const float x[], const float y[], int n)
{
  double *xd = new double[n];
  double *yd = new double[n];
  for (int i = 0; i < n; i++) {
    xd[i] = x[i];
    yd[i] = y[i];
  }
  gEggX.drawlines(wn, xd, yd, n);
  delete xd;
  delete yd;
}

/**
* @brief      多角形を描く（double版）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x[] 多角形の各点のx座標
* @param[in]  y[] 多角形の各点のy座標
* @param[in]  n 点の数
* @section Notes
*  wnで指定したウィンドゥで，多角形を描く．
*  x，yはn個の実数の一次元配列で，x[0]～x[n-1]，y[0]～y[n-1]に折れ線の
*  各点の座標を入れておく．
*/
void drawpoly(int wn, const double x[], const double y[], int n)
{
  gEggX.drawpoly(wn, x, y, n);
}

/**
* @brief      多角形を描く（float版）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x[] 多角形の各点のx座標
* @param[in]  y[] 多角形の各点のy座標
* @param[in]  n 点の数
* @section Notes
*  wnで指定したウィンドゥで，多角形を描く．
*  x，yはn個の実数の一次元配列で，x[0]～x[n-1]，y[0]～y[n-1]に折れ線の
*  各点の座標を入れておく．
*/
void drawpoly(int wn, const float x[], const float y[], int n)
{
  double *xd = new double[n];
  double *yd = new double[n];
  for (int i = 0; i < n; i++) {
    xd[i] = x[i];
    yd[i] = y[i];
  }
  gEggX.drawpoly(wn, xd, yd, n);
  delete xd;
  delete yd;
}

/**
* @brief      多角形を塗り潰す（double版）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x[] 多角形の各点のx座標
* @param[in]  y[] 多角形の各点のy座標
* @param[in]  n 点の数
* @param[in]  i 塗り潰す時の形状
* @section Notes
*  wnで指定したウィンドゥで，多角形の領域を塗り潰す．
*  x，yはn個の実数の一次元配列で，x[0]～x[n-1]，y[0]～y[n-1]に折れ線の
*  各点の座標を入れておく．
*  iは塗り潰す時の形状で通常は0を，凸多角形の時は1を指定する．
*/
void fillpoly(int wn, const double x[], const double y[], int n, int i)
{
  gEggX.fillpoly(wn, x, y, n, i);
}

/**
* @brief      多角形を塗り潰す（float版）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x[] 多角形の各点のx座標
* @param[in]  y[] 多角形の各点のy座標
* @param[in]  n 点の数
* @param[in]  i 塗り潰す時の形状
* @section Notes
*  wnで指定したウィンドゥで，多角形の領域を塗り潰す．
*  x，yはn個の実数の一次元配列で，x[0]～x[n-1]，y[0]～y[n-1]に折れ線の
*  各点の座標を入れておく．
*  iは塗り潰す時の形状で通常は0を，凸多角形の時は1を指定する．
*/
void fillpoly(int wn, const float x[], const float y[], int n, int i)
{
  double *xd = new double[n];
  double *yd = new double[n];
  for (int i = 0; i < n; i++) {
    xd[i] = x[i];
    yd[i] = y[i];
  }
  gEggX.fillpoly(wn, xd, yd, n, i);
  delete xd;
  delete yd;
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
void drawrect(int wn, double x, double y, double w, double h)
{
  return gEggX.drawrect(wn, x, y, w, h);
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
void fillrect(int wn, double x, double y, double w, double h)
{
  return gEggX.fillrect(wn, x, y, w, h);
}

/**
* @brief      円の描画（circle()と全く同じ）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  xcen,ycen 円の中心座標
* @param[in]  xrad X軸方向の半径
* @param[in]  yrad Y軸方向の半径
* @section Notes
*  wn で指定したウィンドウに，(xcen,ycen) を中心に
*  横方向の半径xrad，縦方向の半径yrad の円を描きます．
*/
void drawcirc(int wn, double xcen, double ycen, double xrad, double yrad)
{
  return gEggX.circle(wn, xcen, ycen, xrad, yrad);
}

/**
* @brief      円の描画（drawcirc()と全く同じ）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  xcen,ycen 円の中心座標
* @param[in]  xrad X軸方向の半径
* @param[in]  yrad Y軸方向の半径
* @section Notes
*  wn で指定したウィンドウに，(xcen,ycen) を中心に
*  横方向の半径xrad，縦方向の半径yrad の円を描きます．
*/
void circle(int wn, double xcen, double ycen, double xrad, double yrad)
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
void fillcirc(int wn, double xcen, double ycen, double xrad, double yrad)
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
void drawarc(int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir)
{
  return gEggX.drawarc(wn, xcen, ycen, xrad, yrad, sang, eang, idir);
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
void fillarc(int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir)
{
  return gEggX.fillarc(wn, xcen, ycen, xrad, yrad, sang, eang, idir);
}

//未実装
//void drawsym(int wn, int x, int y, int size, int symtype)
//{
//}

//未実装
//void drawsyms(int wn, const double x[], const double y[], int n, int size, int symtype)
//{
//}

//未実装
//void drawarrow(int wn, double xs, double ys, double xt, double yt, double s, double w, int 10 * i + j)
//{
//}

/**
* @brief      描画フォントの設定（gsetfontset()と全く同じ）
* @ingroup    wineggx
* @param[in]  wn    変更するウィンドウの番号
* @param[in]  argsformat, ... フォントセットの名前 \n ヌル文字を含めて32Byte以下でなければならない．
* @retval     -1 エラー
* @retval     0  取得成功
* @retval     1  代替フォントの取得成功
*/
int newfontset(int wn, const char *argsformat, ...)
{
  assert(argsformat&&"eggx set fontset.");

  va_list argptr;              //可変数引数リスト
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.gsetfontset(wn, argsformat, argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      【wineggx独自】描画フォントの設定（gsetfontset()と全く同じ）（string版）
* @ingroup    wineggx
* @param[in]  wn    変更するウィンドウの番号
* @param[in]  argsformat, ... フォントセットの名前 \n ヌル文字を含めて32Byte以下でなければならない．
* @retval     -1 エラー
* @retval     0  取得成功
* @retval     1  代替フォントの取得成功
*/
int newfontset(int wn, const std::string argsformat, ...)
{
  va_list argptr;              //可変数引数リスト
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.gsetfontset(wn, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      描画フォントの設定（newfontset()と全く同じ）
* @ingroup    wineggx
* @param[in]  wn    変更するウィンドウの番号
* @param[in]  argsformat, ... フォントセットの名前 \n ヌル文字を含めて32Byte以下でなければならない．
* @retval     -1 エラー
* @retval     0  取得成功
* @retval     1  代替フォントの取得成功
*/
int gsetfontset(int wn, const char *argsformat, ...)
{
  assert(argsformat&&"eggx set fontset.");

  va_list argptr;              //可変数引数リスト
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.gsetfontset(wn, argsformat, argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      【wineggx独自】描画フォントの設定（newfontset()と全く同じ）（string版）
* @ingroup    wineggx
* @param[in]  wn    変更するウィンドウの番号
* @param[in]  argsformat, ... フォントセットの名前 \n ヌル文字を含めて32Byte以下でなければならない．
* @retval     -1 エラー
* @retval     0  取得成功
* @retval     1  代替フォントの取得成功
*/
int gsetfontset(int wn, const std::string argsformat, ...)
{
  va_list argptr;              //可変数引数リスト
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.gsetfontset(wn, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      文字列の描画
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x,y   描画先座標（文字列を描画する位置の左下の座標）
* @param[in]  size  描画する文字の大きさ
* @param[in]  theta 文字列の回転角度 [degree] ( * 現在は無効 )
* @param[in]  argsformat, ... 文字列
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
*  この場合のフォントの指定は，gsetfontset() 関数を利用します．
*  gsetfontset() でのフォント指定がない場合は，デフォルトの14 ドットのフォントセットで描画されます．
*/
int drawstr( int wn, double x, double y, int size, double theta,const char *argsformat, ... )
{
  assert(argsformat&&"eggx draw strings.");

  va_list argptr;              //可変数引数リスト
  va_start(argptr,argsformat); //initalize va_ function
  int len = gEggX.drawstr(wn, x, y, size, theta, argsformat, argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      【wineggx独自】文字列の描画（string版）
* @ingroup    wineggx
* @param[in]  wn 描画するウィンドウの番号
* @param[in]  x,y   描画先座標（文字列を描画する位置の左下の座標）
* @param[in]  size  描画する文字の大きさ
* @param[in]  theta 文字列の回転角度 [degree] ( * 現在は無効 )
* @param[in]  argsformat, ... 文字列
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
*  この場合のフォントの指定は，gsetfontset() 関数を利用します．
*  gsetfontset() でのフォント指定がない場合は，デフォルトの14 ドットのフォントセットで描画されます．
*/
int drawstr(int wn, double x, double y, int size, double theta, const std::string argsformat, ...)
{
  va_list argptr;              //可変数引数リスト
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.drawstr(wn, x, y, size, theta, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

//未実装
//void gscroll(int wn, int incx, int incy, int clr)
//{
//}

//未実装
//void gputarea(int wn, double x, double y, int srcwn, int srcly, double srcxs, double srcys, double srcxe, double srcye)
//{
//}

//未実装
//int gputimage(int wn, double x, double y, unsigned char *buf, int width, int height, int msk)
//{
//}

//未実装
//unsigned char *ggetimage(int wn, int ly, double xs, double ys, double xe, double ye, int *rwidth, int *rheight)
//{
//}

//未実装
//int gsaveimage(int wn, int ly, double xs, double ys, double xe, double ye, const char *conv, int nd, const char *argsformat, ...)
//{
//}

//未実装
//unsigned char *readimage(const char *conv, const char *filename, int *rwidth, int *rheight, int *rmsk)
//{
//}

//未実装
//int writeimage(const unsigned char *buf, int width, int height, int msk, const char *conv, int nd, const char *argsformat, ...)
//{
//}

/**
* @brief      ggetch()，ggetevent()，ggetxpress()の動作モードを設定する
* @ingroup    wineggx
* @param[in]  flag ENABLE ノンブロッキングモード，DISABLE ブロッキングモード
* @retval     なし
*/
void gsetnonblock(int flag)
{
  gEggX.gsetnonblock(flag);
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
* @brief      全てのウィンドウのマウスやキーボードの入力情報を返す（double版）．
* @ingroup    wineggx
* @param[out]  type イベントの種類
* @param[out]  button ボタンの番号（マウスの場合），キーコード（キーボードの場合）
* @param[out]  x マウスポインタのx座標（アプリケーション座標系）
* @param[out]  y マウスポインタのy座標（アプリケーション座標系）
* @retval      入力のあったウィンドウ番号
*/
int ggetevent(int *type, int *button, double *x, double *y)
{
  return gEggX.ggetevent(type, button, x, y);
}

/**
* @brief      全てのウィンドウのマウスやキーボードの入力情報を返す（float版）．
* @ingroup    wineggx
* @param[out]  type イベントの種類
* @param[out]  button ボタンの番号（マウスの場合），キーコード（キーボードの場合）
* @param[out]  x マウスポインタのx座標（アプリケーション座標系）
* @param[out]  y マウスポインタのy座標（アプリケーション座標系）
* @retval      入力のあったウィンドウ番号
*/
int ggetevent(int *type, int *button, float *x, float *y)
{
  double xd, yd;
  int ret = gEggX.ggetevent(type, button, &xd, &yd);
  *x = float(xd);
  *y = float(yd);
  return ret;
}

/**
* @brief      全てのウィンドウのマウスやキーボードの入力情報を返す（キーとボタンのみ）（double版）．
* @ingroup    wineggx
* @param[out]  type イベントの種類
* @param[out]  button ボタンの番号（マウスの場合），キーコード（キーボードの場合）
* @param[out]  x マウスポインタのx座標（アプリケーション座標系）
* @param[out]  y マウスポインタのy座標（アプリケーション座標系）
* @retval      入力のあったウィンドウ番号
*/
int ggetxpress(int *type, int *button, double *x, double *y)
{
  return gEggX.ggetxpress(type, button, x, y);
}

/**
* @brief      全てのウィンドウのマウスやキーボードの入力情報を返す（キーとボタンのみ）（float版）．
* @ingroup    wineggx
* @param[out]  type イベントの種類
* @param[out]  button ボタンの番号（マウスの場合），キーコード（キーボードの場合）
* @param[outr]  x マウスポインタのx座標（アプリケーション座標系）
* @param[out]  y マウスポインタのy座標（アプリケーション座標系）
* @retval      入力のあったウィンドウ番号
*/
int ggetxpress(int *type, int *button, float *x, float *y)
{
  double xd, yd;
  int ret = gEggX.ggetxpress(type, button, &xd, &yd);
  *x = float(xd);
  *y = float(yd);
  return ret;
}

/**
* @brief      ミリ秒単位で実行を延期する
* @ingroup    wineggx
* @param[in]  msec 延期時間[msec]
* @retval     なし
*/
void msleep(unsigned long msec)
{
  Sleep(msec);
}
