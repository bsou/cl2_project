/*
** SortFileThread.hxx
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Wed Dec 27 10:36:21 2006 Julien Lemoine
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

#ifndef   	SORTFILETHREAD_HXX_
# define   	SORTFILETHREAD_HXX_

#include "SortFileThread.h"
#include <assert.h>
#include <algorithm>
#include <sstream>
#include "Exception.h"
#include "SortFile.h"

template <typename T, class SortBloc, class FileAccess, class CopyObject>
ToolBox::SortFileThread<T, SortBloc, FileAccess, CopyObject>::
SortFileThread(SortFile<T, SortBloc, FileAccess, CopyObject> &mainThread, 
	       const std::string &startFilename) :
  _mainThread(mainThread), _startFilename(startFilename), 
  _bloc(5000000), // Sort per bloc of 5 million
  _blocCnt(0)
{
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
ToolBox::SortFileThread<T, SortBloc, FileAccess, CopyObject>::~SortFileThread()
{
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
void ToolBox::SortFileThread<T, SortBloc, FileAccess, CopyObject>::start()
{
  bool endProcessing = false;
  while (!endProcessing)
    {
      // Lock the file
      FileAccess *file = _mainThread.lockFile();
      try
	{
	  while (!file->eof() && !_bloc.isFull())
	    {
	      // Read from file
	      file->readNextObject();
	      const T obj = file->getObject();
	      // add to bloc
	      _bloc.add(obj);
	    }
	}
      catch (ToolBox::EOFException&) { }
      endProcessing = file->eof(); 

      // Release file
      _mainThread.releaseFile();
      if (_bloc.getNbElements() > 0)
	{
	  // Sort Objects
	  _bloc.sortLess();

	  std::stringstream outFile;
	  outFile << _startFilename << _blocCnt;
	  ++_blocCnt;
	  FileAccess ofile(outFile.str().c_str(), true);
	  for (unsigned int i = 0; i < _bloc.getNbElements(); ++i)
	    ofile.write(_bloc.getElement(i));
	  _mainThread.addFile(outFile.str());
	}
      _bloc.clear();
    }
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
void* ToolBox::SortFileThread<T, SortBloc, FileAccess, CopyObject>::init(void *instance)
{
  assert(instance != 0x0);
  SortFileThread<T, SortBloc, FileAccess, CopyObject> *thread = 
    (SortFileThread<T, SortBloc, FileAccess, CopyObject>*)instance;
  thread->start();
  return 0x0;
}

#endif 	    /* !SORTFILETHREAD_HXX_ */

