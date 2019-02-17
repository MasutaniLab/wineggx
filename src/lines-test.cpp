#include <iostream>
#include <wineggx.h>
using namespace std;

int main()
{
    int win = gopen(600, 600);
    window(win, -300, -300, 300, 300);
    gsetbgcolorrgb(win, 255, 255, 255);
    gclr(win);
    newlinewidth(win, 3);

    const int num = 8;
    double x[num] = { 0,100,100,  0,  0,100, 50,  0 };
    double y[num] = { 0,  0,100,100,  0,100,150,100 };
    newrgbcolor(win, 255, 0, 0);
    drawlines(win, x, y, num);
    float xf[num] = { 0,-100,-100,  0,  0,-100, -50,  0 };
    float yf[num] = { 0,  0,-100,-100,  0,-100,-150,-100 };
    newrgbcolor(win, 0, 0, 255);
    drawlines(win, xf, yf, num);

    const int num2 = 5;
    double x2[num2] = { 200, 100, 300, 100, 300 };
    double y2[num2] = { 300, 100, 200, 200, 100 };
    newrgbcolor(win, 255, 255, 0);
    drawpoly(win, x2, y2, num2);
    float xf2[num2] = { -200, -100, -300, -100, -300 };
    float yf2[num2] = { -300, -100, -200, -200, -100 };
    newrgbcolor(win, 0, 255, 255);
    drawpoly(win, xf2, yf2, num2);

    const int num3 = 5;
    double x3[num3] = { 200, 100, 300, 100, 300 };
    double y3[num3] = { -300, -100, -200, -200, -100 };
    newrgbcolor(win, 0, 255, 0);
    fillpoly(win, x3, y3, num3, 0);
    float xf3[num3] = { -200, -100, -300, -100, -300 };
    float yf3[num3] = { 300, 100, 200, 200, 100 };
    newrgbcolor(win, 255, 0, 255);
    fillpoly(win, xf3, yf3, num3, 1);

    const int num4 = 100;
    double x4[num4*num4];
    double y4[num4*num4];
    for (int i = 0; i < num4; i++) {
      double x = 300 * i / num4;
      for (int j = 0; j < num4; j++) {
        double y = 300 * j / num4;
        x4[num4*i + j] = x;
        y4[num4*i + j] = y;
      }
    }
    newrgbcolor(win, 0, 0, 0);
    drawpts(win, x4, y4, num4*num4);
    float xf4[num4*num4];
    float yf4[num4*num4];
    for (int i = 0; i < num4; i++) {
      float x = float(-300 * i / num4);
      for (int j = 0; j < num4; j++) {
        float y = float(-300 * j / num4);
        xf4[num4*i + j] = x;
        yf4[num4*i + j] = y;
      }
    }
    newrgbcolor(win, 0, 0, 0);
    drawpts(win, xf4, yf4, num4*num4);

    ggetch();
    gclose(win);
    return 0;
}
