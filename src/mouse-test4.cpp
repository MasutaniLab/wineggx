//マウスカーソル範囲外の座標の図形描画
#include <iostream>
#include <wineggx.h>
using namespace std;

int main()
{
  int win = gopen(500, 500);
  window(win, -100, -100, 100, 100);
  gsetbgcolor(win, "white");
  newrgbcolor(win, 0, 0, 0);
  layer(win, 0, 1);

  while (true) {
    gclr(win);
    double x = mouseX;
    double y = mouseY;
    cout << x << ", " << y << endl;

    drawline(win, 0, 0, x, y);
    drawcirc(win, x, y, 20, 20);
    fillcirc(win, x, y, 15, 15);
    drawrect(win, x, y, 20, 20);
    fillrect(win, x, y, 15, 15);
    drawstr(win, x, y, 100, 0, "ABC");

    const int num = 30;
    double xp[num * num];
    double yp[num * num];
    for (int i = 0; i < num; i++) {
        double xx = x + 50.0 * i / num;
        for (int j = 0; j < num; j++) {
            double yy = y + 50.0 * j / num;
            xp[num * i + j] = xx;
            yp[num * i + j] = yy;
        }
    }
    drawpts(win, xp, yp, num * num);

    const int numg = 3;
    double xg[numg];
    double yg[numg];
    xg[0] = x; yg[0] = y;
    xg[1] = x+30; yg[1] = y+30;
    xg[2] = x+30; yg[2] = y-30;
    drawpoly(win, xg, yg, numg);

    copylayer(win, 1, 0);
    msleep(10);
  }
  return 0;
}
