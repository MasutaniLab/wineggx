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