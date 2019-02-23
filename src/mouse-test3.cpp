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
    if (mousePressed) {
      int c = mouseButton * 80;
      newrgbcolor(windowId, c, c, c);
    } else {
      newrgbcolor(windowId, 0, 0, 0);
    }
    fillcirc(windowId, mouseX, mouseY, 1, 1);
    msleep(30);

    if (mouseX > 90 && mouseY > 90) break;
  }

  gcloseall();
  return 0;
}
