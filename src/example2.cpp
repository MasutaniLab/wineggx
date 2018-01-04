#include <iostream>
#include <wineggx.h>
using namespace std;

int main()
{
    const int XMIN = -100;
    const int XMAX = +100;
    const int YMIN = -100;
    const int YMAX = +100;
    double x = 0;
    double y = 0;
    double vx = 10;
    double vy = 5;
    double r = 10;
    int win = gopen(500, 500);
    window(win, XMIN, YMIN, XMAX, YMAX);
    layer(win, 0, 1);
    gsetbgcolorrgb(win, 224, 255, 224);
    for (;;) {
        x += vx;
        if (x < XMIN+r || XMAX-r < x) {
            vx *= -1;
            cout << "x”½“]" << endl;
        }
        y += vy;
        if (y < YMIN+r || YMAX-r <y) {
            vy *= -1;
            cout << "y”½“]" << endl;
        }
        gclr(win);
        newrgbcolor(win, 255, 0, 0);
        fillcirc(win, x, y, r, r);
        copylayer(win, 1, 0);
        msleep(30);
    }
    return 0;
}
