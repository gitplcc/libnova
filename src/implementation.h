/*
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Copyright (C) 2000 - 2005 Liam Girdwood
 */

#ifndef _LN_IMPLEMENTATION_H
#define _LN_IMPLEMENTATION_H

#include <time.h>

#if !HAVE_CBRT
/* Simple cube root */
double cbrt(double x);
#endif

#if !HAVE_STRTOK_R
/* Catches calls to the POSIX strtok_r and converts them to a related WIN32 version. */
char *strtok_r(char *str, const char *sep, char **last);
#endif

#if !HAVE_NAN
/* Not a Number function generator */
double nan(const char *code);
#endif

#if !HAVE_ISBLANK
#define isblank(c)  ((c)== ' ' || (c)=='\t')
#endif

#endif /* _LN_IMPLEMENTATION_H */
