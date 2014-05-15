/*
** FastGetLine.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Thu Jun  1 20:32:07 2006 Julien Lemoine
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
#include "FastGetLine.h"
#include "Exception.h"

namespace ToolBox
{
  const unsigned FastGetLineBuffSize = 32768;

  FastGetLine::FastGetLine() :
    _end(false), _buff(0x0), _is(0x0),
    _size (0x0), _buff_pos(0), _available(0)
  {
    _buff = new char[FastGetLineBuffSize];
  }

  FastGetLine::~FastGetLine()
  {
    delete[] _buff;
  }

  void FastGetLine::closeFile()
  {
    if (!_is || !_is->is_open())
      throw ToolBox::FileException("No opened file", HERE);
    _is->close();
    delete _is;
  }

  void FastGetLine::openFile(const std::string &file)
  {
    _is = new std::ifstream(file.c_str(), std::ios::binary);
    if (!_is)
      throw ToolBox::FileException("Could not open " + file, HERE);
    if (!_is->is_open())
      {
	delete _is;
	throw ToolBox::FileException("Could not open " + file, HERE);
      }
    _buff_pos = 0;
    _size = 0;
    _available = 0;
    _end = false;
  }
  
  bool FastGetLine::computeNextLine()
  {
    unsigned int i;
    bool found = false;
    unsigned pos = FastGetLineBuffSize + 1;
    unsigned endbuff;

    if (!_is || !_is->is_open())
      throw ToolBox::FileException("No opened file", HERE);
    //skip old line
    _buff_pos += _size;
    _available -= _size;
    // Skip \n and \r and looking for separator
    endbuff = _available;
    for (i = _buff_pos; pos > FastGetLineBuffSize && i < endbuff; ++i)
      if (_buff[i] != '\n' && _buff[i] != '\r')
	{
	  if (!found)
	    {
	      found = true;
	      _available -= (i - _buff_pos);
	      _buff_pos = i;
	    }
	}
      else if (found)
	pos = i;
    if (pos > FastGetLineBuffSize)
      {
	_get_buffer();
	found = false;
	pos = FastGetLineBuffSize + 1;
	// Skip \n and \r and looking for separator
	endbuff = _available;
	for (i = 0; pos > FastGetLineBuffSize && i < endbuff; ++i)
	  if (_buff[i] != '\n' && _buff[i] != '\r')
	    {
	      if (!found)
		{
		  found = true;
		  _available -= i;
		  _buff_pos = i;
		}
	    }
	  else if (found)
	    pos = i;
	if (pos > FastGetLineBuffSize)
	  {
	    endbuff = _available;
	    for (i = _buff_pos; i < endbuff && (_buff[i] == '\r' ||
						_buff[i] == '\n'); ++i)
	      {
		--_available; // skip in front of buffer ...
		++_buff_pos;
	      }
	    _size = _available;
	    return !_end;
	  }
      }
    _size = pos - _buff_pos;
    return true;
  }

  const char* FastGetLine::getLineStart() const
  {
    return _buff + _buff_pos;
  }

  unsigned FastGetLine::getLineSize() const
  {
    return _size;
  }

  void FastGetLine::_get_buffer()
  {
    unsigned int i;

    // First of all : keep old datas...
    for (i = 0; i < _available; ++i)
      _buff[i] = _buff[_buff_pos + i];
    _buff_pos = 0;
    _is->read(_buff + _available, FastGetLineBuffSize - _available);
    _available += _is->gcount();
    if (!_is->gcount())
      _end = true;
  }

}
