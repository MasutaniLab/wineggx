#include <iostream>
#include <wineggx.h>
using namespace std;

int main()
{
    int win = gopen(500, 500);
    window(win, -100, -100, 100, 100);
    gsetbgcolorrgb(win, 255, 255, 255);
    gclr(win);
    newrgbcolor(win, 0, 0, 0);
    circle(win, 30, 40, 50, 50);
    cout << "hello, EGGX!" << endl;
    ggetch();
    gclose(win);
    return 0;
}
