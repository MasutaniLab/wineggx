#include "cselectwindow.h"
using namespace std;

//全て削除する
void CSelectWindow::AllDel()
{
  m_buff.clear();
}

//ウィンドウ検索行列に追加する
void CSelectWindow::Add(HWND hWnd,void *window)
{
  if(!hWnd || !window)return;

  WindBuffer buf;
  buf.hWnd   = hWnd;
  buf.window = window;

  m_buff.push_front(buf);
}

//ウィンドウ検索行列から外す
void CSelectWindow::Delete(HWND hWnd)
{
  if(!hWnd||m_buff.empty())return;

  list<WindBuffer>::iterator itr = m_buff.begin();
  list<WindBuffer>::iterator end = m_buff.end();

  for(;itr!=end;itr++)
  {
      if(itr->hWnd == hWnd)
      {
          m_buff.erase(itr);
          if(m_buff.empty())return;
          itr = m_buff.begin();
          end = m_buff.end();
      }
  }
}


//ウィンドウに対応したポインタを返す
void *CSelectWindow::Serch(HWND hWnd)
{
  if(!hWnd||m_buff.empty())return NULL;

  list<WindBuffer>::iterator itr = m_buff.begin();
  list<WindBuffer>::iterator end = m_buff.end();

  for(;itr!=end;itr++)
  {
      if(itr->hWnd == hWnd)
      {
          return itr->window;
      }
  }
  return NULL;
}