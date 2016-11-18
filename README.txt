Windows版EGGX
Version 1.0
2014年10月16日
升谷 保博
（大阪電気通信大学）


■はじめに

これは，Windows用のC（C++)言語用の2次元グラフィックスライブラリです．広
く使われているX11グラフィックスライブラリ「EGGX/ProCALL」

  http://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html

の仕様（の一部）と互換になるように作られています．EGGX/ProCALLの作者で
ある山内氏（JAXA）から「Windows版EGGX」と名乗ることの許可を得ています．

このライブラリの特徴は，EGGX互換であることと，WinAPIを全く意識すること
なく，main()関数で始まるCやC++のプログラムで使うことができることです．


■使い方

Visual Studioの場合，プロジェクトのプロパティに3ヶ所に設定が必要です．

・C/C++の「追加のインクルードディレクトリ」に
  「（wineggxのディレクトリ）\include」を追加してください．

・リンカ―の「追加のライブラリディレクトリ」に
  「（wineggxのディレクトリ）\lib」を追加してください．

・リンカ―の入力の「追加の依存ファイル」に「wineggx.lib」を追加してください．


■プログラムの作り方

プログラムの最初に「#include <wineggx.h>」を追加してください．関数の使
い方は，EGGXのC版と全く同じです．


■オリジナルとの違い

「#include <eggx.h>」とする代わりに「#include <wineggx.h>」としてください．

「#include <eggxlib.h>」を使う「eggx_」で始まる関数には対応していません．

実装されているのは以下の関数だけです．

------------------------------------------------------------------------
int gopen(int xsize,int ysize);
void gclose( int wn );
void gcloseall( void );
int winname( int wn, const char *argsformat, ... );
void window( int wn, double xs, double ys, double xe, double ye );
void layer( int wn, int lys, int lyw );
void copylayer( int wn, int lysrc, int lydest );
void newcolor( int wn, const char *argsformat, ... );
void newrgbcolor( int wn, int r, int g, int b );
void newhsvcolor( int wn, int h, int s, int v );
void gsetbgcolor( int wn, const char *argsformat, ... );
void gsetbgcolorrgb( int wn, int r, int g, int b );
void gclr( int wn );
void newlinewidth( int wn, int width );
void newlinestyle( int wn, int style );
void pset( int wn,double x,double y );
void drawline( int wn, double x0, double y0, double x1, double y1 );
void moveto( int wn, double x, double y );
void lineto( int wn, double x, double y );
void line( int wn,double x,double y,int mode );
void circle( int wn, double xcen, double ycen, double xrad, double yrad );
void fillcirc( int wn, double xcen, double ycen, double xrad, double yrad );
void drawarc( int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir );
void fillarc( int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir );
void drawrect( int wn, double x, double y, double w, double h );
void fillrect( int wn, double x, double y, double w, double h );
int drawstr( int wn, double x, double y, int size, double theta,const char *argsformat, ... );
int gsetfontset( int wn, const char *argsformat, ... );
int ggetch();
void gsetnonblock( int flag );
void msleep( unsigned long msec );
------------------------------------------------------------------------

以上．
