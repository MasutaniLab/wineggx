/**
* @file wineggx.cpp
* @brief Windows��EGGX
* @section Notes
*  Copyright (C) 2007-2014 Team ODENS \n\n
* @date �X�V���� �X�V��\n
* 2019/02/XX ���J �۔� Version 2.0\n
* 2014/10/16 ���J �۔��uWindows��EGGX Version 1.0�v�𖼏��\n
* 2014/03/12 ���J �۔�\n
*/

/**
* @addtogroup wineggx_back wineggx-WinEggX���C�u����
* @brief Windows��eggx���C�u�����̑��������
**/

#define WINEGGX_MAIN
#include "wineggx.h"
#include "ceggx.h"

using namespace std;
bool first = true;

/**
* @brief      �C�ӂ̃T�C�Y�̃O���t�B�b�N�X��ʂ��J��
* @ingroup    wineggx
* @param[in]  xsize �E�B���h�E�̉���
* @param[in]  ysize �E�B���h�E�̏c��
* @return     �Ԃ�l�ɂ�EGGX�Ŏg�p����C�E�B���h�E�ԍ����Ԃ��Ă��܂��D
* @retval     -1 �G���[
*/
int gopen(int xsize, int ysize)
{
  assert(xsize&&ysize&&"eggx set window size.");
  if (first) {
    gEggX.setmouse(&windowId, &mouseX, &mouseY, &mouseButton, &mousePressed);
    first = false;
  }
  return gEggX.gopen(xsize, ysize);
}

/**
* @brief      �O���t�B�b�N�X�E�B���h�E�����
* @ingroup    wineggx
* @param[in]  wn �E�B���h�E�ԍ�
*/
void gclose(int wn)
{
  return gEggX.gclose(wn);
}

/**
* @brief      ���ׂẴO���t�B�b�N�X�p�E�B���h�D�����
* @ingroup    wineggx
*/
void gcloseall(void)
{
  return gEggX.gcloseall();
}

/**
* @brief      �E�B���h�D�̃^�C�g����ύX����
* @ingroup    wineggx
* @param[in]  wn �^�C�g����ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �ݒ肷��E�B���h�E�̕�����
* @return     �ݒ肵���E�B���h�D�^�C�g���̕�����̒���
*/
int winname(int wn, const char *argsformat, ...)
{
  assert(argsformat&&"eggx set window names.");

  va_list argptr;              //�ϐ��������X�g
  va_start(argptr,argsformat); //initalize va_ function
  int len = gEggX.winname(wn, argsformat, argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      �ywineggx�Ǝ��z�E�B���h�D�̃^�C�g����ύX����istring�Łj
* @ingroup    wineggx
* @param[in]  wn �^�C�g����ύX����E�B���h�E�̔ԍ�
* @param[in]  ... �ݒ肷��E�B���h�E�̕�����
* @return     �ݒ肵���E�B���h�D�^�C�g���̕�����̒���
*/
int winname(int wn, const std::string argsformat, ...)
{
  va_list argptr;              //�ϐ��������X�g
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.winname(wn, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

//������
//void coordinate(int wn, int xw, int yw, double xa, double ya, double xscale, double yscale)
//{
//}

/**
* @brief      ���W�n�̕ύX
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  xs ������X���W
* @param[in]  ys ������Y���W
* @param[in]  xe �E���X���W
* @param[in]  ye �E���Y���W
*/
void window(int wn, double xs, double ys, double xe, double ye)
{
  return gEggX.window(wn, xs, ys, xe, ye);
}

/**
* @brief      ���C���̐ݒ������
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  lys �\�����C���[( 0 or 1 )
* @param[in]  lyw �������ݐ惌�C���[( 0 or 1 )
* @section Notes
*  ���ݕ\�����Ă��郌�C���ɑ΂���(lys == lyw �̏ꍇ��) �A�����ĕ`��֐������s����ƁC�ǂ��`��p
*  �t�H�[�}���X�������܂���D�����ȕ`�悪�K�v�ȏꍇ�ɂ́C���ݕ\�����Ă��Ȃ����C���ɑ΂��ĕ`�悵�C
*  �Ō�ɕ\�����C����layer() �֐��Ő؂�ւ��邩�Ccopylayer() �֐�(x2.5.10) �ŕ`�惌�C���̉摜��\
*  �����C���ɃR�s�[����悤�ɂ��܂��D\n
*  �f�t�H���g�ł�layer(wn,0,0) �̏�ԂƂȂ��Ă��܂��D
*/
void layer(int wn, int lys, int lyw)
{
  return gEggX.layer(wn, lys, lyw);
}

/**
* @brief      ���C���̃R�s�[������
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  lysrc  �R�s�[�����C���[�ԍ�
* @param[in]  lydest �R�s�[�惌�C���[�ԍ�
* @section Notes
*  wn �̃E�B���h�E�ԍ��́C���C��lysrc �̉摜�����C��lydest �ɂ��̂܂܃R�s�[���܂��D
*  ���̃R�s�[�͏u���ɍs���邽�߁C�A�j���[�V�����̍Đ��Ɏg�����Ƃ��ł��܂��D
*/
void copylayer(int wn, int lysrc, int lydest)
{
  return gEggX.copylayer(wn, lysrc, lydest);
}

/**
* @brief      �E�B���h�D�̔w�i�F�̕ύX
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �F��\��������
*  wn �Ŏw�肳�ꂽ�E�B���h�D�̔w�i�F��ύX���܂��Dargsformat(�Ƃ���ɑ�������)�Ŏw�肳��镶�����
*  �w�i�F�ɐݒ肵�܂��D2�߂̈���argsformat�ȍ~�́CC �W���֐���printf() �֐��̏ꍇ�Ɠ��l�̉ψ���
*  �ƂȂ��Ă��܂��D���̔w�i�F�̕�����ɂ́CX�E�B���h�E�Œ�`����Ă���F���C"#c0c0ff"�̂悤�ɁC16�i��
*  ��Red,Green,Blue ���w�肵�܂��D
*/
void gsetbgcolor(int wn, const char *argsformat, ...)
{
  assert(argsformat&&"eggx set background color.");

  va_list argptr;              //�ϐ��������X�g
  va_start(argptr, argsformat); //initalize va_ function
  gEggX.gsetbgcolor(wn, argsformat, argptr);//
  va_end(argptr);              //close     va_ function
}

/**
* @brief      �ywineggx�Ǝ��z�E�B���h�D�̔w�i�F�̕ύX�istring�Łj
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �F��\��������
*  wn �Ŏw�肳�ꂽ�E�B���h�D�̔w�i�F��ύX���܂��Dargsformat(�Ƃ���ɑ�������)�Ŏw�肳��镶�����
*  �w�i�F�ɐݒ肵�܂��D2�߂̈���argsformat�ȍ~�́CC �W���֐���printf() �֐��̏ꍇ�Ɠ��l�̉ψ���
*  �ƂȂ��Ă��܂��D���̔w�i�F�̕�����ɂ́CX�E�B���h�E�Œ�`����Ă���F���C"#c0c0ff"�̂悤�ɁC16�i��
*  ��Red,Green,Blue ���w�肵�܂��D
*/
void gsetbgcolor(int wn, const std::string argsformat, ...)
{
  va_list argptr;              //�ϐ��������X�g
  va_start(argptr, argsformat); //initalize va_ function
  gEggX.gsetbgcolor(wn, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function
}

/**
* @brief      �ywineggx�Ǝ��z�E�B���h�D�̔w�i�F�̕ύX
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  r  �ԐF ( 0 to 255 )
* @param[in]  g  �ΐF ( 0 to 255 )
* @param[in]  b  �F ( 0 to 255 )
* @section Notes
*  wn �Ŏw�肳�ꂽ�E�B���h�D�̔w�i�F��ύX���܂��Dr,g,b �ɂ͂��ꂼ��Red,Green,Blue �̋P�x��
*  256 �i�K�̐���(0�`255) �Ŏw�肵�܂��D
*/
void gsetbgcolorrgb(int wn, int r, int g, int b)
{
  return gEggX.gsetbgcolorrgb(wn, r, g, b);
}

/**
* @brief      �`�惌�C���̑S����
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�̕`�惌�C����w�i�F�Ɏw�肳�ꂽ�F�ŏ��������܂��D
*/
void gclr(int wn)
{
  return gEggX.gclr(wn);
}

//������
//void tclr(void)
//{
//}

//������
//void newpen(int wn, int cn)
//{
//}

/**
* @brief      �`��F�̕ύX
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �F��\��������
*  wn �Ŏw�肳�ꂽ�E�B���h�D�ł̕`��F��ύX���܂��Dargsformat(�Ƃ���ɑ�������)�Ŏw�肳��镶�����
*  �w�i�F�ɐݒ肵�܂��D2�߂̈���argsformat�ȍ~�́CC �W���֐���printf() �֐��̏ꍇ�Ɠ��l�̉ψ���
*  �ƂȂ��Ă��܂��D���̔w�i�F�̕�����ɂ́CX�E�B���h�E�Œ�`����Ă���F���C"#c0c0ff"�̂悤�ɁC16�i��
*  ��Red,Green,Blue ���w�肵�܂��D
*/
void newcolor( int wn, const char *argsformat, ... )
{
  assert(argsformat&&"eggx set new color.");

  va_list argptr;              //�ϐ��������X�g
  va_start(argptr, argsformat); //initalize va_ function
  gEggX.newcolor(wn, argsformat, argptr);//
  va_end(argptr);              //close     va_ function
}

/**
* @brief      �ywineggx�Ǝ��z�`��F�̕ύX�istring�Łj
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �F��\��������
*  wn �Ŏw�肳�ꂽ�E�B���h�D�ł̕`��F��ύX���܂��Dargsformat(�Ƃ���ɑ�������)�Ŏw�肳��镶�����
*  �w�i�F�ɐݒ肵�܂��D2�߂̈���argsformat�ȍ~�́CC �W���֐���printf() �֐��̏ꍇ�Ɠ��l�̉ψ���
*  �ƂȂ��Ă��܂��D���̔w�i�F�̕�����ɂ́CX�E�B���h�E�Œ�`����Ă���F���C"#c0c0ff"�̂悤�ɁC16�i��
*  ��Red,Green,Blue ���w�肵�܂��D
*/
void newcolor(int wn, const std::string argsformat, ...)
{
  va_list argptr;              //�ϐ��������X�g
  va_start(argptr, argsformat); //initalize va_ function
  gEggX.newcolor(wn, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function
}

/**
* @brief      �`��F�̕ύX
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  r  �ԐF ( 0 to 255 )
* @param[in]  g  �ΐF ( 0 to 255 )
* @param[in]  b  �F ( 0 to 255 )
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ł̕`��F��ύX���܂��Dr,g,b �ɂ͂��ꂼ��Red,Green,Blue �̋P�x��
*  256 �i�K�̐���(0�`255) �Ŏw�肵�܂��D
*/
void newrgbcolor(int wn, int r, int g, int b)
{
  return gEggX.newrgbcolor(wn, r, g, b);
}

/**
* @brief      �`��F�̕ύX
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  h  �F�� ( 0 to 359 )
* @param[in]  s  �ʓx ( 0 to 255 )
* @param[in]  v  ���x ( 0 to 255 )
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ł̕`��F��ύX���܂��Dh,s,v �ɂ͂��ꂼ��CHue,Satulation,Value��
*  �w�肵�܂�)�Ds��v��256�i�K�̐���(0�`255)���Ch��0�`359�܂ł̐���(�p�x)���w�肵�܂��D
*/
void newhsvcolor(int wn, int h, int s, int v)
{
  return gEggX.newhsvcolor(wn, h, s, v);
}

//������
//int makecolor(int cmode, double dmin, double dmax, double data, int *r, int *g, int *b)
//{
//}

/**
* @brief      �����̕ύX
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  width ����
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�Ő���`�����̐�����ύX���܂��D
*  �f�t�H���g�ł͕�1 ���ݒ肳��Ă��܂��D
*/
void newlinewidth(int wn, int width)
{
  return gEggX.newlinewidth(wn, width);
}

/**
* @brief      ���̃X�^�C���̕ύX
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  style ���̃X�^�C��
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�Ő���`�����̃X�^�C����ύX���܂��D
*  ����style �ɗ^���鎖���ł���l�́CLineSolid(����) ��LineOnOffDash(�_��) �ł��D
*  �f�t�H���g�ł͎���(LineSolid) ���ݒ肳��Ă��܂��D
*  Windows API�̎d�l��CLineSolid�͐���1�ȉ��łȂ��ƗL���łȂ��D
*/
void newlinestyle(int wn, int style)
{
  return gEggX.newlinestyle(wn, style);
}

//������
//void newgcfunction(int wn, int fnc)
//{
//}

/**
* @brief      �_�̕`��
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x,y �`�����W
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ɓ_��`�悵�܂��D
*/
void pset(int wn, double x, double y)
{
  return gEggX.pset(wn, x, y);
}

/**
* @brief      �����̕`��
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x0,y0 �[�_�̍��W
* @param[in]  x1,y1 �[�_�̍��W
* @section Notes
*  wn �Ŏw�肵���E�B���h�D��(x0, y0) ����(x1, y1) �ɒ�����`�悵�܂��D
*/
void drawline(int wn, double x0, double y0, double x1, double y1)
{
  return gEggX.drawline(wn, x0, y0, x1, y1);
}

/**
* @brief      ������`�����߂̏����ʒu�̐ݒ�
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x,y �`�����W
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ɁClineto() �֐��̂��߂̏����ʒu��(x, y) �ɐݒ肵�܂��D
*/
void moveto(int wn, double x, double y)
{
  return gEggX.moveto(wn, x, y);
}

/**
* @brief      �A���I�ɒ�����`��
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x,y ���W
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ɁC�ȑOmoveto()�܂���lineto() ���Ă΂ꂽ���Ɏw�肳�ꂽ���W����
*  (x, y) �֒����������܂��D
*/
void lineto(int wn, double x, double y)
{
  return gEggX.lineto(wn,x,y);
}

/**
* @brief      �����̕`��
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x,y �`�����W
* @param[in]  mode �`�惂�[�h
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ɒ�����`�悵�܂��D
*  mode �� @ref PENDOWN ���w�肷��ƈȑOline() �֐����Ă΂ꂽ�_����C
*  (x,y) �֒����������܂��Dmode ��PENUP ���w�肷���(x,y) ��
*  line() �֐��̏����ʒu�ɐݒ肵�܂��Dmode=PENDOWN �Ńy�������낵��
*  �`��Cmode=PENUP �Ńy�����グ�Ĉړ��ƍl����Ƃ킩��₷���ł��傤�D
*   �܂��Cmode=PSET �̏ꍇ��(x,y) �ɓ_��`���C�y���̈ʒu���X�V����܂��D
*/
void line(int wn, double x, double y, int mode)
{
  return gEggX.line(wn,x,y,mode);
}

/**
* @brief      �����̓_��`���idouble�Łj
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] �e�_��x���W
* @param[in]  y[] �e�_��y���W
* @param[in]  n �_�̐�
* @section Notes
*  n�̓_��`���D
*  x�Cy��n�̎����̈ꎟ���z��ŁCx[0]�`x[n-1]�Cy[0]�`y[n-1]�Ɋe�_�̍��W�����Ă����D
*/
void drawpts(int wn, const double x[], const double y[], int n)
{
  gEggX.drawpts(wn, x, y, n);

}

/**
* @brief      �����̓_��`���ifloat�Łj
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] �e�_��x���W
* @param[in]  y[] �e�_��y���W
* @param[in]  n �_�̐�
* @section Notes
*  n�̓_��`���D
*  x�Cy��n�̎����̈ꎟ���z��ŁCx[0]�`x[n-1]�Cy[0]�`y[n-1]�Ɋe�_�̍��W�����Ă����D
*/
void drawpts(int wn, const float x[], const float y[], int n)
{
  double *xd = new double[n];
  double *yd = new double[n];
  for (int i = 0; i < n; i++) {
    xd[i] = x[i];
    yd[i] = y[i];
  }
  gEggX.drawpts(wn, xd, yd, n);
  delete xd;
  delete yd;
}

/**
* @brief      �܂����`���idouble�Łj
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] �܂���̊e�_��x���W
* @param[in]  y[] �܂���̊e�_��y���W
* @param[in]  n �_�̐�
* @section Notes
*  wn�Ŏw�肵���E�B���h�D�ŁC�܂����`���D
*  x�Cy��n�̎����̈ꎟ���z��ŁCx[0]�`x[n-1]�Cy[0]�`y[n-1]�ɐ܂����
*  �e�_�̍��W�����Ă����D
*/
void drawlines(int wn, const double x[], const double y[], int n)
{
  gEggX.drawlines(wn, x, y, n);
}

/**
* @brief      �܂����`���ifloat�Łj
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] �܂���̊e�_��x���W
* @param[in]  y[] �܂���̊e�_��y���W
* @param[in]  n �_�̐�
* @section Notes
*  wn�Ŏw�肵���E�B���h�D�ŁC�܂����`���D
*  x�Cy��n�̎����̈ꎟ���z��ŁCx[0]�`x[n-1]�Cy[0]�`y[n-1]�ɐ܂����
*  �e�_�̍��W�����Ă����D
*/
void drawlines(int wn, const float x[], const float y[], int n)
{
  double *xd = new double[n];
  double *yd = new double[n];
  for (int i = 0; i < n; i++) {
    xd[i] = x[i];
    yd[i] = y[i];
  }
  gEggX.drawlines(wn, xd, yd, n);
  delete xd;
  delete yd;
}

/**
* @brief      ���p�`��`���idouble�Łj
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] ���p�`�̊e�_��x���W
* @param[in]  y[] ���p�`�̊e�_��y���W
* @param[in]  n �_�̐�
* @section Notes
*  wn�Ŏw�肵���E�B���h�D�ŁC���p�`��`���D
*  x�Cy��n�̎����̈ꎟ���z��ŁCx[0]�`x[n-1]�Cy[0]�`y[n-1]�ɐ܂����
*  �e�_�̍��W�����Ă����D
*/
void drawpoly(int wn, const double x[], const double y[], int n)
{
  gEggX.drawpoly(wn, x, y, n);
}

/**
* @brief      ���p�`��`���ifloat�Łj
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] ���p�`�̊e�_��x���W
* @param[in]  y[] ���p�`�̊e�_��y���W
* @param[in]  n �_�̐�
* @section Notes
*  wn�Ŏw�肵���E�B���h�D�ŁC���p�`��`���D
*  x�Cy��n�̎����̈ꎟ���z��ŁCx[0]�`x[n-1]�Cy[0]�`y[n-1]�ɐ܂����
*  �e�_�̍��W�����Ă����D
*/
void drawpoly(int wn, const float x[], const float y[], int n)
{
  double *xd = new double[n];
  double *yd = new double[n];
  for (int i = 0; i < n; i++) {
    xd[i] = x[i];
    yd[i] = y[i];
  }
  gEggX.drawpoly(wn, xd, yd, n);
  delete xd;
  delete yd;
}

/**
* @brief      ���p�`��h��ׂ��idouble�Łj
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] ���p�`�̊e�_��x���W
* @param[in]  y[] ���p�`�̊e�_��y���W
* @param[in]  n �_�̐�
* @param[in]  i �h��ׂ����̌`��
* @section Notes
*  wn�Ŏw�肵���E�B���h�D�ŁC���p�`�̗̈��h��ׂ��D
*  x�Cy��n�̎����̈ꎟ���z��ŁCx[0]�`x[n-1]�Cy[0]�`y[n-1]�ɐ܂����
*  �e�_�̍��W�����Ă����D
*  i�͓h��ׂ����̌`��Œʏ��0���C�ʑ��p�`�̎���1���w�肷��D
*/
void fillpoly(int wn, const double x[], const double y[], int n, int i)
{
  gEggX.fillpoly(wn, x, y, n, i);
}

/**
* @brief      ���p�`��h��ׂ��ifloat�Łj
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x[] ���p�`�̊e�_��x���W
* @param[in]  y[] ���p�`�̊e�_��y���W
* @param[in]  n �_�̐�
* @param[in]  i �h��ׂ����̌`��
* @section Notes
*  wn�Ŏw�肵���E�B���h�D�ŁC���p�`�̗̈��h��ׂ��D
*  x�Cy��n�̎����̈ꎟ���z��ŁCx[0]�`x[n-1]�Cy[0]�`y[n-1]�ɐ܂����
*  �e�_�̍��W�����Ă����D
*  i�͓h��ׂ����̌`��Œʏ��0���C�ʑ��p�`�̎���1���w�肷��D
*/
void fillpoly(int wn, const float x[], const float y[], int n, int i)
{
  double *xd = new double[n];
  double *yd = new double[n];
  for (int i = 0; i < n; i++) {
    xd[i] = x[i];
    yd[i] = y[i];
  }
  gEggX.fillpoly(wn, xd, yd, n, i);
  delete xd;
  delete yd;
}

/**
* @brief      �����`�̕`��
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x,y  �`����W�i�����`�̍����j
* @param[in]  w    �����`�̉���
* @param[in]  h    �����`�̏c��
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ɁC���_(x,y) ���琳�̕����ɕ�w�C
*  ����h �̒����`��`���܂��D�D
*/
void drawrect(int wn, double x, double y, double w, double h)
{
  return gEggX.drawrect(wn, x, y, w, h);
}

/**
* @brief      �����`�̕`��i�h��Ԃ��j
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x,y  �`����W�i�����`�̍����j
* @param[in]  w    �����`�̉���
* @param[in]  h    �����`�̏c��
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ɁC���_(x,y) ���琳�̕����ɕ�w�C
*  ����h �̒����`��`���܂��D�D
*/
void fillrect(int wn, double x, double y, double w, double h)
{
  return gEggX.fillrect(wn, x, y, w, h);
}

/**
* @brief      �~�̕`��icircle()�ƑS�������j
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  xcen,ycen �~�̒��S���W
* @param[in]  xrad X�������̔��a
* @param[in]  yrad Y�������̔��a
* @section Notes
*  wn �Ŏw�肵���E�B���h�E�ɁC(xcen,ycen) �𒆐S��
*  �������̔��axrad�C�c�����̔��ayrad �̉~��`���܂��D
*/
void drawcirc(int wn, double xcen, double ycen, double xrad, double yrad)
{
  return gEggX.circle(wn, xcen, ycen, xrad, yrad);
}

/**
* @brief      �~�̕`��idrawcirc()�ƑS�������j
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  xcen,ycen �~�̒��S���W
* @param[in]  xrad X�������̔��a
* @param[in]  yrad Y�������̔��a
* @section Notes
*  wn �Ŏw�肵���E�B���h�E�ɁC(xcen,ycen) �𒆐S��
*  �������̔��axrad�C�c�����̔��ayrad �̉~��`���܂��D
*/
void circle(int wn, double xcen, double ycen, double xrad, double yrad)
{
  return gEggX.circle(wn,xcen,ycen,xrad,yrad);
}

/**
* @brief      ���S���W�C���a��^���ĉ~��h��ׂ�
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  xcen,ycen �~�̒��S���W
* @param[in]  xrad X�������̔��a
* @param[in]  yrad Y�������̔��a
* @section Notes
*  wn �Ŏw�肵���E�B���h�E�ɁC(xcen,ycen) �𒆐S��
*  �������̔��axrad�C�c�����̔��ayrad �̉~��h��Ԃ��܂��D
*/
void fillcirc(int wn, double xcen, double ycen, double xrad, double yrad)
{
  return gEggX.fillcirc(wn,xcen,ycen,xrad,yrad);
}

/**
* @brief      �~�ʂ̕`��
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  xcen,ycen �~�̒��S���W
* @param[in]  xrad X�������̔��a
* @param[in]  yrad Y�������̔��a
* @param[in]  sang �`��J�n�p�x [degree]
* @param[in]  eang �`��I���p�x [degree]
* @param[in]  idir �`����� ( 1 ����� or -1 �E���)
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ɁC(xcen,ycen) �𒆐S�ɉ������̔��axrad�C
*  �c�����̔��ayrad �̉~�ʂ�`���܂��Dsang �͊J�n�p�Ceang �͏I���p�ŁC
*  �x�ŗ^���܂��Didir �͉~�ʂ�`��������1 �ō����C-1 �ŉE���ƂȂ�܂��D
*/
void drawarc(int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir)
{
  return gEggX.drawarc(wn, xcen, ycen, xrad, yrad, sang, eang, idir);
}


/**
* @brief      �~�ʂ̕`��(�h��Ԃ�)
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  xcen,ycen �~�̒��S���W
* @param[in]  xrad X�������̔��a
* @param[in]  yrad Y�������̔��a
* @param[in]  sang �`��J�n�p�x [degree]
* @param[in]  eang �`��I���p�x [degree]
* @param[in]  idir �`����� ( 1 ����� or -1 �E���)
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ŁC(xcen,ycen) �𒆐S�ɉ������̔��axrad�C
*  �c�����̔��ayrad �̉~�ʂ�h��ׂ��܂��Dsang �͊J�n�p�Ceang �͏I���p�ŁC
*  �x�ŗ^���܂��Didir �͉~�ʂ�`��������1 �ō����C-1�ŉE���ƂȂ�܂��D
*/
void fillarc(int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir)
{
  return gEggX.fillarc(wn, xcen, ycen, xrad, yrad, sang, eang, idir);
}

//������
//void drawsym(int wn, int x, int y, int size, int symtype)
//{
//}

//������
//void drawsyms(int wn, const double x[], const double y[], int n, int size, int symtype)
//{
//}

//������
//void drawarrow(int wn, double xs, double ys, double xt, double yt, double s, double w, int 10 * i + j)
//{
//}

/**
* @brief      �`��t�H���g�̐ݒ�igsetfontset()�ƑS�������j
* @ingroup    wineggx
* @param[in]  wn    �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �t�H���g�Z�b�g�̖��O \n �k���������܂߂�32Byte�ȉ��łȂ���΂Ȃ�Ȃ��D
* @retval     -1 �G���[
* @retval     0  �擾����
* @retval     1  ��փt�H���g�̎擾����
*/
int newfontset(int wn, const char *argsformat, ...)
{
  assert(argsformat&&"eggx set fontset.");

  va_list argptr;              //�ϐ��������X�g
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.gsetfontset(wn, argsformat, argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      �ywineggx�Ǝ��z�`��t�H���g�̐ݒ�igsetfontset()�ƑS�������j�istring�Łj
* @ingroup    wineggx
* @param[in]  wn    �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �t�H���g�Z�b�g�̖��O \n �k���������܂߂�32Byte�ȉ��łȂ���΂Ȃ�Ȃ��D
* @retval     -1 �G���[
* @retval     0  �擾����
* @retval     1  ��փt�H���g�̎擾����
*/
int newfontset(int wn, const std::string argsformat, ...)
{
  va_list argptr;              //�ϐ��������X�g
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.gsetfontset(wn, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      �`��t�H���g�̐ݒ�inewfontset()�ƑS�������j
* @ingroup    wineggx
* @param[in]  wn    �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �t�H���g�Z�b�g�̖��O \n �k���������܂߂�32Byte�ȉ��łȂ���΂Ȃ�Ȃ��D
* @retval     -1 �G���[
* @retval     0  �擾����
* @retval     1  ��փt�H���g�̎擾����
*/
int gsetfontset(int wn, const char *argsformat, ...)
{
  assert(argsformat&&"eggx set fontset.");

  va_list argptr;              //�ϐ��������X�g
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.gsetfontset(wn, argsformat, argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      �ywineggx�Ǝ��z�`��t�H���g�̐ݒ�inewfontset()�ƑS�������j�istring�Łj
* @ingroup    wineggx
* @param[in]  wn    �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �t�H���g�Z�b�g�̖��O \n �k���������܂߂�32Byte�ȉ��łȂ���΂Ȃ�Ȃ��D
* @retval     -1 �G���[
* @retval     0  �擾����
* @retval     1  ��փt�H���g�̎擾����
*/
int gsetfontset(int wn, const std::string argsformat, ...)
{
  va_list argptr;              //�ϐ��������X�g
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.gsetfontset(wn, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      ������̕`��
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x,y   �`�����W�i�������`�悷��ʒu�̍����̍��W�j
* @param[in]  size  �`�悷�镶���̑傫��
* @param[in]  theta ������̉�]�p�x [degree] ( * ���݂͖��� )
* @param[in]  argsformat, ... ������
* @return     ���ۂɕ`�悵��������̒���
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ɁC����������W(x,y) ����`���܂��D
*  size �͕����̑傫���ŁC�h�b�g�P�ʂŎw�肵�܂��D
*  theta �͕�����̉�]���w�肷������ł����C���o�[�W�����ł͋@�\���܂���D
*  �������argsformat �ɗ^���܂����C���̈����ȍ~��printf() �֐��̈����Ɠ��l��
*  �t�H�[�}�b�g�ɂȂ��Ă��܂��̂ŁC�g�p��̂悤�ɕϐ��̒l�Ȃǂ�`�������ł��܂��D
*  �����̃T�C�Ysize ��1�`24 �͈̔͂Ŏw��ł��܂��D
*  size �Ǝ��ۂ̃t�H���g�Ƃ̊֌W�͈ȉ��̕\�̂悤�ɂȂ��Ă��܂��D
*  ���̏ꍇ�C�����͔��p�p�����̂ݕ`��ł��܂��D2 �o�C�g����(����)
*  ��`�悷��ꍇ�́Csize �ɂ�FONTSET ���w�肵�܂��D
*  ���̏ꍇ�̃t�H���g�̎w��́Cgsetfontset() �֐��𗘗p���܂��D
*  gsetfontset() �ł̃t�H���g�w�肪�Ȃ��ꍇ�́C�f�t�H���g��14 �h�b�g�̃t�H���g�Z�b�g�ŕ`�悳��܂��D
*/
int drawstr( int wn, double x, double y, int size, double theta,const char *argsformat, ... )
{
  assert(argsformat&&"eggx draw strings.");

  va_list argptr;              //�ϐ��������X�g
  va_start(argptr,argsformat); //initalize va_ function
  int len = gEggX.drawstr(wn, x, y, size, theta, argsformat, argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

/**
* @brief      �ywineggx�Ǝ��z������̕`��istring�Łj
* @ingroup    wineggx
* @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
* @param[in]  x,y   �`�����W�i�������`�悷��ʒu�̍����̍��W�j
* @param[in]  size  �`�悷�镶���̑傫��
* @param[in]  theta ������̉�]�p�x [degree] ( * ���݂͖��� )
* @param[in]  argsformat, ... ������
* @return     ���ۂɕ`�悵��������̒���
* @section Notes
*  wn �Ŏw�肵���E�B���h�D�ɁC����������W(x,y) ����`���܂��D
*  size �͕����̑傫���ŁC�h�b�g�P�ʂŎw�肵�܂��D
*  theta �͕�����̉�]���w�肷������ł����C���o�[�W�����ł͋@�\���܂���D
*  �������argsformat �ɗ^���܂����C���̈����ȍ~��printf() �֐��̈����Ɠ��l��
*  �t�H�[�}�b�g�ɂȂ��Ă��܂��̂ŁC�g�p��̂悤�ɕϐ��̒l�Ȃǂ�`�������ł��܂��D
*  �����̃T�C�Ysize ��1�`24 �͈̔͂Ŏw��ł��܂��D
*  size �Ǝ��ۂ̃t�H���g�Ƃ̊֌W�͈ȉ��̕\�̂悤�ɂȂ��Ă��܂��D
*  ���̏ꍇ�C�����͔��p�p�����̂ݕ`��ł��܂��D2 �o�C�g����(����)
*  ��`�悷��ꍇ�́Csize �ɂ�FONTSET ���w�肵�܂��D
*  ���̏ꍇ�̃t�H���g�̎w��́Cgsetfontset() �֐��𗘗p���܂��D
*  gsetfontset() �ł̃t�H���g�w�肪�Ȃ��ꍇ�́C�f�t�H���g��14 �h�b�g�̃t�H���g�Z�b�g�ŕ`�悳��܂��D
*/
int drawstr(int wn, double x, double y, int size, double theta, const std::string argsformat, ...)
{
  va_list argptr;              //�ϐ��������X�g
  va_start(argptr, argsformat); //initalize va_ function
  int len = gEggX.drawstr(wn, x, y, size, theta, argsformat.c_str(), argptr);//
  va_end(argptr);              //close     va_ function

  return len;
}

//������
//void gscroll(int wn, int incx, int incy, int clr)
//{
//}

//������
//void gputarea(int wn, double x, double y, int srcwn, int srcly, double srcxs, double srcys, double srcxe, double srcye)
//{
//}

//������
//int gputimage(int wn, double x, double y, unsigned char *buf, int width, int height, int msk)
//{
//}

//������
//unsigned char *ggetimage(int wn, int ly, double xs, double ys, double xe, double ye, int *rwidth, int *rheight)
//{
//}

//������
//int gsaveimage(int wn, int ly, double xs, double ys, double xe, double ye, const char *conv, int nd, const char *argsformat, ...)
//{
//}

//������
//unsigned char *readimage(const char *conv, const char *filename, int *rwidth, int *rheight, int *rmsk)
//{
//}

//������
//int writeimage(const unsigned char *buf, int width, int height, int msk, const char *conv, int nd, const char *argsformat, ...)
//{
//}

/**
* @brief      ggetch()�Cggetevent()�Cggetxpress()�̓��샂�[�h��ݒ肷��
* @ingroup    wineggx
* @param[in]  flag ENABLE �m���u���b�L���O���[�h�CDISABLE �u���b�L���O���[�h
* @retval     �Ȃ�
*/
void gsetnonblock(int flag)
{
  gEggX.gsetnonblock(flag);
}

/**
* @brief      �L�[�{�[�h������͂��ꂽ������Ԃ�
* @ingroup    wineggx
* @param[in]  �Ȃ�
* @retval     <0 ���͂Ȃ��i�m���u���b�L���O���[�h�̏ꍇ�j
* @retval     �����ꂽ�L�[�̕����R�[�h
*/
int ggetch()
{
  return gEggX.ggetch();
}

/**
* @brief      �S�ẴE�B���h�E�̃}�E�X��L�[�{�[�h�̓��͏���Ԃ��idouble�Łj�D
* @ingroup    wineggx
* @param[out]  type �C�x���g�̎��
* @param[out]  button �{�^���̔ԍ��i�}�E�X�̏ꍇ�j�C�L�[�R�[�h�i�L�[�{�[�h�̏ꍇ�j
* @param[out]  x �}�E�X�|�C���^��x���W�i�A�v���P�[�V�������W�n�j
* @param[out]  y �}�E�X�|�C���^��y���W�i�A�v���P�[�V�������W�n�j
* @retval      ���͂̂������E�B���h�E�ԍ�
*/
int ggetevent(int *type, int *button, double *x, double *y)
{
  return gEggX.ggetevent(type, button, x, y);
}

/**
* @brief      �S�ẴE�B���h�E�̃}�E�X��L�[�{�[�h�̓��͏���Ԃ��ifloat�Łj�D
* @ingroup    wineggx
* @param[out]  type �C�x���g�̎��
* @param[out]  button �{�^���̔ԍ��i�}�E�X�̏ꍇ�j�C�L�[�R�[�h�i�L�[�{�[�h�̏ꍇ�j
* @param[out]  x �}�E�X�|�C���^��x���W�i�A�v���P�[�V�������W�n�j
* @param[out]  y �}�E�X�|�C���^��y���W�i�A�v���P�[�V�������W�n�j
* @retval      ���͂̂������E�B���h�E�ԍ�
*/
int ggetevent(int *type, int *button, float *x, float *y)
{
  double xd, yd;
  int ret = gEggX.ggetevent(type, button, &xd, &yd);
  *x = float(xd);
  *y = float(yd);
  return ret;
}

/**
* @brief      �S�ẴE�B���h�E�̃}�E�X��L�[�{�[�h�̓��͏���Ԃ��i�L�[�ƃ{�^���̂݁j�idouble�Łj�D
* @ingroup    wineggx
* @param[out]  type �C�x���g�̎��
* @param[out]  button �{�^���̔ԍ��i�}�E�X�̏ꍇ�j�C�L�[�R�[�h�i�L�[�{�[�h�̏ꍇ�j
* @param[out]  x �}�E�X�|�C���^��x���W�i�A�v���P�[�V�������W�n�j
* @param[out]  y �}�E�X�|�C���^��y���W�i�A�v���P�[�V�������W�n�j
* @retval      ���͂̂������E�B���h�E�ԍ�
*/
int ggetxpress(int *type, int *button, double *x, double *y)
{
  return gEggX.ggetxpress(type, button, x, y);
}

/**
* @brief      �S�ẴE�B���h�E�̃}�E�X��L�[�{�[�h�̓��͏���Ԃ��i�L�[�ƃ{�^���̂݁j�ifloat�Łj�D
* @ingroup    wineggx
* @param[out]  type �C�x���g�̎��
* @param[out]  button �{�^���̔ԍ��i�}�E�X�̏ꍇ�j�C�L�[�R�[�h�i�L�[�{�[�h�̏ꍇ�j
* @param[outr]  x �}�E�X�|�C���^��x���W�i�A�v���P�[�V�������W�n�j
* @param[out]  y �}�E�X�|�C���^��y���W�i�A�v���P�[�V�������W�n�j
* @retval      ���͂̂������E�B���h�E�ԍ�
*/
int ggetxpress(int *type, int *button, float *x, float *y)
{
  double xd, yd;
  int ret = gEggX.ggetxpress(type, button, &xd, &yd);
  *x = float(xd);
  *y = float(yd);
  return ret;
}

/**
* @brief      �~���b�P�ʂŎ��s����������
* @ingroup    wineggx
* @param[in]  msec ��������[msec]
* @retval     �Ȃ�
*/
void msleep(unsigned long msec)
{
  Sleep(msec);
}
