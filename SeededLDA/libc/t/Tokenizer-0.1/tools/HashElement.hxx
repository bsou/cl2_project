/*
** HashElement.hxx
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 16:41:07 2006 Julien Lemoine
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

#include "HashElement.h"

template <typename T>
ToolBox::HashElement<T>::HashElement(const char *string) :
  _str(string), _next(0x0)
{
}

template <typename T>
ToolBox::HashElement<T>::HashElement() :
  _str(0x0), _next(0x0)
{
}

template <typename T>
ToolBox::HashElement<T>::~HashElement()
{
  // Do not free anything. We are using factory
}

template <typename T>
const char* ToolBox::HashElement<T>::getString() const
{
  return _str;
}
 
template <typename T>
ToolBox::HashElement<T>* ToolBox::HashElement<T>::getNextElement()
{
  return _next;
}

template <typename T>
const ToolBox::HashElement<T>* ToolBox::HashElement<T>::getNextElement() const
{
  return _next;
}

template <typename T>
void ToolBox::HashElement<T>::setNextElement(HashElement *next)
{
  _next = next;
}

template <typename T>
void ToolBox::HashElement<T>::setValue(const T &value)
{
  _value = value;
}

template <typename T>
const T& ToolBox::HashElement<T>::getValue() const
{
  return _value;
}
