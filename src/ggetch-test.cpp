//ggetch()‚Ì“®ìŠm”F
#include <iostream>
#include <string>
#include <wineggx.h>
using namespace std;

int main()
{
  int win = gopen(500, 500);

  cout << "non block? (y/n): ";
  string ans;
  cin >> ans;
  bool nonblock = (ans == "y");
  if (nonblock) {
    gsetnonblock(ENABLE);
  } else {
    gsetnonblock(DISABLE);
  }

  while (true) {
    int k = ggetch();
    cout << k << endl;
    if (nonblock) {
      msleep(100);
    }
  }
  gclose(win);
  return 0;
}
