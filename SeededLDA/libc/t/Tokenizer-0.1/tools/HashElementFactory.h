/*							-*- C++ -*-
** HashElementFactory.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 16:43:12 2006 Julien Lemoine
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

#ifndef   	HASHELEMENTFACTORY_H_
# define   	HASHELEMENTFACTORY_H_

#include <list>

namespace ToolBox
{
  // fwd declaration
  template <typename T>
  class HashElement;

  /**
   * @brief construct HashElement per bloc to avoid multiple call to
   * new/delete
   */
  template <typename T>
  class HashElementFactory
    {
    public:
      HashElementFactory(const T &empty, unsigned blocSize = 65536);
      ~HashElementFactory();

    private:
      /// avoid default constructor
      HashElementFactory();
      /// avoid copy constructor
      HashElementFactory(const HashElementFactory &e);
      /// avoid affectation operator
      HashElementFactory& operator=(const HashElementFactory &e);
      
    public:
      void clear();
      HashElement<T>* getNewHashElement();

    private:
      T					_empty;
      unsigned				_blocSize;
      std::list<HashElement<T>*>	_blocs;
      unsigned				_lastBlocPosition;
      HashElement<T>			*_lastBloc;
    };
}

#endif 	    /* !HASHELEMENTFACTORY_H_ */
