/*
** TestToolBoxTrie.cpp
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

#include "TestToolBoxTrie.h"
#include <iostream>
#include <sstream>
#include <list>
#include "tools/Exception.h"
#include "tools/Trie.hxx"

namespace UnitTest
{

  void TestToolBoxTrie::testString()
  {
    try
      {

		ToolBox::Trie<std::string> strTrie(std::string(""));
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

  void TestToolBoxTrie::testUnsigned()
  {
	try
	  {
		ToolBox::Trie<unsigned> nbTrie(0);
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

  void TestToolBoxTrie::testSort()
  {
    try
      {
		//Test if trie is well sorted
		ToolBox::Trie<unsigned> test(0);
		test.addEntry("acd", 3, 1);
		test.addEntry("ade", 3, 2);
		test.addEntry("abc", 3, 3);
		test.addEntry("ace", 3, 4);
		test.addEntry("adc", 3, 5);
		test.addEntry("abe", 3, 6);
		test.addEntry("acc", 3, 7);
		test.addEntry("add", 3, 8);
		test.addEntry("abd", 3, 9);
		const ToolBox::TrieNode<unsigned> *first = test.getInitialNode(), *last = 0x0;
		CPPUNIT_ASSERT_EQUAL((unsigned)0, first->getValue());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)first->getBrother());
		CPPUNIT_ASSERT_EQUAL((unsigned char)'a', first->getSubNodeLabel());
		// Get one first sub node
		first = first->getSubNode(); //a*
		CPPUNIT_ASSERT_EQUAL((unsigned)0, first->getValue());
		CPPUNIT_ASSERT(first != 0x0);

		// There is no other letter than a
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)first->getBrother());

		// Get first sub node of 'a'
		CPPUNIT_ASSERT(first->getSubNode() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'b', first->getSubNodeLabel());
		first = first->getSubNode(); //ab*
		CPPUNIT_ASSERT_EQUAL((unsigned)0, first->getValue());
		CPPUNIT_ASSERT(first->getSubNode() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'c', first->getSubNodeLabel());
		last = first->getSubNode(); //abc
		CPPUNIT_ASSERT_EQUAL((unsigned)3, last->getValue());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getSubNode());
		CPPUNIT_ASSERT(last->getBrother() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'d', last->getBrotherLabel());
		last = last->getBrother(); // abd
		CPPUNIT_ASSERT_EQUAL((unsigned)9, last->getValue());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getSubNode());
		CPPUNIT_ASSERT(last->getBrother() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'e', last->getBrotherLabel());
		last = last->getBrother(); // abe
		CPPUNIT_ASSERT_EQUAL((unsigned)6, last->getValue());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getSubNode());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getBrother());

		CPPUNIT_ASSERT(first->getBrother() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'c', first->getBrotherLabel());
		first = first->getBrother(); //ac*
		CPPUNIT_ASSERT_EQUAL((unsigned)0, first->getValue());

		CPPUNIT_ASSERT(first->getSubNode() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'c', first->getSubNodeLabel());
		last = first->getSubNode(); //acc
		CPPUNIT_ASSERT_EQUAL((unsigned)7, last->getValue());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getSubNode());
		CPPUNIT_ASSERT(last->getBrother() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'d', last->getBrotherLabel());
		last = last->getBrother(); // acd
		CPPUNIT_ASSERT_EQUAL((unsigned)1, last->getValue());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getSubNode());
		CPPUNIT_ASSERT(last->getBrother() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'e', last->getBrotherLabel());
		last = last->getBrother(); // ace
		CPPUNIT_ASSERT_EQUAL((unsigned)4, last->getValue());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getSubNode());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getBrother());

		CPPUNIT_ASSERT(first->getBrother() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'d', first->getBrotherLabel());
		first = first->getBrother(); //ad*
		CPPUNIT_ASSERT_EQUAL((unsigned)0, first->getValue());

		CPPUNIT_ASSERT(first->getSubNode() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'c', first->getSubNodeLabel());
		last = first->getSubNode(); //adc
		CPPUNIT_ASSERT_EQUAL((unsigned)5, last->getValue());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getSubNode());
		CPPUNIT_ASSERT(last->getBrother() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'d', last->getBrotherLabel());
		last = last->getBrother(); // add
		CPPUNIT_ASSERT_EQUAL((unsigned)8, last->getValue());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getSubNode());
		CPPUNIT_ASSERT(last->getBrother() != 0x0);
		CPPUNIT_ASSERT_EQUAL((unsigned char)'e', last->getBrotherLabel());
		last = last->getBrother(); // ade
		CPPUNIT_ASSERT_EQUAL((unsigned)2, last->getValue());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getSubNode());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)last->getBrother());
		CPPUNIT_ASSERT_EQUAL((void*)0x0, (void*)first->getBrother());
      }
    catch (const ToolBox::Exception &e)
      {
		std::cerr << e << std::endl;
		CPPUNIT_ASSERT(false);
      }
  }
}
