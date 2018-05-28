#include <iostream>
#include <wineggx.h>
using namespace std;

int main()
{
    int win = gopen(500, 500);
    //gsetnonblock(ENABLE);
    for (;;) {
      int k = ggetch();
      cout << k << endl;
      //msleep(100);
    }
    gclose(win);
    return 0;
}
