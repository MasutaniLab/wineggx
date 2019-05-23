#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <string>
#include <wineggx.h>
using namespace std;

//�O���[�o���萔�E�ϐ�
int win;
const int XMIN = -100;
const int XMAX = +100;
const int YMIN = -100;
const int YMAX = +100;

//�֐��v���g�^�C�v
void initializeDraw(void);
void drawGrid(int win, double xmin, double ymin, double xmax, double ymax, double grid);
void terminateDraw(void);
void draw();
void gclrGrid(int win);

int main()
{
    srand((unsigned int)time(NULL));
    initializeDraw();
    draw();
    terminateDraw();
    return 0;
}

//�T�v�FEGGX�����������C���W�n��`��
//�����F�Ȃ�
//�߂�l�F�Ȃ�
void initializeDraw(void)
{
    win = gopen(500, 500);
    window(win, XMIN, YMIN, XMAX, YMAX);
    gsetbgcolorrgb(win, 255, 255, 255);
    gclrGrid(win);
}

//�T�v�F�ėp�̍��W���Ɗi�q��`���D
//�����Fwin �E�B���h�E�̔ԍ��C
//      xmin x�̍ŏ��l�Cymin y�̍ŏ��l�Cxman x�̍ő�l�Cymax y�̍ő�l�C
//      grid �i�q�̊Ԋu
//�߂�l�F�Ȃ�
void drawGrid(int win,
              double xmin, double ymin, double xmax, double ymax,
              double grid)
{
    gclr(win);
    newrgbcolor(win, 0, 0, 0);
    newlinewidth(win, 1);
    newlinestyle(win, LineOnOffDash);
    for (double x = xmin; x <= xmax; x += grid) {
        drawline(win, x, ymin, x, ymax);
    }
    for (double y = ymin; y <= ymax; y += grid) {
        drawline(win, xmin, y, xmax, y);
    }
    newlinestyle(win, LineSolid);
    drawline(win, 0, ymin, 0, ymax);
    drawline(win, xmin, 0, xmax, 0);
}

//�T�v�F����̍��W���Ɗi�q��`���D
//�����Fwin �E�B���h�E�̔ԍ�
//�߂�l�F�Ȃ�
void gclrGrid(int win)
{
    drawGrid(win, XMIN, YMIN, XMAX, YMAX, 10);
}

//�T�v�FEGGX�̏I��
//�����F�Ȃ�
//�߂�l�F�Ȃ�
void terminateDraw(void)
{
  gsetnonblock(DISABLE);
  while (ggetch() != 27); //ESC�L�[�ŏI��
  gclose(win);
}