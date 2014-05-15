/*
** TestStringFactory.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Dec 17 16:52:37 2006 Julien Lemoine
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

#include "TestStringFactory.h"
#include "tools/StringFactory.h"

void UnitTest::TestStringFactory::testClear()
{
  ToolBox::StringFactory factory(10);
  
  const char *s1 = factory.allocateString(std::string("abc"));
  const char *s2 = factory.allocateString(std::string("def"));
  const char *s3 = factory.allocateString(std::string("g"));
  const char *s4 = factory.allocateString(std::string("h"));
  
  factory.clear();
  s1 = factory.allocateString(std::string("abc"));
  s2 = factory.allocateString(std::string("def"));
  s3 = factory.allocateString(std::string("g"));
  s4 = factory.allocateString(std::string("h"));
  CPPUNIT_ASSERT(s1 + 4 == s2);
  CPPUNIT_ASSERT(s2 + 4 == s3);
  CPPUNIT_ASSERT(s4 != s3 + 2);
}

void UnitTest::TestStringFactory::testSmall()
{
  ToolBox::StringFactory factory(10);
  
  const char *s1 = factory.allocateString(std::string("abc"));
  const char *s2 = factory.allocateString(std::string("def"));
  const char *s3 = factory.allocateString(std::string("g"));
  const char *s4 = factory.allocateString(std::string("h"));
  
  CPPUNIT_ASSERT(s1 + 4 == s2);
  CPPUNIT_ASSERT(s2 + 4 == s3);
  CPPUNIT_ASSERT(s4 != s3 + 2);
  CPPUNIT_ASSERT_EQUAL(std::string("abc"), std::string(s1));
  CPPUNIT_ASSERT_EQUAL(std::string("def"), std::string(s2));
  CPPUNIT_ASSERT_EQUAL(std::string("g"), std::string(s3));
  CPPUNIT_ASSERT_EQUAL(std::string("h"), std::string(s4));
}
