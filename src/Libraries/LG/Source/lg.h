/*

Copyright (C) 2015-2018 Night Dive Studios, LLC.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
//              LG.H            Looking Glass Over-Arching Master Control Header File
//
/*
* $Header: n:/project/lib/src/lg/rcs/lg.h 1.3 1993/08/06 11:00:11 rex Exp $
* $Log: lg.h $
 * Revision 1.3  1993/08/06  11:00:11  rex
 * Removed libdbg.h
 *
 * Revision 1.2  1993/03/19  18:20:22  rex
 * Added RCS header
 *
*/

#ifndef __TYPES_H
#include "lg_types.h"
#endif

#include <string.h>

//#include "memall.h"
#include "dbg.h"


// DG: helpful for seeing which stubbed out things are even used

// prints a stub message (incl. containing function)
#define STUB(msg)                               \
    printf("STUB: %s() %s\n", __func__, msg);

// prints a stub message (incl. containing function) only the first time it's called
#define STUB_ONCE(msg) do {                             \
        static int show=1;                              \
        if(show) {                                      \
            show = 0;                                   \
            printf("STUB: %s() %s\n", __func__, msg);   \
        }                                               \
    } while(0);

// For mac version.
#define lg_max(a,b) (((a) > (b)) ? (a) : (b))
#define lg_min(a,b) (((a) < (b)) ? (a) : (b))

#define LG_memset  memset
#define LG_memcpy  memcpy
#define LG_memmove memmove
//#define BlockMove(src, dest, num) LG_memmove(src, dest, num);

#define GAMEONLY                    1
#define SVGA_SUPPORT                1
#define USE_STEALTH                 1
#define USE_PFIELD                  1
#define DISTANCE_AI_KILL            1
#define TEXTURE_SELECTION           1
#define NO_HELP_STRINGS             1
#define NO_CORRUPT_SAVES            1
#define MAP_RESHIFTING              1
#define DIRAC_EDMS                  1
#define NO_ANTIGRAV_CRATES          1
#define DOOM_EMULATION_MODE         1
#define EDMS_SAFETY_NET             1
#define AUDIOLOGS                   1
#define SVGA_CUTSCENES              1
#define LOST_TREASURES_OF_MFD_GAMES 1
