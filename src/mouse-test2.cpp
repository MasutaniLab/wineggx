#include <iostream>
#include <wineggx.h>
using namespace std;

int main()
{
    int win1 = gopen(500, 500);
    window(win1, -100, -100, 100, 100);
    gsetbgcolorrgb(win1, 255, 0, 0);
    gclr(win1);

    int win2 = gopen(500, 500);
    window(win2, -10, -10, 10, 10);
    gsetbgcolorrgb(win2, 0, 255, 0);
    gclr(win2);

    int win3 = gopen(500, 500);
    window(win3, -1, -1, 1, 1);
    gsetbgcolorrgb(win3, 0, 0, 255);
    gclr(win3);

    //gsetnonblock(ENABLE);
    while (true) {
        int type, button;
        double x, y;
        int win = ggetxpress(&type, &button, &x, &y);
        cout << "win: " << win << ", type: " << type << ", button: " << button
            << ", x: " << x << ", y:" << y << endl;
        if (type == KeyPress && button == 0x1b) break;
    }

    ggetch();
    gcloseall();
    return 0;
}
