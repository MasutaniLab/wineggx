#include <t-pro1.h>

void draw()
{
    double x = 0; //ボールのx座標
    double y = 0; //ボールのy座標
    double dx = 1 + rand()*2.0 / (RAND_MAX + 1); //ボールのx座標の移動量
    double dy = 1 + rand()*2.0 / (RAND_MAX + 1); //ボールのy座標の移動量
    double r = 20; //ボールの半径
    double wp = 40; //パドルの幅
    double hp = 10; //パドルの高さ
    double xp = 0; //パドルのx座標
    double yp = YMIN + hp / 2; //パドルのy座標
    int score = 0; //打ち返した回数
    gsetnonblock(ENABLE);
    layer(win, 0, 1);
    for (;;) {
        gclrGrid(win);
        //パドル
        xp = mouseX;
        if (xp > XMAX - wp / 2) { //右の縁にめり込んでいるか？
            xp = XMAX - wp / 2;
        } else if (xp < XMIN + wp / 2) { //左の縁にめり込んでいるか？
            xp = XMIN + wp / 2;
        }
        //ボールx座標
        x += dx;
        if (x > XMAX - r) {  //右の縁にめり込んでいるか？
            x = XMAX - r;
            dx = -dx;
        } else if (x < XMIN + r) { //左の縁にめり込んでいるか？
            x = XMIN + r;
            dx = -dx;
        }
        //ボールy座標
        y += dy;
        if (y > YMAX - r) { //上の縁にめり込んでいるか？
            y = YMAX - r;
            dy = -dy;
        } else if (y < YMIN + r + hp
            && xp - wp / 2 < x && x < xp + wp / 2) { //パドルに当たったか？
            y = YMIN + r + hp;
            dy = -dy;
            score++;
        }
        newrgbcolor(win, 0, 0, 0); //黒色
        fillrect(win, xp - wp / 2, yp - hp / 2, wp, hp); //パドル
        newrgbcolor(win, 255, 0, 0); //赤色
        fillcirc(win, x, y, r, r); //ボール
        newrgbcolor(win, 0, 0, 0); //黒色
        drawstr(win, -90, 80, 50, 0, "%2d", score); //得点
        if (y < YMIN - r) { //下の縁より全体が下か？
            break;
        }
        copylayer(win, 1, 0);
        msleep(20);
    }
    newrgbcolor(win, 0, 0, 0); //黒色
    drawstr(win, -90, -20, 100, 0, "GAME OVER");
    copylayer(win, 1, 0);
}
