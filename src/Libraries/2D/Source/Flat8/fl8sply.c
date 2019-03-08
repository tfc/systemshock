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
/*
 * $Source: r:/prj/lib/src/2d/RCS/fl8sply.c $
 * $Revision: 1.1 $
 * $Author: kevin $
 * $Date: 1994/08/16 13:11:44 $
 *
 * Routines for drawing flat shaded polygons onto a flat 8 canvas.
 *
 * This file is part of the 2d library.
 */

#include "cnvdat.h"
#include "gente.h"
#include "poly.h"
#include "tlucdat.h"
#include "tmapint.h"

// prototypes
int gri_spoly_loop(grs_tmap_loop_info *ti);
void gri_spoly_init(grs_tmap_loop_info *ti);
void gri_clut_spoly_init(grs_tmap_loop_info *ti);
void gri_stpoly_init(grs_tmap_loop_info *ti);
void gri_clut_stpoly_init(grs_tmap_loop_info *ti);

int gri_spoly_loop(grs_tmap_loop_info *ti) {
    int d;
    fix i, di;
    fix xl, xr;
    uchar *ti_d;
    fix ti_li, ti_ri;

    xl = ti->noname5.left.x;
    xr = ti->noname6.right.x;
    ti_li = ti->noname5.left.i;
    ti_ri = ti->noname6.right.i;
    ti_d = ti->noname1.d;

    do {
        i = ti_li;
        di = fix_div(ti_ri - i, xr - xl);
        i += fix_mul(fix_ceil(xl) - xl, di);

        if ((d = fix_cint(xr) - fix_cint(xl)) > 0) {
            switch (ti->noname4.bm.hlog) {
                int x;
              case GRL_OPAQUE:
                  for (x = fix_cint(xl); x < fix_cint(xr); x++) {
                      ti_d[x] = fix_fint(i);
                      i += di;
                  }
                  break;
              case GRL_CLUT:
                  for (x = fix_cint(xl); x < fix_cint(xr); x++) {
                      ti_d[x] = ti->clut[fix_fint(i)];
                      i += di;
                  }
                  break;
              case GRL_TLUC8:
                  for (x = fix_cint(xl); x < fix_cint(xr); x++) {
                      ti_d[x] = tluc8stab[fix_light(i) + ti_d[x]];
                      i += di;
                  }
                  break;
              case GRL_CLUT | GRL_TLUC8:
                  for (x = fix_cint(xl); x < fix_cint(xr); x++) {
                      ti_d[x] = ti->clut[tluc8stab[fix_light(i) + ti_d[x]]];
                      i += di;
                  }
                  break;
            }
        } else if (d < 0) {
            return TRUE;
        }
        /* update span extrema and destination. */
        ti_d += grd_bm.row;
        xl += ti->noname5.left.noname.dx;
        xr += ti->noname6.right.noname.dx;
        ti_li += ti->noname5.left.di;
        ti_ri += ti->noname6.right.di;
    } while ((--(ti->n)) > 0);

    ti->noname1.d = ti_d;
    ti->noname5.left.x = xl;
    ti->noname6.right.x = xr;
    ti->noname5.left.i = ti_li;
    ti->noname6.right.i = ti_ri;

    return FALSE;
}

void gri_spoly_init(grs_tmap_loop_info *ti) {
    ti->noname4.bm.hlog = GRL_OPAQUE;
    ti->noname1.d = ti->noname1.y * grd_bm.row + grd_bm.bits;
    ti->loop_func = (void (*)())gri_spoly_loop;
    ti->noname8.top_edge_func = (void (*)())gri_ix_edge;
    ti->noname9.bot_edge_func = (void (*)())gri_ix_edge;
}

void gri_clut_spoly_init(grs_tmap_loop_info *ti) {
    ti->noname4.bm.hlog = GRL_CLUT;
    ti->noname1.d = ti->noname1.y * grd_bm.row + grd_bm.bits;
    ti->loop_func = (void (*)())gri_spoly_loop;
    ti->noname8.top_edge_func = (void (*)())gri_ix_edge;
    ti->noname9.bot_edge_func = (void (*)())gri_ix_edge;
}

void gri_stpoly_init(grs_tmap_loop_info *ti) {
    if (tluc8stab != NULL)
        ti->noname4.bm.hlog = GRL_TLUC8;
    else
        ti->noname4.bm.hlog = GRL_OPAQUE;
    ti->noname1.d = ti->noname1.y * grd_bm.row + grd_bm.bits;
    ti->loop_func = (void (*)())gri_spoly_loop;
    ti->noname8.top_edge_func = (void (*)())gri_ix_edge;
    ti->noname9.bot_edge_func = (void (*)())gri_ix_edge;
}

void gri_clut_stpoly_init(grs_tmap_loop_info *ti) {
    if (tluc8stab != NULL)
        ti->noname4.bm.hlog = GRL_CLUT | GRL_TLUC8;
    else
        ti->noname4.bm.hlog = GRL_CLUT;
    ti->noname1.d = ti->noname1.y * grd_bm.row + grd_bm.bits;
    ti->loop_func = (void (*)())gri_spoly_loop;
    ti->noname8.top_edge_func = (void (*)())gri_ix_edge;
    ti->noname9.bot_edge_func = (void (*)())gri_ix_edge;
}
