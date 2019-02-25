//ggetxpress()の動作確認
#include <iostream>
#include <string>
#include <wineggx.h>
using namespace std;

int main()
{
  int win1 = gopen(500, 500);
  window(win1, -100, -100, 100, 100);
  gsetbgcolor(win1, "red");
  gclr(win1);
  newrgbcolor(win1, 0, 0, 0);

  int win2 = gopen(500, 500);
  window(win2, -50, -50, 50, 50);
  gsetbgcolor(win2, "green");
  gclr(win2);
  newrgbcolor(win2, 0, 0, 0);

  int win3 = gopen(500, 500);
  window(win3, -25, -25, 25, 25);
  gsetbgcolor(win3, "blue");
  gclr(win3);
  newrgbcolor(win3, 0, 0, 0);

  bool all = true;
  cout << "non block? (y/n): ";
  string ans;
  cin >> ans;
  bool nonblock = (ans == "y");
  if (nonblock) {
    gsetnonblock(ENABLE);
    cout << "skip if win == -1? (y/n): ";
    string ans2;
    cin >> ans2;
    if (ans2 == "y") {
      all = false;
    }
  } else {
    gsetnonblock(DISABLE);
  }

  while (true) {
    int type, button;
    double x, y;
#if 1
    //参照引数版（wineggx独自）
    int win = ggetxpress(type, button, x, y);
#else
    //EGGX互換版
    int win = ggetxpress(&type, &button, &x, &y);
#endif
    if (all || win != -1) {
      cout << "win: " << win << ", type: " << type << ", button: " << button
        << ", x: " << x << ", y: " << y << endl;
    }
    if (win >= 0 && type >= 0) {
      if (1 <= button && button <= 3) {
        int c = button * 80;
        newrgbcolor(win, c, c, c);
      } else {
        newrgbcolor(win, 0, 0, 0);
      }
      static double r = 1;
      if (button == 4) {
          r += 0.1;
      } else if (button == 5) {
          r -= 0.1;
      }
      fillcirc(win, x, y, r, r);
    }

    if (type == KeyPress && button == 0x1b) break;

    if (nonblock) {
      msleep(30);
    }
  }

  gcloseall();
  return 0;
}
