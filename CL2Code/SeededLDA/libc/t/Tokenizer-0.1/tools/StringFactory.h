/*							-*- C++ -*-
** StringFactory.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 16:47:31 2006 Julien Lemoine
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

#ifndef   	STRINGFACTORY_H_
# define   	STRINGFACTORY_H_

#include <list>
#include <string>

namespace ToolBox
{
  class StringFactory
    {
    public:
      StringFactory(unsigned blocSize);
      ~StringFactory();

    private:
      /// avoid default constructor
      StringFactory();
      /// avoid copy constructor
      StringFactory(const StringFactory &e);
      /// avoid affectation operator
      StringFactory& operator=(const StringFactory &e);

    public:
      const char* allocateString(const char *str, unsigned strLen);
      const char* allocateString(const std::string &str);
      void clear();

    private:
      unsigned		_blocSize;
      std::list<char *>	_blocs;
      unsigned		_lastBlocPosition;
      char		*_lastBloc;
    };
}

#endif 	    /* !STRINGFACTORY_H_ */
