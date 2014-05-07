/*							-*- C++ -*-
** FastGetLine.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Thu Jun  1 20:31:51 2006 Julien Lemoine
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

#ifndef   	FASTGETLINE_H_
# define   	FASTGETLINE_H_

#include <iostream>
#include <string>
#include <fstream>

namespace ToolBox
{
  /**
   * @brief class used to read file line per line...<br>
   * This library is about 15time faster than std::getline
   * @author Julien Lemoine <speedblue@happycoders.org>
   *
   * Here is an example of file read using FastGetLine :
   * <pre>
   #include "FastGetLine.h"
   int main(int argc, char **argv)
   {
     if (argc >= 2)
       {
   	ToolBox::FastGetLine getLine;
   	getLine.openFile(argv[1]);
   	unsigned size;
   	const char *line;
   	bool res = true;
   	while (res)
   	  {
   	    res = getLine.computeNextLine();
   	    line = getLine.getLineStart();
   	    size = getLine.getLineSize();
   	    if (size)
   	      {
   		for (unsigned int i = 0; i < size; ++i)
   		  std::cout << line[i];
   		std::cout << std::endl;
   	      }
   	  }
   	getLine.closeFile();
       }
   }
   </pre>
  */
  class FastGetLine
  {
  public:
    /// default constructor
    FastGetLine();
    /// default constructor
    ~FastGetLine();

  private:
    /// avoid copy constructor
    FastGetLine(const FastGetLine &e);
    /// avoid affectation operator
    FastGetLine& operator=(const FastGetLine &e);

  public:
    /**
     * @brief open new file
     * @param file the filename to open
     */ 
    void openFile(const std::string &file);
    /// close current opened file
    void closeFile();
    /// compute the position of next line in file
    bool computeNextLine();
    /**
     * @brief get the first character of next line. Must be called
     * after computeNextLine method
     */
    const char* getLineStart() const;
    /**
     * @brief get the number of characters for the next line. Must be called
     * after computeNextLine method
     */
    unsigned getLineSize() const;

  protected:
    //read new characters from file
    void _get_buffer();

  protected:
    bool		_end;
    char		*_buff;
    std::ifstream	*_is;
    unsigned		_size;
    unsigned		_buff_pos;
    unsigned		_available;
  };
}
#endif	    /* !FASTGETLINE_H_ */
