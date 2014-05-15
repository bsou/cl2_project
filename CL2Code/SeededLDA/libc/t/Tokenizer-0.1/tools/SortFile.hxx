/*
** SortFile.hxx
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Wed Dec 27 10:12:12 2006 Julien Lemoine
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

#ifndef   	SORTFILE_HXX_
# define   	SORTFILE_HXX_

#include "SortFile.h"
#include <sstream>
#include <assert.h>
#include "tools/Exception.h"
#include "tools/ports.h"
#include "SortFileThread.h"

template <typename T, class SortBloc, class FileAccess, class CopyObject>
ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::SortFile(const std::string &filename, 
								 bool computeFreq,
								 bool removeDuplicates) :
  _filename(filename), _fileAccess(0x0), _freq(0),
  _computeFreq(computeFreq), _removeDuplicates(removeDuplicates), _nbValues(0)
{
  pthread_mutex_init(&_fileMutex, 0x0);
  pthread_mutex_init(&_listMutex, 0x0);
  _fileAccess = new FileAccess(filename.c_str(), false);
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::~SortFile()
{
  pthread_mutex_destroy(&_fileMutex);
  pthread_mutex_destroy(&_listMutex);
  if (_fileAccess)
    delete _fileAccess;
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
void ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::start(const std::string &tmpFilePath, 
								   unsigned nbThreads)
{
  // First Step : Sort huge file in blocs
  _computeBlocs(tmpFilePath, nbThreads);
  // Second step : Merge bloc
  _mergeBlocs();
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
FileAccess* ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::lockFile()
{
  pthread_mutex_lock(&_fileMutex);
  return _fileAccess;
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
void ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::releaseFile()
{
  pthread_mutex_unlock(&_fileMutex);
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
void ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::addFile(const std::string &filename)
{
  pthread_mutex_lock(&_listMutex);
  _files.push_back(filename);
  pthread_mutex_unlock(&_listMutex);
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
const std::list<std::string>& ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::getFiles() const
{
  return _files;
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
void ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::_computeBlocs(const std::string &tmpFilePath, 
									   unsigned nbThreads)
{
  assert(nbThreads > 0);

  std::list<pthread_t>									threads;
  pthread_attr_t									thread_attr;
  typename std::list<SortFileThread<T, SortBloc, FileAccess, CopyObject>*>		objs;
  typename std::list<SortFileThread<T, SortBloc, FileAccess, CopyObject>*>::iterator	objIt;
  std::list<pthread_t>::iterator							pit;

  // for posix compatibility (not set by default on all architecture)
  pthread_attr_init(&thread_attr);
  pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setscope(&thread_attr,  PTHREAD_SCOPE_SYSTEM);

  for (unsigned int i = 0; i < nbThreads; ++i)
    {
      pthread_t thread;
      std::stringstream ss;
      ss << tmpFilePath << ToolBox::DirectorySeparatorString << i;
      SortFileThread<T, SortBloc, FileAccess, CopyObject> *st = 
	new SortFileThread<T, SortBloc, FileAccess, CopyObject>(*this, ss.str());
      objs.push_back(st);
      int res = pthread_create(&thread, &thread_attr, 
			       SortFileThread<T, SortBloc, FileAccess, CopyObject>::init, (void*)st);
      if (res != 0)
	throw ToolBox::ThreadError("Could not create thread", HERE);
      threads.push_back(thread);
    }
  // wait threads
  for (pit = threads.begin(); pit != threads.end(); ++pit)
    pthread_join(*pit, 0x0);
  delete _fileAccess;
  _fileAccess = 0x0;
  // delete object
  for (objIt = objs.begin(); objIt != objs.end(); ++objIt)
    delete *objIt;
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
void ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::_computeFreqs(const T &p,
									   FileAccess &outFile, FileAccess *freqFile)
{
  if (_removeDuplicates && _obj.cmpEqual(p))
    ++_freq;
  else
    {
      if (_freq > 0)
	{
	  if (freqFile)
	    freqFile->writeFreq(_obj.getObj(), _freq);
	  outFile.write(_obj.getObj());
	  ++_nbValues;
	}
      _obj.affect(p);
      _freq = 1;
    }
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
void ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::_mergeBlocs()
{
  std::vector<FileAccess*>	priorityQueue;

  priorityQueue.reserve(_files.size());
  for (std::list<std::string>::const_iterator it = _files.begin(); it != _files.end(); ++it)
    {
      FileAccess *obj = new FileAccess(it->c_str(), false);
      try
	{
	  obj->readNextObject();
	  priorityQueue.push_back(obj);
	  std::push_heap(priorityQueue.begin(), priorityQueue.end(), FileAccess::_compareReadSortedFile);
	}
      catch (ToolBox::EOFException&) { delete obj; }
    }
  unsigned queueSize = priorityQueue.size();

  FileAccess outFile(_filename.c_str(), true);
  FileAccess *freqFile = 0x0;
  if (_computeFreq)
    {
      std::string freq = _filename + std::string(".freqs");
      freqFile = new FileAccess(freq.c_str(), true);
    }
  while (queueSize > 0)
    {
      // get top of queue
      FileAccess *better = priorityQueue[0];
      const T betterObj = better->getObject();

      _computeFreqs(betterObj, outFile, freqFile);
      std::pop_heap(priorityQueue.begin(), priorityQueue.begin() + queueSize, 
		    FileAccess::_compareReadSortedFile);
      try { better->readNextObject(); }
      catch (ToolBox::EOFException&) {}

      if (better->eof())
	{
	  delete better;
	  --queueSize;
	}
      else
	{
	  priorityQueue[queueSize - 1] = better;
	  std::push_heap(priorityQueue.begin(), priorityQueue.begin() + queueSize, 
			 FileAccess::_compareReadSortedFile);
	}
    }
  if (_freq)
    {
      if (_computeFreq)
	freqFile->writeFreq(_obj.getObj(), _freq);
      ++_nbValues;
      outFile.write(_obj.getObj());
    }
  if (_computeFreq)
    delete freqFile;

  for (std::list<std::string>::const_iterator fit = _files.begin(); 
       fit != _files.end(); ++fit)
    ToolBox::unlink(fit->c_str());
}

template <typename T, class SortBloc, class FileAccess, class CopyObject>
uint64_t ToolBox::SortFile<T, SortBloc, FileAccess, CopyObject>::getNbValues() const
{
  return _nbValues;
}

#endif 	    /* !SORTFILE_HXX_ */
