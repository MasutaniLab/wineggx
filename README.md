# Windows版EGGX
Version 2.0 （マウスイベントに対応，関数追加）  
2019年2月XX日  
升谷 保博  
（大阪電気通信大学）  

## はじめに

これは，Windows用のC（C++)言語用の2次元グラフィックスライブラリです．広
く使われているX11グラフィックスライブラリ「EGGX/ProCALL」

http://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html

の仕様（の一部）と互換になるように作られています．EGGX/ProCALLの作者で
ある山内氏（JAXA）から「Windows版EGGX」と名乗ることの許可を得ています．
なお，仕様は互換になるようにしていますが，オリジナルのソースコードは利用
していません．

このライブラリの特徴は，EGGX互換であることと，WinAPIを全く意識すること
なく，main()関数で始まるCやC++のプログラムで使うことができることです．

## インストール

Visual Studio 2015で動作確認しています．

特別なライブラリは必要ありません．CMakeでソリューションを生成し，
Visual Studioでソリューションをビルドすると，静的なライブラリと動作確認用の
実行ファイルが作られます．
さらに，プロジェクトINSTALLをビルドすると，変数`INSTALL_PREFIX`に設定された
ディレクトリの下の`lib`ディレクトリにライブラリファイルが，
`include`ディレクトリにヘッダファイルが，コピーされます．
変数`INSTALL_PREFIX`のデフォルト値は，この場所
（`README.md`の置かれているディレクトリ）です．

## 利用例のプロジェクト

- example1
  - EGGXを利用する最低限のコードです．
- example2
  - ボールアニメーション．
- texample1
  - C++言語の初学者のために，色々な約束事を意識せずにグラフィックスのプログラムを作らせる
  試みです．`include\t-pro1.h` を`#include`して，`draw()`関数の定義を描きます．
  Processingを意識していますが，draw()が繰り返し実行されることはありません．
- texample2
  - `t-pro1.h`を使ったボールアニメーション．

## 動作確認のプロジェクト

- ggetch-test
  - ggetch()の動作確認．
- mouse-test
  - ggetevent()の動作確認．
- mouse-test2
  - ggetxpress()の動作確認．
- mouse-test3
  - 大域変数windowId, mouseX, mouseY, mousePressed, mouseButtonの動作確認．
- string-test
  - `char *`の代わりに`std::string`を使う関数の動作確認．
- ver2-test
  - Version 2.0で追加した関数の動作確認．

## 使い方

プログラムの最初に`#include <wineggx.h>`を追加してください．関数の使
い方は，EGGXのC版と全く同じです．

Visual Studioの場合，プロジェクトのプロパティに3ヶ所に設定が必要です．

- C/C++の「追加のインクルードディレクトリ」に
  `（インストールディレクトリ）\include`を追加してください．

- リンカ―の「追加のライブラリディレクトリ」に
  `（インストールディレクトリ）\lib`を追加してください．

- リンカ―の入力の「追加の依存ファイル」の並びに以下を追加してください．
  - 構成がDebugの場合は，`wineggxd.lib`
  - 構成がReleaseの場合は，`wineggx.lib`

## オリジナルとの違い

- `#include <eggx.h>`とする代わりに`#include <wineggx.h>`としてください．

- `#include <eggxlib.h>`を使う`eggx_`で始まる関数には対応していません．

- ウィンドウの番号は1からではなく，0からです． 

- 実装されているのは以下の関数だけです．

~~~
int gopen(int xsize,int ysize);
void gclose(int wn);
void gcloseall(void);
void gresize(int wn, int xsize, int ysize);
int winname(int wn, const char *argsformat, ...);
int winname(int wn, const std::string argsformat, ...); //wineggx独自
void coordinate(int wn, int xw, int yw, double xa, double ya, double xscale, double yscale);
void window(int wn, double xs, double ys, double xe, double ye);
void layer(int wn, int lys, int lyw);
void copylayer(int wn, int lysrc, int lydest);
void gsetbgcolor(int wn, const char *argsformat, ...);
void gsetbgcolor(int wn, const std::string argsformat, ...); //wineggx独自
void gsetbgcolorrgb(int wn, int r, int g, int b); //wineggx独自
void gclr(int wn);
void tclr(void);
void newpen(int wn, int cn);
void newcolor(int wn, const char *argsformat, ...);
void newcolor(int wn, const std::string argsformat, ...); //wineggx独自
void newrgbcolor(int wn, int r, int g, int b);
void newhsvcolor(int wn, int h, int s, int v);
void newlinewidth( int wn, int width );
void newlinestyle( int wn, int style );
void pset(int wn, double x, double y);
void drawline(int wn, double x0, double y0, double x1, double y1);
void moveto(int wn, double x, double y);
void lineto(int wn, double x, double y);
void line(int wn, double x, double y, int mode);
void drawpts(int wn, const double x[], const double y[], int n);
void drawpts(int wn, const float x[], const float y[], int n);
void drawlines(int wn, const double x[], const double y[], int n);
void drawlines(int wn, const float x[], const float y[], int n);
void drawpoly(int wn, const double x[], const double y[], int n);
void drawpoly(int wn, const float x[], const float y[], int n);
void fillpoly(int wn, const double x[], const double y[], int n, int i);
void fillpoly(int wn, const float x[], const float y[], int n, int i);
void drawrect(int wn, double x, double y, double w, double h);
void fillrect(int wn, double x, double y, double w, double h);
void drawcirc(int wn, double xcen, double ycen, double xrad, double yrad);
void circle(int wn, double xcen, double ycen, double xrad, double yrad);
void fillcirc(int wn, double xcen, double ycen, double xrad, double yrad);
void drawarc(int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir);
void fillarc(int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir);
int newfontset(int wn, const char *argsformat, ...);
int newfontset(int wn, const std::string argsformat, ...);
int gsetfontset(int wn, const char *argsformat, ...);
int gsetfontset(int wn, const std::string argsformat, ...);
int drawstr(int wn, double x, double y, int size, double theta, const char *argsformat, ...);
int drawstr(int wn, double x, double y, int size, double theta, const std::string argsformat, ...); //wineggx独自
void gsetnonblock(int flag);
int ggetch();
int ggetevent(int *type, int *button, double *x, double *y);
int ggetevent(int *type, int *button, float *x, float *y);
int ggetevent(int &type, int &button, double &x, double &y); //wineggx独自
int ggetevent(int &type, int &button, float &x, float &y); //wineggx独自
int ggetxpress(int *type, int *button, double *x, double *y);
int ggetxpress(int *type, int *button, float *x, float *y);
int ggetxpress(int &type, int &button, double &x, double &y); //wineggx独自
int ggetxpress(int &type, int &button, float &x, float &y); //wineggx独自
void msleep(unsigned long msec);
~~~

## 履歴

- 2007年ごろに大阪電気通信大学升谷研究室において，当時大学院生の片岡 賢太郎氏が，EGGXを使って書かれていたLinuxのプログラムをWindowsに移植するために，EGGXに似た関数群を作成しました．それを升谷が引き継ぎ，コンソールプログラムで使えるように改良し，研究室の内部で利用していました．

- 2014年度から大阪電気通信大学 総合情報学部 情報学科の「C++プログラミング実習2」の教材として採用することになり，山内氏から「Windows版EGGX」と名乗ることの許可を得ました（Version 1.0）．

- 2017年度から同学科の「C++プログラミング実習1」の教材としても採用することになり，これを機会にGitHubで公開することにしました（Version 1.1）．

- 開発環境をVisual Studio 2012から2015へ移行しました（Version 1.1b）．

- CMakeに対応しました（Version 1.1c）．

- Version 2.0
  - ggetch()で文字以外のキーにも対応するようにしました（カーソルキー等）．
  - マウスイベントに対応しました（`ggetevent()，ggetxpress()`）．
  - 関数を追加しました．`gresize(), coordinate(), tclr(), newpen(), drawpts(), drawlines(), drawpoly(), fillpoly(), newfontset()`
  - Processing風に大域変数でマウスの状態を取得できるようにしました（wineggx独自）．
  - Cスタイルの文字列（`char *`）の代わりに`std::string`も使えるようにしました（wineggx独自）．

---
## イベント処理に関する覚書

### ブロック
```
gsetnonblock(DISABLE);
win = ggetevent(&type, &button, &x, &y);
```

|動作|win|type|button|x|y|
|:--|:--|:--|:--|:--|:--|
|マウス移動                 |ウィンドウ番号|MotionNotify (6)|ボタン番号|x座標|y座標|
|マウスがウィンドウへ入る    |入ったウィンドウ番号|EnterNotify (7)|ボタン番号|x座標|y座標|
|マウスがウィンドウから出る  |出る前のウィンドウ番号|LeaveNotify (8)|ボタン番号|x座標|y座標|
|マウスのボタンを押す        |ウィンドウ番号|ButtonPress (4)|ボタン番号|x座標|y座標|
|キーボードのキーを押す      |ウィンドウ番号|KeyPress (2)|文字コード|0|0|

### ノンブロック
```
gsetnonblock(ENABLE);
win = ggetevent(&type, &button, &x, &y);
```

|動作|win|type|button|x|y|
|:--|:--|:--|:--|:--|:--|
|イベントなし|-1|不定|不定|不定|不定|
|マウス移動                 |ウィンドウ番号|MotionNotify (6)|ボタン番号|x座標|y座標|
|マウスがウィンドウへ入る    |入ったウィンドウ番号|EnterNotify (7)|ボタン番号|x座標|y座標|
|マウスがウィンドウから出る  |出る前のウィンドウ番号|LeaveNotify (8)|ボタン番号|x座標|y座標|
|マウスのボタンを押す        |ウィンドウ番号|ButtonPress (4)|ボタン番号|x座標|y座標|
|キーボードのキーを押す      |ウィンドウ番号|KeyPress (2)|文字コード|0|0|

### 大域変数（wineggx独自）

|動作|windowId|mousePressed|mouseButton|mouseX|mouseY|
|:--|:--|:--|:--|:--|:--|
|マウスがウィンドウ外                    |-1|false|0|0|0|
|マウスがウィンドウ内でボタン押していない  |ウィンドウ番号|false|0|x座標|y座標|
|マウスがウィンドウ内でボタン押している    |ウィンドウ番号|true|ボタン番号|x座標|y座標|

### ボタン番号

1. 左ボタン
2. 中ボタン
3. 右ボタン
4. ホイールを前へ回転
5. ホイールを後へ回転
