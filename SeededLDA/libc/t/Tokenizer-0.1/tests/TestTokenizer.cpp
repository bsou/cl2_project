/*
** TestTokenizer.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 15:13:12 2006 Julien Lemoine
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

#include "TestTokenizer.h"
#include <sstream>
#include "tokenizer/Tokenizer.h"

void UnitTest::TestTokenizer::testSharp()
{
  std::string str("AA & BB");

  Tokenizer::Tokenizer tokenizer(1);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("AA"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("BB"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testCodeTT()
{
  std::string str("AA<code>int main()</code>BB<tt>CC</tt>DD");

  Tokenizer::Tokenizer tokenizer(1);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("AA"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("BB"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("CC"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("DD"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testLimit()
{
  std::stringstream ss;
  ss << "a b";
  
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(1);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), false);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("a"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("b"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testSimple()
{
  std::stringstream ss;

  ss << "Just \t\n one   test  with compound-word\n http://www.test.com/oneUrl:withSpecial_chars/~login/" << std::endl
	 << "___Word___ __--Word2--___" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(1);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), false);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Just"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("one"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("test"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("with"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("compound"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Word2"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testSimpleSize4()
{
  std::stringstream ss;

  ss << "Just \t\n one   test  with compound-word\n http://www.test.com/oneUrl:withSpecial_chars/~login/" << std::endl
	 << "___Word___ __--Word2--___" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(1, 4);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), false);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Just"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("test"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("with"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("compound"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Word2"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testDouble()
{
  std::stringstream ss;

  ss << "Just \t\n one   test  with compound-word\n http://www.test.com/oneUrl:withSpecial_chars/~login/" << std::endl
	 << "___Word___ __--Word2--___" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(2);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), false);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Just one"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("one test"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("test with"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("with compound"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("compound word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("word Word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Word Word2"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testDoubleSize4()
{
  std::stringstream ss;

  ss << "Just \t\n one   test  with compound-word\n http://www.test.com/oneUrl:withSpecial_chars/~login/" << std::endl
	 << "___Word___ __--Word2--___" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(2, 4);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), false);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Just test"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("test with"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("with compound"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("compound word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("word Word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Word Word2"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testTriple()
{
  std::stringstream ss;

  ss << "Just \t\n one   test  with compound-word\n http://www.test.com/oneUrl:withSpecial_chars/~login/" << std::endl
	 << "___Word___ __--Word2--___" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(3);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), false);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Just one test"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("one test with"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("test with compound"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("with compound word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("compound word Word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("word Word Word2"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testTripleSize4()
{
  std::stringstream ss;

  ss << "Just \t\n one   test  with compound-word\n http://www.test.com/oneUrl:withSpecial_chars/~login/" << std::endl
	 << "___Word___ __--Word2--___" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(3, 4);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), false);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Just test with"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("test with compound"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("with compound word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("compound word Word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("word Word Word2"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testWiki()
{
  std::stringstream ss;
  ss << "test_hitos = &nbsp; De hecho<br />&nbsp; Declaration" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(1);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("test"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("hitos"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("De"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("hecho"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Declaration"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testSimpleWiki()
{
  std::stringstream ss;

  ss << "Just \t\n one [[wiki:synthax test of synthax]]  test  with compound-word {{a|b|c}}\n http://www.test.com/oneUrl:withSpecial_chars/~login/ <!-- one comment -->" << std::endl
	 << "___Word___ __--Word2--___" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(1);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Just"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("one"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("test"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("with"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("compound"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Word2"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testDoubleWiki()
{
  std::stringstream ss;
  
  ss << "Just \t\n one [[wiki:synthax test of synthax]]  test  with compound-word {{a|b|c}}\n http://www.test.com/oneUrl:withSpecial_chars/~login/ <!-- one comment -->" << std::endl
	 << "___Word___ __--Word2--___" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(2);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Just one"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("one test"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("test with"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("with compound"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("compound word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("word Word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Word Word2"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testSpecial()
{
  Tokenizer::Tokenizer tokenizer(1);
  std::string str("AA{| Skip {| Skip {| Skip |} Skip |} Skip|}BB");
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);

  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("AA"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("BB"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());

  str = ("AA{{ blah blah blah}}BB");
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);

  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("AA"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("BB"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());

  str = ("AA<code> blah blah blah}}</code>BB");
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);

  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("AA"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("BB"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());

  str = ("AA[[ blah blah blah]]BB");
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);

  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("AA"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("BB"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());

  str = ("AA<!-- blah blah blah-->BB");
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);

  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("AA"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("BB"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());

}

void UnitTest::TestTokenizer::testTripleWikiEmpty()
{
  std::stringstream ss;

  ss << " [[Image:Southamerica-political-es-01.png|right|210px|AmÃ©rica del Sur]]" << std::endl
     << "{{AP|Cultura de Andorra}}" << std::endl
     << "{| &lt;align=&quot;center&quot;&gt; class=wikitable" << std::endl
     << "|+ '''&lt;big&gt;Fiestas&lt;/big&gt;'''" << std::endl
     << "|- style=&quot;background:#efefef;&quot;" << std::endl
     << "! Fecha" << std::endl
     << "! Nombre en castellano" << std::endl
     << "! Nombre local" << std::endl
     << "! Notas" << std::endl
     << "|- " << std::endl
     << "| [[8 de septiembre]]" << std::endl
     << "| [[Santuario de Meritxell|Virgen de Meritxell]]" << std::endl
     << "| Mare de DÃ©u de Meritxell" << std::endl
     << "| Fiesta nacional de Andorra" << std::endl
     << "|-" << std::endl
     << "| [[14 de marzo]]" << std::endl
     << "| DÃ­a de la ConstituciÃ³n" << std::endl
     << "| Diada de la ConstituciÃ³" << std::endl
     << "| Aprobada el 14 de marzo de 1993" << std::endl
     << "|}" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(1);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);

  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}

void UnitTest::TestTokenizer::testTripleWiki()
{
  std::stringstream ss;

  ss << "Just \t\n one [[wiki:synthax test of synthax]]  test  with compound-word {{a|b|c}}\n http://www.test.com/oneUrl:withSpecial_chars/~login/ <!-- one comment -->" << std::endl
	 << "___Word___ __--Word2--___" << std::endl;
  std::string str = ss.str();

  Tokenizer::Tokenizer tokenizer(3);
  tokenizer.setBufferToAnalyze(str.c_str(), str.size(), true);
  
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("Just one test"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("one test with"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("test with compound"), std::string(token.getContent(), token.getSize()));
  }	
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("with compound word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("compound word Word"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(false, tokenizer.eof());
  {
	Tokenizer::Token token = tokenizer.getNextToken();
	CPPUNIT_ASSERT_EQUAL(std::string("word Word Word2"), std::string(token.getContent(), token.getSize()));
  }
  CPPUNIT_ASSERT_EQUAL(true, tokenizer.eof());
}
