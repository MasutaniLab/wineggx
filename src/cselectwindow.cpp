/**
* @file cselectwindow.cpp
* @brief Windows��EGGX�̗���
* @par Copyright
*  Copyright (C) 2007-2019 Team ODENS, Masutani Lab, Osaka Electro-Communication University
* @par ����
* -2019/02/XX ���J �۔� Version 2.0
* -2014/10/16 ���J �۔��uWindows��EGGX Version 1.0�v�𖼏��
* -2014/03/12 ���J �۔�
**/

#include "cselectwindow.h"
using namespace std;

//�S�č폜����
void CSelectWindow::AllDel()
{
  m_buff.clear();
}

//�E�B���h�E�����s��ɒǉ�����
void CSelectWindow::Add(HWND hWnd,void *window)
{
  if(!hWnd || !window)return;

  WindBuffer buf;
  buf.hWnd   = hWnd;
  buf.window = window;

  m_buff.push_front(buf);
}

//�E�B���h�E�����s�񂩂�O��
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


//�E�B���h�E�ɑΉ������|�C���^��Ԃ�
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