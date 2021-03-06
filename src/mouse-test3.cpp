//大域変数windowId, mouseX, mouseY, mousePressed, mouseButtonの動作確認．
#include <iostream>
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

  while (true) {
    cout << "windowId: " << windowId << "，mousePressed: " << mousePressed << ", mouseButton: " << mouseButton
      << ", mouseX: " << mouseX << ", mouseY: " << mouseY << endl;
    static double r = 1;
    if (mousePressed) {
      if (1<= mouseButton && mouseButton <= 3) {
        int c = mouseButton * 80;
        newrgbcolor(windowId, c, c, c);
      } else if (mouseButton == 4) {
        r += 0.1;
      } else if (mouseButton == 5) {
        r -= 0.1;
      }
    } else {
      newrgbcolor(windowId, 0, 0, 0);
    }
    fillcirc(windowId, mouseX, mouseY, r, r);

    if (90 < mouseX && mouseX < 100 && 90 < mouseY && mouseY < 100) break;

    msleep(30);
  }

  gcloseall();
  return 0;
}
