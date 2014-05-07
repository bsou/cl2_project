/*
** TestToolBoxHash.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Fri Jul 14 13:45:15 2006 Julien Lemoine
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

#include "TestToolBoxHash.h"
#include <iostream>
#include <sstream>
#include <list>
#include "tools/Exception.h"
#include "tools/Hash.hxx"

namespace UnitTest
{

  void TestToolBoxHash::testString()
  {
    try
      {
	ToolBox::Hash<std::string> strTrie(std::string(""));
	strTrie.addEntry("Premiere Chaine", 15, std::string("1er"));
	strTrie.addEntry("Deuxieme Chaine", 15, std::string("2eme"));
	{
	  const std::string &s = strTrie.getEntry("unknown", 7);
	  CPPUNIT_ASSERT_EQUAL(std::string(""), s);
	}
	{
	  const std::string &s = strTrie.getEntry("test", 4);
	  CPPUNIT_ASSERT_EQUAL(std::string(""), s);
	}
	{
	  const std::string &s = strTrie.getEntry("Premiere Chaine", 15);
	  CPPUNIT_ASSERT_EQUAL(std::string("1er"), s);
	}
	{
	  const std::string &s = strTrie.getEntry("Premiere Chaine", 14);
	  CPPUNIT_ASSERT_EQUAL(std::string(""), s);
	}
	{
	  const std::string &s = strTrie.getEntry("premiere Chaine", 15);
	  CPPUNIT_ASSERT_EQUAL(std::string(""), s);
	}
	{
	  const std::string &s = strTrie.getEntry("Premiere Chaine ", 16);
	  CPPUNIT_ASSERT_EQUAL(std::string(""), s);
	}
	{
	  const std::string &s = strTrie.getEntry("Deuxieme Chaine", 15);
	  CPPUNIT_ASSERT_EQUAL(std::string("2eme"), s);
	}
      }
    catch (const ToolBox::Exception &e)
      {
	std::cerr << e << std::endl;
	CPPUNIT_ASSERT(false);
      }
  }

  void TestToolBoxHash::testUnsigned()
  {
    try
      {
	ToolBox::Hash<unsigned> nbTrie(0);
	nbTrie.addEntry("un", 2, 1);
	nbTrie.addEntry("deux", 4, 2);
	nbTrie.addEntry("test", 4, 3);
	nbTrie.addEntry("tat", 3, 4);
	nbTrie.addEntry("taa", 3, 4);
	nbTrie.addEntry("tbp", 3, 5);
	nbTrie.addEntry("tlp", 3, 6);

	unsigned res = 0;

	res = nbTrie.getEntry("un", 2);
	CPPUNIT_ASSERT_EQUAL((unsigned)1, res);

	res = nbTrie.getEntry("Un", 2);
	CPPUNIT_ASSERT_EQUAL((unsigned)0, res);
		
	res = nbTrie.getEntry("UN", 2);
	CPPUNIT_ASSERT_EQUAL((unsigned)0, res);

	res = nbTrie.getEntry("", 0);
	CPPUNIT_ASSERT_EQUAL((unsigned)0, res);
		
	res = nbTrie.getEntry("deux", 4);
	CPPUNIT_ASSERT_EQUAL((unsigned)2, res);
		
	res = nbTrie.getEntry(" deux ", 6);
	CPPUNIT_ASSERT_EQUAL((unsigned)0, res);
      }
    catch (const ToolBox::Exception &e)
      {
	std::cerr << e << std::endl;
	CPPUNIT_ASSERT(false);
      }
  }
}
