/*							-*- C++ -*-
** ITokenizer.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 15:01:12 2006 Julien Lemoine
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

#ifndef   	ITOKENIZER_H_
# define   	ITOKENIZER_H_

#include "Token.h"

namespace Tokenizer
{
  /**
   * Interface that represent a tokenizer (getNextToken method)
   */
  class ITokenizer
  {
  public:
    ITokenizer();
    virtual ~ITokenizer();

  private:
    /// avoid copy constructor
    ITokenizer(const ITokenizer &e);
    /// avoid affectation operator
    ITokenizer& operator=(const ITokenizer &e);

  public:
    /// get next token
    virtual Token getNextToken() const = 0;
  };
}

#endif 	    /* !ITOKENIZER_H_ */
