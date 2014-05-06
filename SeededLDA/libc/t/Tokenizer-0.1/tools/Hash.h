/*							-*- C++ -*-
** Hash.h
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

#ifndef   	HASH_H_
# define   	HASH_H_

#include <vector>

namespace ToolBox
{
  class StringFactory;
  template <typename T>
  class HashElementFactory;
  template <typename T>
  class HashElement;

  /**
   * @brief implementation of a hash table. Use the same interface
   * than trie
   */
  template <typename T>
  class Hash
    {
    public:
      Hash(const T &empty, unsigned nbCellInTable = 1000081);
      ~Hash();

    private:
      /// avoid default constructor
      Hash();
      /// avoid copy constructor
      Hash(const Hash &e);
      /// avoid affectation operator
      Hash& operator=(const Hash &e);

    public:
      /// add an entry in the Trie, if entry already exist an exception
      /// is throw
      void addEntry(const char *str, unsigned strLen, const T &value);
      /// associates a value to a string, if string is already in Trie,
      /// value is overwriten
      void setEntry(const char *str, unsigned strLen, const T &value);
      /// get an entry in the Trie
      const T& getEntry(const char *str, unsigned strLen) const;
      /// clear the content of trie
      void clear();

    private:
      /**
       * implementation of hash function to have an index in _table
       * vector
       */
      unsigned _getIndex(const char *str, unsigned strLen) const;
      /// add a new HashElement in index position of _table
      void _addIndex(unsigned index, const char *str, unsigned strLen, const T &value);

    private:
      T					_empty;
      /// hash table
      std::vector<HashElement<T>*>	_table;
      /// hash element factory
      HashElementFactory<T>		*_factory;
      /// string factory
      StringFactory			*_stringFactory;
    };
}

#endif 	    /* !HASH_H_ */
