#include <iostream>
#include <string>
#include <wineggx.h>
using namespace std;

int main()
{
    int win = gopen(500, 500);
    string s = "abc %d %lf";
    winname(win, s, 123, 456.789);
    window(win, -100, -100, 100, 100);
    string ss = "#%02x%02x%02x";
    gsetbgcolor(win, ss, 255, 255, 0);
    gclr(win);
    newcolor(win, ss, 255, 0, 255);
    circle(win, 30, 40, 50, 50);
    string sss = "Consolas";
    gsetfontset(win, sss);
    drawstr(win, -90, -50, 50, 0, s, 987, 654.321);
    cout << "hello, EGGX!" << endl;
    ggetch();
    gclose(win);
    return 0;
}
