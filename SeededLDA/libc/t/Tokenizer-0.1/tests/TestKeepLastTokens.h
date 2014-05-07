/*							-*- C++ -*-
** TestKeepLastTokens.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 15:23:12 2006 Julien Lemoine
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

#ifndef   	TESTKEEPLASTTOKENS_H_
# define   	TESTKEEPLASTTOKENS_H_

#include <cppunit/extensions/HelperMacros.h>

namespace UnitTest
{
  /**
   *
   * @brief Cluster test suite
   *
   * <h2>Try all methods of Tokenizer::KeepLastTokens class</h2>
   *
   * @author Julien Lemoine <speedblue@happycoders.org>
   *
   */
  class TestKeepLastTokens : public CppUnit::TestCase
    {
	  CPPUNIT_TEST_SUITE(TestKeepLastTokens);
	  CPPUNIT_TEST(testEmpty);
	  CPPUNIT_TEST(invalidToken);
	  CPPUNIT_TEST(pushFull);
	  CPPUNIT_TEST(all);
	  CPPUNIT_TEST_SUITE_END();

	private:
	  void testEmpty();
	  void invalidToken();
	  void pushFull();
	  void all();
    };
}

#endif 	    /* !TESTKEEPLASTTOKENS_H_ */
