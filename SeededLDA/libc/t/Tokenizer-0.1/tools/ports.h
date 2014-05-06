/*							-*- C++ -*-
** ports.h
** Login : Julien Lemoine <speedblue@happycoders.org>
** Started on  Sat Jul  1 13:11:45 2006 Julien Lemoine
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

#ifndef   	PORTS_HH_
# define   	PORTS_HH_

/**
 * \file
 * \brief Ports function : contains OS dependant function like file
 * system operation ...
 *
 * <p>These constants/functions are used to avoid compatibility problem between
 * different architecture like MS Windows, Unix, ... </p>
 */

namespace ToolBox
{
  extern const char DirectorySeparator;
  extern const char* DirectorySeparatorString;

  void unlink(const char *pathname);
  void mkdir(const char *pathname);
  void rmdir(const char *pathname);
  void sleep(const unsigned t);
  void usleep(const unsigned t);
}

#endif	    /* !PORTS_HH_ */
