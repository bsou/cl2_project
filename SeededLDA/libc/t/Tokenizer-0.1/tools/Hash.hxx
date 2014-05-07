/*
** Hash.hxx
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 16:40:12 2006 Julien Lemoine
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

#ifndef   	HASH_HXX_
# define   	HASH_HXX_

#include "Hash.h"
#include "HashElement.hxx"
#include "HashElementFactory.hxx"
#include "StringFactory.h"
#include "Exception.h"

template <typename T>
ToolBox::Hash<T>::Hash(const T &empty, unsigned nbCellInTable) :
  _empty(empty), _factory(0x0), _stringFactory(0x0)
{
  _table.resize(nbCellInTable, 0x0);
  _factory = new HashElementFactory<T>(empty);
  _stringFactory = new StringFactory(1024000);
}

template <typename T>
ToolBox::Hash<T>::~Hash()
{
  delete _factory;
  delete _stringFactory;
}

template <typename T>
void ToolBox::Hash<T>::addEntry(const char *str, unsigned strLen, const T &value)
{
  unsigned index = _getIndex(str, strLen);
  HashElement<T> *it = _table[index];
  while (it != 0x0)
    {
      if (strcmp(it->getString(), str) == 0x0)
	throw ToolBox::Exception("The word is already in hash table", HERE);
      it = it->getNextElement();
    }
  _addIndex(index, str, strLen, value);
}

template <typename T>
void ToolBox::Hash<T>::setEntry(const char *str, unsigned strLen, const T &value)
{
  unsigned index = _getIndex(str, strLen);
  HashElement<T> *it = _table[index];
  while (it != 0x0)
    {
      if (strcmp(it->getString(), str) == 0x0)
	{
	  it->setValue(value);
	  return;
	}
      it = it->getNextElement();
    }
  _addIndex(index, str, strLen, value);
}

template <typename T>
const T& ToolBox::Hash<T>::getEntry(const char *str, unsigned strLen) const
{
  unsigned index = _getIndex(str, strLen);
  HashElement<T> *it = _table[index];
  unsigned int i = 0;
  while (it != 0x0)
    {
      ++i;
      if (strcmp(it->getString(), str) == 0x0)
	return it->getValue();
      it = it->getNextElement();
    }
  return _empty;
}

template <typename T>
void ToolBox::Hash<T>::clear()
{
  _factory->clear();
  _stringFactory->clear();
  typename std::vector<HashElement<T>*>::iterator it, ite;
  for (it = _table.begin(), ite = _table.end(); it != ite; ++it)
    *it = 0x0;
}

template <typename T>
void ToolBox::Hash<T>::_addIndex(unsigned index, const char *str, unsigned strLen, const T &value)
{
  HashElement<T> *newEl = _factory->getNewHashElement();
  const char *newStr = _stringFactory->allocateString(str, strLen);
  HashElement<T> el(newStr);
  el.setValue(value);
  el.setNextElement(_table[index]);
  *newEl = el;
  _table[index] = newEl;
}

template <typename T>
unsigned ToolBox::Hash<T>::_getIndex(const char *str, unsigned strLen) const
{
  unsigned val = 0;
  for (unsigned int i = 0; i < strLen; ++i)
    val = (val << 5) + (unsigned char)str[i];
  return val % _table.size();
}

#endif	    /* !HASH_HXX_ */
