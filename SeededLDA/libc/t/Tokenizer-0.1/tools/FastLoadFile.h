/*							-*- C++ -*-
** FastLoadFile.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sat Jul  1 13:06:37 2006 Julien Lemoine
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

#ifndef   	FASTLOADFILE_HH_
# define   	FASTLOADFILE_HH_

#include <iostream>
#include <string>
#include <list>
#include <fstream>

namespace ToolBox
{
  /**
   * @brief class used to read ascii file...<br>
   * This library avoid realloc for big file, it uses 2x file
   * size in memory during load.
   * @author Julien Lemoine <speedblue@happycoders.org>
   *
   * Here is an example of file read using FastReadFileAscii :
   * <pre>
   #include "FastLoadFile.h"

   int main(int argc, char **argv)
   {
     if (argc >= 2)
     { 
       ToolBox::FastLoadFile loader;
       loader.openFile(argv[1]);
       const std::string &str = loader.read();
       loader.closeFile();
       std::cout << str;
       is.close();
     }
   }
   </pre>
  */
  class FastLoadFile
  {
  public:
    /// default constructor
    FastLoadFile();
    /// default destructor
    ~FastLoadFile();

  private:
    /// avoid copy constructor
    FastLoadFile(const FastLoadFile &e);
    /// avoid affectation operator
    FastLoadFile& operator=(const FastLoadFile &e);

  public:
    /**
     * @brief open new file
     * @param file the filename to open
     */ 
    void openFile(const std::string &file);
    /// close current opened file
    void closeFile();
    /// get content of file
    const std::string &read();
    /// alloc a new buffer with the content of file, the buffer will
    /// be desallocated by user with delete[] result.first
    std::pair<const char*, unsigned> readAlloc();

  protected:
    bool _fill_tmp();
    void _read(unsigned &total);

  protected:
    std::pair<char*, unsigned>			_tmp;
    std::list<std::pair<char*, unsigned> >	_buffs;
    std::string					_res;
    std::ifstream				*_is;
  };
}

#endif	    /* !FASTLOADFILE_HH_ */
