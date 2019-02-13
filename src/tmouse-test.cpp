#include <t-pro1.h>

void draw()
{
    double x = 0; //�{�[����x���W
    double y = 0; //�{�[����y���W
    double dx = 1 + rand()*2.0 / (RAND_MAX + 1); //�{�[����x���W�̈ړ���
    double dy = 1 + rand()*2.0 / (RAND_MAX + 1); //�{�[����y���W�̈ړ���
    double r = 20; //�{�[���̔��a
    double wp = 40; //�p�h���̕�
    double hp = 10; //�p�h���̍���
    double xp = 0; //�p�h����x���W
    double yp = YMIN + hp / 2; //�p�h����y���W
    int score = 0; //�ł��Ԃ�����
    gsetnonblock(ENABLE);
    layer(win, 0, 1);
    for (;;) {
        gclrGrid(win);
        //�p�h��
        xp = mouseX;
        if (xp > XMAX - wp / 2) { //�E�̉��ɂ߂荞��ł��邩�H
            xp = XMAX - wp / 2;
        } else if (xp < XMIN + wp / 2) { //���̉��ɂ߂荞��ł��邩�H
            xp = XMIN + wp / 2;
        }
        //�{�[��x���W
        x += dx;
        if (x > XMAX - r) {  //�E�̉��ɂ߂荞��ł��邩�H
            x = XMAX - r;
            dx = -dx;
        } else if (x < XMIN + r) { //���̉��ɂ߂荞��ł��邩�H
            x = XMIN + r;
            dx = -dx;
        }
        //�{�[��y���W
        y += dy;
        if (y > YMAX - r) { //��̉��ɂ߂荞��ł��邩�H
            y = YMAX - r;
            dy = -dy;
        } else if (y < YMIN + r + hp
            && xp - wp / 2 < x && x < xp + wp / 2) { //�p�h���ɓ����������H
            y = YMIN + r + hp;
            dy = -dy;
            score++;
        }
        newrgbcolor(win, 0, 0, 0); //���F
        fillrect(win, xp - wp / 2, yp - hp / 2, wp, hp); //�p�h��
        newrgbcolor(win, 255, 0, 0); //�ԐF
        fillcirc(win, x, y, r, r); //�{�[��
        newrgbcolor(win, 0, 0, 0); //���F
        drawstr(win, -90, 80, 50, 0, "%2d", score); //���_
        if (y < YMIN - r) { //���̉����S�̂������H
            break;
        }
        copylayer(win, 1, 0);
        msleep(20);
    }
    newrgbcolor(win, 0, 0, 0); //���F
    drawstr(win, -90, -20, 100, 0, "GAME OVER");
    copylayer(win, 1, 0);
}
