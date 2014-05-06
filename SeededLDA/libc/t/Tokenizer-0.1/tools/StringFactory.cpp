/*
** StringFactory.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 16:48:49 2006 Julien Lemoine
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

#include "StringFactory.h"

ToolBox::StringFactory::StringFactory(unsigned blocSize) :
  _blocSize(blocSize), _lastBlocPosition(0), _lastBloc(0x0)
{
  _lastBloc = new char[_blocSize];
}

ToolBox::StringFactory::~StringFactory()
{
  delete[] _lastBloc;
  std::list<char*>::iterator it, ite;
  for (it = _blocs.begin(), ite = _blocs.end(); it != ite; ++it)
    delete[] *it;
}

const char* ToolBox::StringFactory::allocateString(const char *str, unsigned strLen)
{
  unsigned nbFree = _blocSize - _lastBlocPosition;
  if (nbFree < strLen + 1)
    { // allocate new bloc
      if (strLen + 1 > _blocSize)
	_blocSize = strLen + 1;
      _blocs.push_back(_lastBloc);
      _lastBloc = new char[_blocSize];
      _lastBlocPosition = 0;
    }
  const char *res = _lastBloc + _lastBlocPosition;
  for (unsigned int i = 0; i < strLen; ++i)
    _lastBloc[i + _lastBlocPosition] = str[i];
  _lastBloc[strLen + _lastBlocPosition] = 0;
  _lastBlocPosition += strLen + 1;
  return res;
}

const char* ToolBox::StringFactory::allocateString(const std::string &str)
{
  return allocateString(str.c_str(), str.size());
}

void ToolBox::StringFactory::clear()
{
  std::list<char*>::iterator it, tmp, ite;
  for (it = _blocs.begin(), ite = _blocs.end(); it != ite; )
    {
      delete[] *it;
      tmp = it;
      ++it;
      _blocs.erase(tmp);
    }
  _lastBlocPosition = 0;
}
