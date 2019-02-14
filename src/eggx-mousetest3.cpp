/* egg mousetst3.c -o mousetst3 -Wall */
#include <stdio.h>
#include <math.h>
#include <wineggx.h>
#include <iostream>
using namespace std;

#define WIDTH 400
#define HEIGHT 300
#define X_CEN (WIDTH/2)
#define Y_CEN (HEIGHT/2)
#define CNT_MAX 64
#define DIV 8.0

int main()
{
  int win;
  int cnt;
  float x_cen = X_CEN;
  float y_cen = Y_CEN;

  win = gopen(WIDTH, HEIGHT);

  puts("Move your mouse cursor on the window :-)");

  /* ���C���̐ݒ������ */
  layer(win, 0, 1);
  /* �m���u���b�N���[�h�ɂ��� */
  gsetnonblock(ENABLE);

  cnt = 0;
  while (1) {
    int win_ev;	/* �C�x���g�̂������E�B���h�D�ԍ� */
    int type, b;	/* �C�x���g�̃^�C�v�C�{�^�� */
    float r, x, y;
    win_ev = ggetevent(&type, &b, &x, &y);
    cout << win_ev << " " << type << " " << b << " " << x << " " << y << endl;
    if (win_ev == win) {
      if (type == MotionNotify) {
        x_cen = x;
        y_cen = y;
      } else if (type == ButtonPress) {
        break;
      }
    } else {
      int i;
      if (CNT_MAX <= cnt) cnt = 0;
      gclr(win);
      for (i = 0; i < 4; i++) {
        r = pow((cnt + i*(CNT_MAX / 4)) % CNT_MAX, 2) / DIV;
        drawcirc(win, x_cen, y_cen, r, r);
      }
      copylayer(win, 1, 0);
      msleep(20);
      cnt++;
    }
  }
  gclose(win);

  return(0);
}
