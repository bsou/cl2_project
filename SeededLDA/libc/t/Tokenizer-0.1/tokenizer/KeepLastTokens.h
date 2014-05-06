/*							-*- C++ -*-
** KeepLastTokens.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 15:02:08 2006 Julien Lemoine
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

#ifndef   	KEEPLASTTOKENS_H_
# define   	KEEPLASTTOKENS_H_

#include <vector>
#include "Token.h"

namespace Tokenizer
{
  /*
   * @brief iterator to iterate over a KeepLastTokens pool of tokens
   */
  class KeepLastTokensIterator
  {
  public:
    KeepLastTokensIterator(const std::vector<Token> &pool, unsigned position);
    ~KeepLastTokensIterator();

  public:
    KeepLastTokensIterator& operator++();
    const Token& operator*() const;
    const Token* operator->() const;
    bool operator!=(const KeepLastTokensIterator &other) const;
    bool operator==(const KeepLastTokensIterator &other) const;

  private:
    const std::vector<Token>	*_pool;
    unsigned					_position;
  };

  /*
   * @brief keep the last N element (tokens). Interface is the same as
   * std::priority_queue but the behavior is not the same, there is no
   * comparaison between token here.
   */
  class KeepLastTokens
  {
  public:
    KeepLastTokens(unsigned NGramSize);
    ~KeepLastTokens();

  private:
    /// avoid default constructor
    KeepLastTokens();
    /// avoid copy constructor
    KeepLastTokens(const KeepLastTokens &e);
    /// avoid affectation operator
    KeepLastTokens& operator=(const KeepLastTokens &e);

  public:
    /// return true if pool is empty
    bool empty() const;
    /// return true if the pool is full
    bool full() const;
    /// remove older token of pool
    void pop();
    /// add a new token at end
    void push(const Token&);
    /// return older token
    const Token& top() const;

    /// iterator on the first element
    KeepLastTokensIterator begin();
    /// iterator after the last element
    KeepLastTokensIterator end();

    /// reset all existing tokens
    void clear();

  private:
    /// pool of token
    std::vector<Token>		_pool;
    /// position of older token
    unsigned					_olderToken;
    /// number of token in pool
    unsigned					_size;
  };
}

#endif 	    /* !KEEPLASTTOKENS_H_ */
