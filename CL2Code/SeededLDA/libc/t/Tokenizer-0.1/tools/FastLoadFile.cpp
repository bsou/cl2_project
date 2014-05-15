/*
** FastLoadFile.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sat Jul  1 13:07:21 2006 Julien Lemoine
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <iostream>
#include "FastLoadFile.h"
#include "Exception.h"

namespace ToolBox
{

  const unsigned int FastLoadFileBuffSize = 32768;

  FastLoadFile::FastLoadFile() :
    _is(0x0)
  {
  }

  FastLoadFile::~FastLoadFile()
  {
  }

  std::pair<const char*, unsigned> FastLoadFile::readAlloc()
  {
    std::pair<const char*, unsigned>	res;
    unsigned				total = 0, index, i;
    char				*buff, *dest;
    std::list<std::pair<char*, unsigned> >::iterator it;

    _read(total);
    dest = new char[total];
    res.first = dest;
    res.second = total;
    for (it = _buffs.begin(); it != _buffs.end(); ++it)
      {
	buff = it->first;
	index = it->second;
	for (i = 0; i < index; ++i)
	  {
	    *dest = buff[i];
	    ++dest;
	  }
	delete[] buff;
      }
    _buffs.clear();
    return res; 
  }

  const std::string& FastLoadFile::read()
  {
    std::pair<const char*, unsigned> res;

    unsigned			total = 0, index, i;
    char			*buff;
    std::string::iterator	strit;
    std::list<std::pair<char*, unsigned> >::iterator it;

    _read(total);
    _res.resize(total);
    strit = _res.begin();
    
    for (it = _buffs.begin(); it != _buffs.end(); ++it)
      {
	buff = it->first;
	index = it->second;
	for (i = 0; i < index; ++i)
	  {
	    *strit = buff[i];
	    ++strit;
	  }
	delete[] buff;
      }
    _buffs.clear();
    return _res; 
  }

  void FastLoadFile::closeFile()
  {
    if (!_is || !_is->is_open())
      throw ToolBox::FileException("No opened file", HERE);
    _is->close();
    delete _is;
  }

  void FastLoadFile::openFile(const std::string &file)
  {
    _buffs.clear();
    _is = new std::ifstream(file.c_str(), std::ios::binary);
    if (!_is)
      throw ToolBox::FileException("Could not open " + file, HERE);
    if (!_is->is_open())
      {
	delete _is;
	throw ToolBox::FileException("Could not open " + file, HERE);
      }
  }

  void FastLoadFile::_read(unsigned &total)
  {
    total = 0;
    if (!_is)
      throw ToolBox::Exception("No File opened", HERE);
    _buffs.clear();
    while (_fill_tmp())
      {
	_buffs.push_back(_tmp);
	total += _tmp.second;
      }
  }

  bool FastLoadFile::_fill_tmp()
  {
    _tmp.first = new char[FastLoadFileBuffSize];
    _is->read(_tmp.first, FastLoadFileBuffSize);
    _tmp.second = _is->gcount();
    if (!_tmp.second)
      {
	delete[] _tmp.first;
	return false;
      }
    return true;
  }

}

