/*
** TestKeepLastTokens.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 15:27:38 2006 Julien Lemoine
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

#include "TestKeepLastTokens.h"
#include "tokenizer/KeepLastTokens.h"
#include "tools/Exception.h"

void UnitTest::TestKeepLastTokens::all()
{
  std::string							str1("str1");
  std::string							str2("str2");
  std::string							str3("str3");
  std::string							str4("str4");
  std::string							str5("str5");

  Tokenizer::KeepLastTokens		pool(5);

  // Add 5 elements in pool
  CPPUNIT_ASSERT_EQUAL(true, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() == pool.end());
  pool.push(Tokenizer::Token(str1.c_str(), str1.size()));
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(t.getContent(), t.getSize()));
  }
  Tokenizer::KeepLastTokensIterator it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());


  pool.push(Tokenizer::Token(str2.c_str(), str2.size()));
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());


  pool.push(Tokenizer::Token(str3.c_str(), str3.size()));
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str3"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());


  pool.push(Tokenizer::Token(str4.c_str(), str4.size()));
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str3"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str4"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());


  pool.push(Tokenizer::Token(str5.c_str(), str5.size()));
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(true, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str3"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str4"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str5"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());


  // Remove 2 elements in pool
  pool.pop();
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str3"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str4"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str5"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());


  pool.pop();
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str3"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str3"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str4"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str5"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());


  // Add 2 elements
  pool.push(Tokenizer::Token(str2.c_str(), str2.size()));
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str3"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str3"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str4"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str5"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());


  pool.push(Tokenizer::Token(str1.c_str(), str1.size()));
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(true, pool.full());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str3"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str3"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str4"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str5"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());

  // remove 5 elements
  pool.pop();
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str4"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str4"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str5"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());


  pool.pop();
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str5"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str5"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());

  pool.pop();
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str2"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it != pool.end());
  CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());


  pool.pop();
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() != pool.end());
  {
	const Tokenizer::Token &t = pool.top();
	CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(t.getContent(), t.getSize()));
  }
  it = pool.begin();
  CPPUNIT_ASSERT_EQUAL(std::string("str1"), std::string(it->getContent(), it->getSize()));
  ++it;
  CPPUNIT_ASSERT(it == pool.end());

  pool.pop();
  CPPUNIT_ASSERT_EQUAL(true, pool.empty());
  CPPUNIT_ASSERT_EQUAL(false, pool.full());
  CPPUNIT_ASSERT(pool.begin() == pool.end());
}

void UnitTest::TestKeepLastTokens::pushFull()
{
  std::string str("str");
  Tokenizer::KeepLastTokens pool(1);

  CPPUNIT_ASSERT_EQUAL(true, pool.empty());
  pool.push(Tokenizer::Token(str.c_str(), str.size()));
  CPPUNIT_ASSERT_EQUAL(false, pool.empty());
  
  try
	{
	  pool.push(Tokenizer::Token(str.c_str(), str.size()));
	  CPPUNIT_ASSERT(false);
	}
  catch (ToolBox::Exception &e)
	{
	  CPPUNIT_ASSERT_EQUAL(std::string("Pool is full"), e.getError());
	}
}

void UnitTest::TestKeepLastTokens::invalidToken()
{
  Tokenizer::KeepLastTokens pool(1);
  std::string str("str");

  try
	{
	  pool.push(Tokenizer::Token(str.c_str(), 0));
	  CPPUNIT_ASSERT(false);
	}
  catch (ToolBox::Exception &e)
	{
	  CPPUNIT_ASSERT_EQUAL(std::string("Invalid token"), e.getError());
	}
  try
	{
	  pool.push(Tokenizer::Token(0x0, 10));
	  CPPUNIT_ASSERT(false);
	}
  catch (ToolBox::Exception &e)
	{
	  CPPUNIT_ASSERT_EQUAL(std::string("Invalid token"), e.getError());
	}
}

void UnitTest::TestKeepLastTokens::testEmpty()
{
  Tokenizer::KeepLastTokens pool(1);
  std::string str("str");

  try
	{
	  pool.pop();
	  CPPUNIT_ASSERT(false);
	}
  catch (ToolBox::Exception &e)
	{
	  CPPUNIT_ASSERT_EQUAL(std::string("Pool is empty"), e.getError());
	}
  try
	{
	  pool.top();
	  CPPUNIT_ASSERT(false);
	}
  catch (ToolBox::Exception &e)
	{
	  CPPUNIT_ASSERT_EQUAL(std::string("Pool is empty"), e.getError());
	}
  pool.push(Tokenizer::Token(str.c_str(), str.size()));
  pool.pop();
  try
	{
	  pool.pop();
	  CPPUNIT_ASSERT(false);
	}
  catch (ToolBox::Exception &e)
	{
	  CPPUNIT_ASSERT_EQUAL(std::string("Pool is empty"), e.getError());
	}
  try
	{
	  pool.top();
	  CPPUNIT_ASSERT(false);
	}
  catch (ToolBox::Exception &e)
	{
	  CPPUNIT_ASSERT_EQUAL(std::string("Pool is empty"), e.getError());
	}
}
