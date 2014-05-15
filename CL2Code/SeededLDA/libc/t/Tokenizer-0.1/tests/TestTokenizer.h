/*							-*- C++ -*-
** TestTokenizer.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 15:12:26 2006 Julien Lemoine
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

#ifndef   	TESTTOKENIZER_H_
# define   	TESTTOKENIZER_H_

#include <cppunit/extensions/HelperMacros.h>

namespace UnitTest
{

  /**
   *
   * @brief Cluster test suite
   *
   * <h2>Try all methods of Tokenizer::Tokenizer class</h2>
   *
   * @author Julien Lemoine <speedblue@happycoders.org>
   *
   */

  class TestTokenizer : public CppUnit::TestCase
  {
    CPPUNIT_TEST_SUITE(TestTokenizer);
    CPPUNIT_TEST(testSimple);
    CPPUNIT_TEST(testDouble);
    CPPUNIT_TEST(testTriple);
    CPPUNIT_TEST(testSimpleSize4);
    CPPUNIT_TEST(testDoubleSize4);
    CPPUNIT_TEST(testTripleSize4);
    CPPUNIT_TEST(testSimpleWiki);
    CPPUNIT_TEST(testDoubleWiki);
    CPPUNIT_TEST(testTripleWiki);
    CPPUNIT_TEST(testLimit);
    CPPUNIT_TEST(testTripleWikiEmpty);
    CPPUNIT_TEST(testWiki);
	CPPUNIT_TEST(testSpecial);
	CPPUNIT_TEST(testCodeTT);
	CPPUNIT_TEST(testSharp);
    CPPUNIT_TEST_SUITE_END();

  private:
    void testSimple();
    void testDouble();
    void testTriple();
    void testSimpleSize4();
    void testDoubleSize4();
    void testTripleSize4();
    void testSimpleWiki();
    void testDoubleWiki();
    void testTripleWiki();
    void testLimit();
    void testTripleWikiEmpty();
    void testWiki();
	void testSpecial();
	void testCodeTT();
	void testSharp();
  };
}


#endif 	    /* !TESTTOKENIZER_H_ */
