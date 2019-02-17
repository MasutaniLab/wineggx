/**
* @file cselectwindow.h
* @brief Windows”ÅEGGX‚Ì— •û
* @par Copyright
*  Copyright (C) 2007-2019 Team ODENS, Masutani Lab, Osaka Electro-Communication University
* @par —š—ğ
* -2019/02/XX ¡’J •Û” Version 2.0
* -2014/10/16 ¡’J •Û”uWindows”ÅEGGX Version 1.0v‚ğ–¼æ‚é
* -2014/03/12 ¡’J •Û”
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