/*							-*- C++ -*-
** HashElement.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 16:45:11 2006 Julien Lemoine
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

#ifndef   	HASHELEMENT_H_
# define   	HASHELEMENT_H_

namespace ToolBox
{
  /**
   * @brief element of a string hash table (store collisions for a
   * position of table)
   */
  template <typename T>
  class HashElement
    {
    public:
      HashElement(const char *string);
      HashElement();
      ~HashElement();

    private:
      /// avoid copy constructor
      HashElement(const HashElement &e);

    public:
      const char* getString() const;
      HashElement* getNextElement();
      const HashElement* getNextElement() const;
      void setNextElement(HashElement *next);
      void setValue(const T &value);
      const T& getValue() const;

    private:
      const char	*_str;
      HashElement<T>	*_next;
      T			_value;
    };
}

#endif 	    /* !HASHELEMENT_H_ */
