/*							-*- C++ -*-
** SortFileThread.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Wed Dec 27 10:32:19 2006 Julien Lemoine
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

#ifndef   	SORTFILETHREAD_H_
# define   	SORTFILETHREAD_H_

#include <string>
#include <vector>

namespace ToolBox
{
  /**
   * @brief sort file that contains list of string (one thread)
   */
  template <typename T, class SortBloc, class FileAccess, class CopyObject>
  class SortFileThread
    {
    public:
      SortFileThread(SortFile<T, SortBloc, FileAccess, CopyObject> &mainThread, 
		     const std::string &startFilename);
      ~SortFileThread();

    private:
      /// avoid default constructor
      SortFileThread();
      /// avoid copy constructor
      SortFileThread(const SortFileThread &e);
      /// avoid affectation operator
      SortFileThread& operator=(const SortFileThread &e);

    public:
      /// launch thread
      static void* init(void *instance);
      /// start
      void start();

    private:
      SortFile<T, SortBloc, FileAccess, CopyObject>	&_mainThread;
      std::string					_startFilename;
      SortBloc						_bloc;
      unsigned						_blocCnt;
    };
}

#endif 	    /* !SORTFILETHREAD_H_ */
