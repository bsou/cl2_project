/*							-*- C++ -*-
** SortFile.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Wed Dec 27 10:11:42 2006 Julien Lemoine
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

#ifndef   	SORTFILE_H_
# define   	SORTFILE_H_

#include <list>
#include <string>
#include <stdio.h>
#include <pthread.h>

namespace ToolBox
{
  /**
   * @brief sort file that contains list of string.
   * This class contains a generic implementation, the class
   * SortBloc must have methods :
   * <pre>
   *  /// return true if the vector is full
   *  bool isFull() const;
   *  /// reset the vector of string
   *  void clear();
   *  /// add a new element
   *  void add(const std::pair<unsigned, const char*> &el);
   *  /// sort the elements
   *  void sortLess();
   *  /// return the number of elements (N)
   *  unsigned getNbElements() const;
   *  /// get element after sort (pos must be between [0..N-1]
   *  const std::pair<unsigned, const char*> getElement(unsigned pos) const;
   * </pre>
   * FileAccess must have methods :
   * <pre>
   * std::pair<unsigned, const char*> read();
   * void write(const std::pair<unsigned, const char*> &pair);
   * void writeFreq(const std::pair<unsigned, const char*> &pair, freq);
   * </pre>
   */
  template <typename T, class SortBloc, class FileAccess, class CopyObject>
  class SortFile
    {
    public:
      SortFile(const std::string &filename, bool computeFreq, bool removeDuplicates = true);
      ~SortFile();

    private:
      /// avoid default constructor
      SortFile();
      /// avoid copy constructor
      SortFile(const SortFile &e);
      /// avoid affectation operator
      SortFile& operator=(const SortFile &e);

    public:
      /// start sort of file
      void start(const std::string &tmpFilePath, unsigned nbThread);
      /// get number of different value in output file after sort
      uint64_t	getNbValues() const;

    public:
      // methods only for SortFileThread
      /// lock file for one thread
      FileAccess* lockFile();
      /// unlock file
      void releaseFile();
      /// add a file in _files list
      void addFile(const std::string &filename);
      /// get files
      const std::list<std::string>& getFiles() const;

    protected:
      void _computeBlocs(const std::string &tmpFilePath, unsigned nbThread);
      void _mergeBlocs();
      void _computeFreqs(const T &p, FileAccess &outFile, FileAccess *freqFile);

    protected:
      
      std::string		_filename;
      pthread_mutex_t		_fileMutex;
      FileAccess		*_fileAccess;
      pthread_mutex_t		_listMutex;
      std::list<std::string>	_files;
      CopyObject		_obj;
      unsigned			_freq;
      bool			_computeFreq;
      bool			_removeDuplicates;
      uint64_t			_nbValues;
    };
}

#endif 	    /* !SORTFILE_H_ */
