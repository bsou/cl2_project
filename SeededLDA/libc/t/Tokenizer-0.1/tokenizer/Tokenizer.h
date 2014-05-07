/*							-*- C++ -*-
** Tokenizer.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 15:08:13 2006 Julien Lemoine
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

#ifndef   	TOKENIZER_H_
# define   	TOKENIZER_H_

#include <vector>
#include "ITokenizer.h"

namespace Tokenizer
{
  class KeepLastTokens;

  /**
   * Tokenizer implementation, skip urls for simple test and can also
   * skip wiki synthax. You can give a minimum size for words (all
   * single words with less caracter that this integer will be skiped)
   */
  class Tokenizer : public ITokenizer 
  {
  public:
    Tokenizer(unsigned NgramSize, unsigned minWordSize = 1);
    ~Tokenizer();
	  
  private:
    /// avoid default constructor
    Tokenizer();
    /// avoid copy constructor
    Tokenizer(const Tokenizer &e);
    /// avoid affectation operator
    Tokenizer& operator=(const Tokenizer &e);
	  
  public:
    /// get next token
    Token getNextToken() const;
    /// set string to analyze (wikiSynthax set to true if string
    /// contains wiki tags)
    void setBufferToAnalyze(const char *str, unsigned strSize, bool wikiSynthax = false);
    /// return true if end of buffer is reach
    bool eof() const;

  protected:
    /// return true if chr is a separator
    bool _isSeparator(const unsigned char chr) const;
    /// skip spaces in buffer (update _position) also skip wikipedia
    /// link
    void _skipSpaces() const;
    /// skip wiki special tags when _bufferContainsWiki is true
    bool _checkStartWiki() const;
    /// get next word in buffer
    Token _getNextWord() const;

  private:
    /// minimum number of chars per word
    unsigned				_minWordSize;
    /// vector for direct access of separators
    std::vector<char>			_separators;
    /// pool containing last tokens
    KeepLastTokens			*_pool;
    /// buffer to parse
    const char				*_buffer;
    /// size of buffer
    unsigned				_bufferSize;
    /// current position in buffer
    mutable unsigned			_position;
    /// buffer that store string (for user)
    char				*_resBuffer;
    /// set to true if current buffer contains wiki synthax
    bool				_bufferContainsWikiSynthax;
  };
}


#endif 	    /* !TOKENIZER_H_ */
