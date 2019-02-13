#include <iostream>
#include <wineggx.h>
using namespace std;

int main()
{
    int win1 = gopen(500, 500);
    window(win1, -100, -100, 100, 100);
    gsetbgcolorrgb(win1, 255, 0, 0);
    gclr(win1);
    newrgbcolor(win1, 0, 0, 0);

    int win2 = gopen(500, 500);
    window(win2, -50, -50, 50, 50);
    gsetbgcolorrgb(win2, 0, 255, 0);
    gclr(win2);
    newrgbcolor(win2, 0, 0, 0);

    int win3 = gopen(500, 500);
    window(win3, -25, -25, 25, 25);
    gsetbgcolorrgb(win3, 0, 0, 255);
    gclr(win3);
    newrgbcolor(win3, 0, 0, 0);

    while (true) {
        cout << "windowId: " << windowId << ", mouseX: " << mouseX << ", mouseY:" << mouseY << endl;
        if (mousePressed) {
            int c = mouseButton * 80;
            newrgbcolor(windowId, c, c, c);
        } else {
            newrgbcolor(windowId, 0, 0, 0);
        }
        fillcirc(windowId, mouseX, mouseY, 1, 1);
        msleep(30);
    }

    ggetch();
    gcloseall();
    return 0;
}
