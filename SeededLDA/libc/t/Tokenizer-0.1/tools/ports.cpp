/*
** ports.cpp
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sun Sep 24 17:00:46 2006 Julien Lemoine
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

#include "ports.h"

#if defined(_WIN32) || defined(__MINGW32__)
# include <direct.h>
#else
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
#endif

namespace ToolBox
{
#if defined(_WIN32) || defined(__MINGW32__)
  const char DirectorySeparator = '\\';
  const char* DirectorySeparatorString = "\\";
#else
  const char DirectorySeparator = '/';
  const char* DirectorySeparatorString = "/";
#endif
}

void ToolBox::unlink(const char *pathname)
{
#if defined(_WIN32) || defined(__MINGW32__)
  _unlink(pathname);
#else
  ::unlink(pathname);
#endif
}

void ToolBox::mkdir(const char *pathname)
{
#if defined(_WIN32) || defined(__MINGW32__)
  _mkdir(pathname);
#else
  ::mkdir(pathname, 0777);
#endif
}

void ToolBox::rmdir(const char *pathname)
{
#if defined(_WIN32) || defined(__MINGW32__)
  _rmdir(pathname);
#else
  ::rmdir(pathname);
#endif
}

void ToolBox::sleep(const unsigned t)
{
#if defined(_WIN32) || defined(__MINGW32__)
  Sleep(1000 * t);
#else
  ::sleep(t);
#endif
}

void ToolBox::usleep(const unsigned t)
{
#if defined(_WIN32) || defined(__MINGW32__)
  Sleep(t);
#else
  ::usleep(1000 * t);
#endif
}
