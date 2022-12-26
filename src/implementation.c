/* $Id: utility.c,v 1.18 2009-04-20 07:17:00 pkubanek Exp $
 **
 * Copyright (C) 1999, 2000 Juan Carlos Remis
 * Copyright (C) 2002 Liam Girdwood <lgirdwood@gmail.com>
 *
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
 */

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  Module:                                                               */
/*                                                                        */
/*  Description:                                                          */
/*                                                                        */
/*                                                                        */
/*  "CAVEAT UTILITOR".                                                    */
/*                                                                        */
/*                   "Non sunt multiplicanda entia praeter necessitatem"  */
/*                                                   Guillermo de Occam.  */
/*------------------------------------------------------------------------*/
/*  Revision History:                                                     */
/*                                                                        */
/*------------------------------------------------------------------------*/

/**/

#include "config.h"

#include "implementation.h"

#include <string.h>
#include <math.h>

/* This section is for Win32 substitutions. */

#if !HAVE_STRTOK_R

/* Catches calls to the POSIX strtok_r and converts them to a related WIN32 version. */
char *strtok_r(char *str, const char *sep, char **last)
{
#if defined(__MINGW32__) || defined(__MINGW64__)
    return strtok_s(str, sep, last);
#else
    return strtok(str, sep);
#endif /* defined(__MINGW32__) || defined(__MINGW64__) */
}

#endif /* !HAVE_STRTOK_R */

/* C89 substitutions for C99 functions. */

#ifndef HAVE_CBRT
/* Simple cube root */
double cbrt(double x)
{
    return pow(x, 1.0 / 3.0);
}
#endif /* !HAVE_CBRT */

#ifndef HAVE_NAN
/* Not a Number function generator */
double nan(const char* code)
{
    double zero = 0.0;

    return zero / 0.0;
}
#endif /* !HAVE_NAN */
