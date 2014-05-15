/*
** HashElementFactory.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 16:44:38 2006 Julien Lemoine
** $Id$
**
** Copyright (C) 2006 Julien Lemoine
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with this program; if not, write to the Free Software
*/

#include "HashElementFactory.h"
#include "HashElement.h"

template <typename T>
ToolBox::HashElementFactory<T>::HashElementFactory(const T &empty, unsigned blocSize) :
  _empty(empty), _blocSize(blocSize), _lastBlocPosition(0), _lastBloc(0x0)
{
  _lastBloc = new HashElement<T>[_blocSize];
}

template <typename T>
ToolBox::HashElementFactory<T>::~HashElementFactory()
{
  typename std::list<HashElement<T>*>::iterator it, ite;
  for (it = _blocs.begin(), ite = _blocs.end(); it != ite; ++it)
    delete[] *it;
  delete[] _lastBloc;
}

template <typename T>
void ToolBox::HashElementFactory<T>::clear()
{
  typename std::list<HashElement<T>*>::iterator it, ite, tmp;
  for (it = _blocs.begin(), ite = _blocs.end(); it != ite; )
    {
      tmp = it;
      ++it;
      delete[] *tmp;
      _blocs.erase(tmp);
    }
  _lastBlocPosition = 0;
}

template <typename T>
ToolBox::HashElement<T>* ToolBox::HashElementFactory<T>::getNewHashElement()
{
  if (_lastBlocPosition == _blocSize)
    {
      _blocs.push_back(_lastBloc);
      _lastBlocPosition = 0;
      _lastBloc = new HashElement<T>[_blocSize];
    }
  HashElement<T> el;
  el.setValue(_empty);
  _lastBloc[_lastBlocPosition] = el;
  ++_lastBlocPosition;
  return &_lastBloc[_lastBlocPosition - 1];
}
