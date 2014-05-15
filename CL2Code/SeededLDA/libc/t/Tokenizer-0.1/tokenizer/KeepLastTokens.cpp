/*
** KeepLastTokens.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 15:13:23 2006 Julien Lemoine
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

#include "KeepLastTokens.h"
#include <assert.h>
#include "tools/Exception.h"


Tokenizer::KeepLastTokensIterator::KeepLastTokensIterator(const std::vector<Token> &pool, unsigned position) :
  _pool(&pool), _position(position)
{
}

Tokenizer::KeepLastTokensIterator::~KeepLastTokensIterator()
{ // nothing to do here
}

Tokenizer::KeepLastTokensIterator& Tokenizer::KeepLastTokensIterator::operator++()
{
  ++_position;
  return *this;
}

const Tokenizer::Token& Tokenizer::KeepLastTokensIterator::operator*() const
{
  assert(_pool != 0x0);
  return (*_pool)[_position % _pool->size()];
}

const Tokenizer::Token* Tokenizer::KeepLastTokensIterator::operator->() const
{
  assert(_pool != 0x0);
  return &((*_pool)[_position % _pool->size()]);

}

bool Tokenizer::KeepLastTokensIterator::operator!=(const KeepLastTokensIterator &other) const
{
  return _pool != other._pool || _position != other._position;
}

bool Tokenizer::KeepLastTokensIterator::operator==(const KeepLastTokensIterator &other) const
{
  return _pool == other._pool && _position == other._position;
}

Tokenizer::KeepLastTokens::KeepLastTokens(unsigned NGramSize) :
  _olderToken(0), _size(0)
{
  /// fill pool with a sentinel
  _pool.resize(NGramSize, Token(0x0, 0));
}
	
Tokenizer::KeepLastTokens::~KeepLastTokens()
{
}

void Tokenizer::KeepLastTokens::pop()
{
  if (_size == 0)
	throw ToolBox::Exception("Pool is empty", HERE);

  _olderToken = (_olderToken + 1) % _pool.size();
  --_size;
}

void Tokenizer::KeepLastTokens::push(const Token &t)
{
  if (t.getContent() == 0x0 || t.getSize() == 0)
	throw ToolBox::Exception("Invalid token", HERE);
  if (_size == _pool.size())
	throw ToolBox::Exception("Pool is full", HERE);
  unsigned position = (_olderToken + _size) % _pool.size();
  _pool[position] = t;
  ++_size;
}

bool Tokenizer::KeepLastTokens::full() const
{
  return _size == _pool.size();
}

bool Tokenizer::KeepLastTokens::empty() const
{
  return _size == 0;
}

const Tokenizer::Token& Tokenizer::KeepLastTokens::top() const
{
  if (_size == 0)
	throw ToolBox::Exception("Pool is empty", HERE);

  assert(_olderToken <= _pool.size());
  assert(_pool[_olderToken].getContent() != 0x0);
  assert(_pool[_olderToken].getSize() > 0);

  return _pool[_olderToken];
}

Tokenizer::KeepLastTokensIterator Tokenizer::KeepLastTokens::begin()
{
  return KeepLastTokensIterator(_pool, _olderToken);
}

Tokenizer::KeepLastTokensIterator Tokenizer::KeepLastTokens::end()
{
  return KeepLastTokensIterator(_pool, _olderToken + _size);
}

void Tokenizer::KeepLastTokens::clear()
{
  _size = 0;
  _olderToken = 0;
}
