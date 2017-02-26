#include <t-pro1.h>

void draw()
{
    double x = 0;
    double y = 0;
    double vx = 10;
    double vy = 5;
    double r = 10;
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
        gclrGrid(win);
        newrgbcolor(win, 255, 0, 0);
        fillcirc(win, x, y, r, r);
        copylayer(win, 1, 0);
        msleep(30);
    }
}