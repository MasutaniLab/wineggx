#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <string>
#include <wineggx.h>
using namespace std;

//グローバル定数・変数
int win;
const int XMIN = -100;
const int XMAX = +100;
const int YMIN = -100;
const int YMAX = +100;

//関数プロトタイプ
void initializeDraw(void);
void drawGrid(int win, double xmin, double ymin, double xmax, double ymax, double grid);
void terminateDraw(void);
void draw();
void gclrGrid(int win);

int main()
{
    srand((unsigned int)time(NULL));
    initializeDraw();
    draw();
    terminateDraw();
    return 0;
}

//概要：EGGXを初期化し，座標系を描く
//引数：なし
//戻り値：なし
void initializeDraw(void)
{
    win = gopen(500, 500);
    window(win, XMIN, YMIN, XMAX, YMAX);
    gsetbgcolorrgb(win, 255, 255, 255);
    gclrGrid(win);
}

//概要：汎用の座標軸と格子を描く．
//引数：win ウィンドウの番号，
//      xmin xの最小値，ymin yの最小値，xman xの最大値，ymax yの最大値，
//      grid 格子の間隔
//戻り値：なし
void drawGrid(int win,
              double xmin, double ymin, double xmax, double ymax,
              double grid)
{
    gclr(win);
    newrgbcolor(win, 0, 0, 0);
    newlinewidth(win, 1);
    newlinestyle(win, LineOnOffDash);
    for (double x = xmin; x <= xmax; x += grid) {
        drawline(win, x, ymin, x, ymax);
    }
    for (double y = ymin; y <= ymax; y += grid) {
        drawline(win, xmin, y, xmax, y);
    }
    newlinestyle(win, LineSolid);
    drawline(win, 0, ymin, 0, ymax);
    drawline(win, xmin, 0, xmax, 0);
}

//概要：所定の座標軸と格子を描く．
//引数：win ウィンドウの番号
//戻り値：なし
void gclrGrid(int win)
{
    drawGrid(win, XMIN, YMIN, XMAX, YMAX, 10);
}

//概要：EGGXの終了
//引数：なし
//戻り値：なし
void terminateDraw(void)
{
  gsetnonblock(DISABLE);
  while (ggetch() != 27); //ESCキーで終了
  gclose(win);
}