/*
** TestSuite.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Thu Jul 13 22:06:54 2006 Julien Lemoine
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

#include <iostream>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/TestResult.h>
#include "TestToolBoxTrie.h"
#include "TestToolBoxHash.h"
#include "TestKeepLastTokens.h"
#include "TestTokenizer.h"
#include "TestStringFactory.h"

class VerboseProgressListener : public CppUnit::TextTestProgressListener
{
  void startTest(CppUnit::Test *test)
  {
    std::cerr << "Running " << test->getName() << '\n';
    std::cerr.flush();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UnitTest::TestTokenizer, "tests");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UnitTest::TestKeepLastTokens, "tests");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UnitTest::TestStringFactory, "tests");

int main(int argc, char **argv)
{
  CppUnit::TestSuite		*suite = new CppUnit::TestSuite("Test Suite");
  CppUnit::TextUi::TestRunner	runner;
  suite->addTest(CppUnit::TestFactoryRegistry::getRegistry("tests").makeTest());
  runner.addTest(suite);

  VerboseProgressListener progress;
  runner.eventManager().addListener(&progress);
  bool wasSucessful = runner.run("", false, true, false);
  return wasSucessful ? 0 : 1;
}
