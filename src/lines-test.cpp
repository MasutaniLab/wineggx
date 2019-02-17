#include <iostream>
#include <wineggx.h>
using namespace std;

int main()
{
  int win = gopen(600, 600);
  window(win, -300, -300, 300, 300);

  for (int stage = 0; stage < 4; stage++) {
    switch (stage) {
    case 1: gresize(win, 1000, 1000); break;
    case 2: gresize(win, 800, 600); break;
    case 3:
      gresize(win, 400, 800);
      coordinate(win, 200, 400, 0, 0, 2.0 / 3.0, 4.0 / 3.0);
      break;
    }
    gsetbgcolorrgb(win, 255, 255, 255);
    gclr(win);
    newlinewidth(win, 3);
    cout << "Hit any key on eggx window." << endl;

    ggetch();
    cout << "drawlines()" << endl;
    {
      const int num = 8;
      double x[num] = { 0,100,100,  0,  0,100, 50,  0 };
      double y[num] = { 0,  0,100,100,  0,100,150,100 };
      newpen(win, 2);
      drawlines(win, x, y, num);
      float xf[num] = { 0,-100,-100,  0,  0,-100, -50,  0 };
      float yf[num] = { 0,  0,-100,-100,  0,-100,-150,-100 };
      newpen(win, 4);
      drawlines(win, xf, yf, num);
    }

    ggetch();
    cout << "drawpoly()" << endl;
    {
      const int num = 5;
      double x[num] = { 200, 100, 300, 100, 300 };
      double y[num] = { 300, 100, 200, 200, 100 };
      newpen(win, 7);
      drawpoly(win, x, y, num);
      float xf[num] = { -200, -100, -300, -100, -300 };
      float yf[num] = { -300, -100, -200, -200, -100 };
      newpen(win, 5);
      drawpoly(win, xf, yf, num);
    }

    ggetch();
    cout << "fillpoly()" << endl;
    {
      const int num = 5;
      double x[num] = { 200, 100, 300, 100, 300 };
      double y[num] = { -300, -100, -200, -200, -100 };
      newpen(win, 3);
      fillpoly(win, x, y, num, 0);
      float xf[num] = { -200, -100, -300, -100, -300 };
      float yf[num] = { 300, 100, 200, 200, 100 };
      newpen(win, 6);
      fillpoly(win, xf, yf, num, 1);
    }

    ggetch();
    cout << "drawpts()" << endl;
    {
      const int num = 100;
      double x[num*num];
      double y[num*num];
      for (int i = 0; i < num; i++) {
        double xx = 300 * i / num;
        for (int j = 0; j < num; j++) {
          double yy = 300 * j / num;
          x[num*i + j] = xx;
          y[num*i + j] = yy;
        }
      }
      newpen(win, 10);
      drawpts(win, x, y, num*num);
      float xf[num*num];
      float yf[num*num];
      for (int i = 0; i < num; i++) {
        float xx = float(-300 * i / num);
        for (int j = 0; j < num; j++) {
          float yy = float(-300 * j / num);
          xf[num*i + j] = xx;
          yf[num*i + j] = yy;
        }
      }
      newpen(win, 12);
      drawpts(win, xf, yf, num*num);
    }

    ggetch();
    cout << "tclr()" << endl;
    tclr();

    ggetch();
  }

  ggetch();
  gclose(win);
  return 0;
}
