/**
 * @file wineggx.h
 * @brief Windows��EGGX
 * @section Notes
 *  Copyright (C) 2007-2014 Team ODENS \n\n
 * @date �X�V���� �X�V��\n
 * 2014/10/16 ���J �۔��uWindows��EGGX Version 1.0�v�𖼏��\n
 * 2014/03/12 ���J �۔�\n
**/

/**
 * @addtogroup wineggx wineggx-WinEggX
 * @brief Windows��eggx���C�u�����̑��������
**/

#include <string>

#define PENDOWN (0) ///< �y�������낷
#define PENUP   (1) ///< �y�����グ��
#define PSET    (2) ///< ���̏�ɓ_��ł�

#define LineSolid (0)
#define LineOnOffDash (1)

#define DISABLE (0)
#define ENABLE  (-1)

#define KeyPress (2)
#define ButtonPress (4)
#define MotionNotify (6)
#define EnterNotify (7)
#define LeaveNotify (8)

/**
 * @brief      �C�ӂ̃T�C�Y�̃O���t�B�b�N�X��ʂ��J��
 * @ingroup    wineggx
 * @param[in]  xsize �E�B���h�E�̉���
 * @param[in]  ysize �E�B���h�E�̏c��
 * @return     �Ԃ�l�ɂ�EGGX�Ŏg�p����C�E�B���h�E�ԍ����Ԃ��Ă��܂��D
 * @retval     -1 �G���[
*/
int gopen(int xsize,int ysize);

/**
 * @brief      �O���t�B�b�N�X�E�B���h�E�����
 * @ingroup    wineggx
 * @param[in]  wn �E�B���h�E�ԍ�
*/
void gclose( int wn );

/**
 * @brief      ���ׂẴO���t�B�b�N�X�p�E�B���h�D�����
 * @ingroup    wineggx
*/
void gcloseall( void );

/**
 * @brief      �E�B���h�D�̃^�C�g����ύX����
 * @ingroup    wineggx
 * @param[in]  wn �^�C�g����ύX����E�B���h�E�̔ԍ�
 * @param[in]  ... �ݒ肷��E�B���h�E�̕�����
 * @return     �ݒ肵���E�B���h�D�^�C�g���̕�����̒���
*/
int winname( int wn, const char *argsformat, ... );

/**
* @brief      �E�B���h�D�̃^�C�g����ύX����
* @ingroup    wineggx
* @param[in]  wn �^�C�g����ύX����E�B���h�E�̔ԍ�
* @param[in]  ... �ݒ肷��E�B���h�E�̕�����
* @return     �ݒ肵���E�B���h�D�^�C�g���̕�����̒���
*/
int winname(int wn, const std::string argsformat, ...);

/**
 * @brief      ���W�n�̕ύX
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  xs ������X���W
 * @param[in]  ys ������Y���W
 * @param[in]  xe �E���X���W
 * @param[in]  ye �E���Y���W
*/
void window( int wn, double xs, double ys, double xe, double ye );

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
void layer( int wn, int lys, int lyw );

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
void copylayer( int wn, int lysrc, int lydest );

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
void newcolor(int wn, const char *argsformat, ...);

/**
* @brief      �`��F�̕ύX�istring�Łj
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �F��\��������
*  wn �Ŏw�肳�ꂽ�E�B���h�D�ł̕`��F��ύX���܂��Dargsformat(�Ƃ���ɑ�������)�Ŏw�肳��镶�����
*  �w�i�F�ɐݒ肵�܂��D2�߂̈���argsformat�ȍ~�́CC �W���֐���printf() �֐��̏ꍇ�Ɠ��l�̉ψ���
*  �ƂȂ��Ă��܂��D���̔w�i�F�̕�����ɂ́CX�E�B���h�E�Œ�`����Ă���F���C"#c0c0ff"�̂悤�ɁC16�i��
*  ��Red,Green,Blue ���w�肵�܂��D
*/
void newcolor(int wn, const std::string argsformat, ...);

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
void newrgbcolor( int wn, int r, int g, int b );

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
void newhsvcolor( int wn, int h, int s, int v );

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
void gsetbgcolor(int wn, const char *argsformat, ...);

/**
* @brief      �E�B���h�D�̔w�i�F�̕ύX�istring�Łj
* @ingroup    wineggx
* @param[in]  wn �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �F��\��������
*  wn �Ŏw�肳�ꂽ�E�B���h�D�̔w�i�F��ύX���܂��Dargsformat(�Ƃ���ɑ�������)�Ŏw�肳��镶�����
*  �w�i�F�ɐݒ肵�܂��D2�߂̈���argsformat�ȍ~�́CC �W���֐���printf() �֐��̏ꍇ�Ɠ��l�̉ψ���
*  �ƂȂ��Ă��܂��D���̔w�i�F�̕�����ɂ́CX�E�B���h�E�Œ�`����Ă���F���C"#c0c0ff"�̂悤�ɁC16�i��
*  ��Red,Green,Blue ���w�肵�܂��D
*/
void gsetbgcolor(int wn, const std::string argsformat, ...);

/**
 * @brief      �E�B���h�D�̔w�i�F�̕ύX�y���ӁF�I���W�i���ɂ͂Ȃ��֐��z
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  r  �ԐF ( 0 to 255 )
 * @param[in]  g  �ΐF ( 0 to 255 )
 * @param[in]  b  �F ( 0 to 255 )
 * @section Notes
 *  wn �Ŏw�肳�ꂽ�E�B���h�D�̔w�i�F��ύX���܂��Dr,g,b �ɂ͂��ꂼ��Red,Green,Blue �̋P�x��
 *  256 �i�K�̐���(0�`255) �Ŏw�肵�܂��D
*/
void gsetbgcolorrgb( int wn, int r, int g, int b );

/**
 * @brief      �`�惌�C���̑S����
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�̕`�惌�C����w�i�F�Ɏw�肳�ꂽ�F�ŏ��������܂��D
*/
void gclr( int wn );

/**
 * @brief      �����̕ύX
 * @ingroup    wineggx
 * @param[in]  wn �ύX����E�B���h�E�̔ԍ�
 * @param[in]  width ����
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�Ő���`�����̐�����ύX���܂��D
 *  �f�t�H���g�ł͕�1 ���ݒ肳��Ă��܂��D
*/
void newlinewidth( int wn, int width );

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
void newlinestyle( int wn, int style );

/**
 * @brief      �_�̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y ���W
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɓ_��`�悵�܂��D
*/
void pset( int wn,double x,double y );


/**
 * @brief      �����̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x0,y0 �[�_�̍��W
 * @param[in]  x1,y1 �[�_�̍��W
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D��(x0, y0) ����(x1, y1) �ɒ�����`�悵�܂��D
*/
void drawline( int wn, double x0, double y0, double x1, double y1 );

/**
 * @brief      ������`���߂̏����ʒu�̐ݒ�
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y �`�����W
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɁClineto() �֐��̂��߂̏����ʒu��(x, y) �ɐݒ肵�܂��D
*/
void moveto( int wn, double x, double y );

/**
 * @brief      �A���I�ɒ�����`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y ���W
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɁC�ȑOmoveto()�܂���lineto() ���Ă΂ꂽ���Ɏw�肳�ꂽ���W����
 *  (x, y) �֒����������܂��D
*/
void lineto( int wn, double x, double y );

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
void line( int wn,double x,double y,int mode );

/**
 * @brief      �~�̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  xcen,ycen �~�̒��S���W
 * @param[in]  xrad X�������̔��a
 * @param[in]  yrad Y�������̔��a
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�E�ɁC(xcen,ycen) �𒆐S��
 *  �������̔��axrad�C�c�����̔��ayrad �̉~��`���܂��D
*/
void circle( int wn, double xcen, double ycen, double xrad, double yrad );

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
void fillcirc( int wn, double xcen, double ycen, double xrad, double yrad );

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
void drawarc( int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir );

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
void fillarc( int wn, double xcen, double ycen, double xrad, double yrad, double sang, double eang, int idir );

/**
 * @brief      �����`�̕`��
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y  �`����W�i�����`�̍����j
 * @param[in]  w    �����`�̉���
 * @param[in]  h    �����`�̏c��
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɁC���_(x,y) ���琳�̕����ɕ�w�C
 *  ����h �̒����`��`���܂��D
*/
void drawrect( int wn, double x, double y, double w, double h );

/**
 * @brief      �����`�̕`��i�h��Ԃ��j
 * @ingroup    wineggx
 * @param[in]  wn �`�悷��E�B���h�E�̔ԍ�
 * @param[in]  x,y  �`����W�i�����`�̍����j
 * @param[in]  w    �����`�̉���
 * @param[in]  h    �����`�̏c��
 * @section Notes
 *  wn �Ŏw�肵���E�B���h�D�ɁC���_(x,y) ���琳�̕����ɕ�w�C
 *  ����h �̒����`��`���܂��D
*/
void fillrect( int wn, double x, double y, double w, double h );

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
int drawstr(int wn, double x, double y, int size, double theta, const char *argsformat, ...);

/**
* @brief      ������̕`��istring�Łj
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
int drawstr(int wn, double x, double y, int size, double theta, const std::string argsformat, ...);

/**
* @brief      �`��t�H���g�̐ݒ�
* @ingroup    wineggx
* @param[in]  wn    �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �t�H���g�Z�b�g�̖��O \n �k���������܂߂�32Byte�ȉ��łȂ���΂Ȃ�Ȃ��D
* @retval     -1 �G���[
* @retval     0  �擾����
* @retval     1  ��փt�H���g�̎擾����
*/
int gsetfontset(int wn, const char *argsformat, ...);

/**
* @brief      �`��t�H���g�̐ݒ�istring�Łj
* @ingroup    wineggx
* @param[in]  wn    �ύX����E�B���h�E�̔ԍ�
* @param[in]  argsformat, ... �t�H���g�Z�b�g�̖��O \n �k���������܂߂�32Byte�ȉ��łȂ���΂Ȃ�Ȃ��D
* @retval     -1 �G���[
* @retval     0  �擾����
* @retval     1  ��փt�H���g�̎擾����
*/
int gsetfontset(int wn, const std::string argsformat, ...);

/**
 * @brief      �L�[�{�[�h������͂��ꂽ������Ԃ�
 * @ingroup    wineggx
 * @param[in]  �Ȃ�
 * @retval     <0 ���͂Ȃ��i�m���u���b�L���O���[�h�̏ꍇ�j
 * @retval     �����ꂽ�L�[�̕����R�[�h
*/
int ggetch();

/**
 * @brief      ggetch()�̓��샂�[�h��ݒ肷��
 * @ingroup    wineggx
 * @param[in]  flag ENABLE �m���u���b�L���O���[�h�CDISABLE �u���b�L���O���[�h
 * @retval     �Ȃ�
*/
void gsetnonblock( int flag );

/**
 * @brief      �~���b�P�ʂŎ��s����������
 * @ingroup    wineggx
 * @param[in]  msec ��������[msec]
 * @retval     �Ȃ�
*/
void msleep( unsigned long msec );

/**
* @brief      �S�ẴE�B���h�E�̃}�E�X��L�[�{�[�h�̓��͏���Ԃ��D
* @ingroup    wineggx
* @param[out]  type �C�x���g�̎��
* @param[out]  button �{�^���̔ԍ��i�}�E�X�̏ꍇ�j�C�L�[�R�[�h�i�L�[�{�[�h�̏ꍇ�j
* @param[out]  x �}�E�X�|�C���^��x���W�i�A�v���P�[�V�������W�n�j
* @param[out]  y �}�E�X�|�C���^��y���W�i�A�v���P�[�V�������W�n�j
* @retval      ���͂̂������E�B���h�E�ԍ�
*/
int ggetevent(int *type, int *button, double *x, double *y);

/**
* @brief      �S�ẴE�B���h�E�̃}�E�X��L�[�{�[�h�̓��͏���Ԃ��i�L�[�ƃ{�^���̂݁j�D
* @ingroup    wineggx
* @param[out]  type �C�x���g�̎��
* @param[out]  button �{�^���̔ԍ��i�}�E�X�̏ꍇ�j�C�L�[�R�[�h�i�L�[�{�[�h�̏ꍇ�j
* @param[out]  x �}�E�X�|�C���^��x���W�i�A�v���P�[�V�������W�n�j
* @param[out]  y �}�E�X�|�C���^��y���W�i�A�v���P�[�V�������W�n�j
* @retval      ���͂̂������E�B���h�E�ԍ�
*/
int ggetxpress(int *type, int *button, double *x, double *y);