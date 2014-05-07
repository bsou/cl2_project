/*
** Tokenizer.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 15:09:41 2006 Julien Lemoine
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

#include "Tokenizer.h"
#include <string>
#include <assert.h>
#include <iostream>
#include "KeepLastTokens.h"
#include "tools/Exception.h"

enum e_TokenizerConstants { TokenizerMaxStringSize = 65535 };

Tokenizer::Tokenizer::Tokenizer(unsigned NgramSize, unsigned minWordSize) :
  _minWordSize(minWordSize), _pool(0x0), _buffer(0x0), _bufferSize(0), _position(0),
  _resBuffer(0x0), _bufferContainsWikiSynthax(false)
{
  std::string separators(" \t\r\v\f\n¶Þ§þ-,:;!?.\\~{}'\"%#=^¿()/<>$&*+@[]|_´`»«");
  _pool = new KeepLastTokens(NgramSize);
  _separators.resize(256, 0);
  for (unsigned int i = 0; i < separators.size(); ++i)
    _separators[(unsigned char)separators[i]] = 1;
  _resBuffer = new char[TokenizerMaxStringSize];
}

Tokenizer::Tokenizer::Tokenizer::~Tokenizer()
{
  delete _pool;
  delete[] _resBuffer;
}

bool Tokenizer::Tokenizer::eof() const
{
  _skipSpaces();
  return _position >= _bufferSize;
}

Tokenizer::Token Tokenizer::Tokenizer::getNextToken() const
{
  while (!_pool->full())
    {
      if (eof())
		throw ToolBox::EOFException("End of Buffer", HERE);
      Token t = _getNextWord();
      while (t.getSize() < _minWordSize)
		t = _getNextWord();
      _pool->push(t);
    }
  unsigned index = 0, i;
  KeepLastTokensIterator beginIt = _pool->begin();
  KeepLastTokensIterator endIt = _pool->end();

  while (beginIt != endIt)
    {
      const char *str = beginIt->getContent();
      unsigned strSize = beginIt->getSize();
      for (i = 0; index < TokenizerMaxStringSize && i < strSize; ++i, ++index)
		_resBuffer[index] = str[i];
      ++beginIt;
      if (beginIt != endIt && index < TokenizerMaxStringSize)
		{
		  _resBuffer[index] = ' ';
		  ++index;
		}
    }
  _pool->pop();
  return Token(_resBuffer, index);
}

inline Tokenizer::Token Tokenizer::Tokenizer::_getNextWord() const
{
  assert(_buffer != 0x0);
  _skipSpaces();
  unsigned wordStart = _position;
  while (_position < _bufferSize && !_isSeparator(_buffer[_position]))
	++_position;
  if (_position == wordStart)
    throw ToolBox::EOFException("End of Buffer", HERE);
  return Token(_buffer + wordStart, _position - wordStart);
}

void Tokenizer::Tokenizer::setBufferToAnalyze(const char *str, unsigned strSize, bool wikiSynthax)
{
  _pool->clear();
  _position = 0;
  _buffer = str;
  _bufferSize = strSize;
  _bufferContainsWikiSynthax = wikiSynthax;
}

inline void Tokenizer::Tokenizer::_skipSpaces() const
{
  while (_position < _bufferSize && (_checkStartWiki() || _isSeparator(_buffer[_position])))
    ++_position;
}

inline bool Tokenizer::Tokenizer::_checkStartWiki() const
{
  // Skip urls
  if (_buffer[_position] == 'h')
    if (_position + 7 < _bufferSize && strncmp(_buffer + _position, "http://", 7) == 0)
      {
		while (!isspace(_buffer[_position + 1]))
		  ++_position;
		return true;
      }

  if (_bufferContainsWikiSynthax == false)
    return false;

  // Switch is here to boost a little performance
  switch (_buffer[_position])
    {
    case '\342': // skip special sequences like \342\224\202
      if (_position + 2 < _bufferSize)
		{
		  _position += 2;
		  return true;
		}
      break;
    case '&': // we are on a special tag, &lt; &gt; &nbsp;
	  {
		// Looking if we are in the case &lt; ...
		unsigned p = _position;
		while (!isspace(_buffer[_position]))
		  ++_position;
		if (_buffer[_position - 1] == ';')
		  return true;
		_position = p;
	  }
    case '[':
      // Skip content of [[...]]
      if (_position + 1 < _bufferSize && strncmp(_buffer + _position, "[[", 2) == 0)
		{
		  while (_position + 1 < _bufferSize && strncmp(_buffer + _position, "]]", 2) != 0)
			++_position;
		  ++_position;
		  return true;
		}
      break;
    case '{':
      if (_position + 1 < _bufferSize)
		switch (_buffer[_position + 1])
		  {
		  case '|': // Skip content of {|...|}
			{
			  unsigned cnt = 1;
			  _position += 2;
			  while (_position + 1 < _bufferSize && cnt != 0)
				{
				  if (strncmp(_buffer + _position, "{|", 2) == 0)
					++cnt;
				  else if (strncmp(_buffer + _position, "|}", 2) == 0)
					--cnt;
				  ++_position;
				}
			  return true;
			}
			break;
		  case '{': // Skip content of {{...}}
			{
			  while (_position + 1 < _bufferSize && strncmp(_buffer + _position, "}}", 2) != 0)
				++_position;
			  ++_position;
			  return true;
			}
			break;
		  }
      break;
    case '<':
      // Skip <!-- ... -->
      if (_position + 3 < _bufferSize && strncmp(_buffer + _position, "<!--", 4) == 0)
		{
		  _position += 4;
		  while (_position + 2 < _bufferSize && strncmp(_buffer + _position, "-->", 3) != 0)
			++_position;
		  _position += 2;
		  return true;
		}
	  // Skip <code>...</code>
	  if (_position + 5 < _bufferSize && strncmp(_buffer + _position, "<code>", 6) == 0)
		{
		  _position += 6;
		  while (_position + 6 < _bufferSize && strncmp(_buffer + _position, "</code>", 7) != 0)
			++_position;
		  _position += 6;
		  return true;
		}
      // Skip <...>
      if (_position < _bufferSize && _buffer[_position] == '<')
		{
		  while (_position < _bufferSize && _buffer[_position] != '>')
			++_position;
		  return true;
		}
      break;
    }
  return false;
}

inline bool Tokenizer::Tokenizer::_isSeparator(const unsigned char chr) const
{
  return _separators[chr] == 1;
}
