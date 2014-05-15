/*							-*- C++ -*-
** TestStringFactory.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 16:51:11 2006 Julien Lemoine
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

#ifndef   	TESTSTRINGFACTORY_H_
# define   	TESTSTRINGFACTORY_H_

#include <cppunit/extensions/HelperMacros.h>

namespace UnitTest
{
  /**
   *
   * @brief String Factory test suite
   *
   * <h2>Try all methods of ToolBox::StringFActory class</h2>
   *
   * @author Julien Lemoine <speedblue@happycoders.org>
   *
   */

  class TestStringFactory : public CppUnit::TestCase
    {
	  CPPUNIT_TEST_SUITE(TestStringFactory);
	  CPPUNIT_TEST(testClear);
	  CPPUNIT_TEST(testSmall);
	  CPPUNIT_TEST_SUITE_END();
  
    public:
      void testClear();
      void testSmall();
    };
}

#endif 	    /* !TESTSTRINGFACTORY_H_ */
