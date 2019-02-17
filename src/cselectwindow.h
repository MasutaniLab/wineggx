/**
* @file cselectwindow.h
* @brief Windows��EGGX�̗���
* @par Copyright
*  Copyright (C) 2007-2019 Team ODENS, Masutani Lab, Osaka Electro-Communication University
* @par ����
* -2019/02/XX ���J �۔� Version 2.0
* -2014/10/16 ���J �۔��uWindows��EGGX Version 1.0�v�𖼏��
* -2014/03/12 ���J �۔�
**/

#include <windows.h>
#include <list>

class CSelectWindow
{
public:
  void AllDel();
  void Add(HWND hWnd,void *window);
  void Delete(HWND hWnd);
  void *Serch(HWND hWnd);

private:
  struct WindBuffer
  {
      HWND  hWnd;
      void *window;
  };
  std::list<WindBuffer> m_buff;
};