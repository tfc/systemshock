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
 * $Source: r:/prj/lib/src/2d/RCS/fl8cply.c $
 * $Revision: 1.2 $
 * $Author: kevin $
 * $Date: 1994/10/17 14:59:58 $
 *
 * Routines for drawing flat shaded polygons onto a flat 8 canvas.
 *
 * This file is part of the 2d library.
 */

#include "cnvdat.h"
#include "fix.h"
#include "gente.h"
#include "poly.h"
#include "rgb.h"
#include "scrdat.h"
#include "tmapint.h"

// prototypes
int gri_cpoly_loop(grs_tmap_loop_info *ti);
void gri_cpoly_init(grs_tmap_loop_info *ti);
void gri_clut_cpoly_init(grs_tmap_loop_info *ti);

int gri_cpoly_loop(grs_tmap_loop_info *ti) {
    int x, d;
    fix dx, frac;
    fix r, g, b, dr, dg, db;

    do {
        dx = ti->noname6.right.x - ti->noname5.left.x;
        frac = fix_ceil(ti->noname5.left.x) - ti->noname5.left.x;

        r = ti->noname5.left.u;
        dr = fix_div(ti->noname6.right.u - r, dx);
        r += fix_mul(frac, dr);

        g = ti->noname5.left.v;
        dg = fix_div(ti->noname6.right.v - g, dx);
        g += fix_mul(frac, dg);

        b = ti->noname5.left.i;
        db = fix_div(ti->noname6.right.i - b, dx);
        b += fix_mul(frac, db);

        if ((d = fix_cint(ti->noname6.right.x) - fix_cint(ti->noname5.left.x)) > 0) {
            switch (ti->noname4.bm.hlog) {
              case GRL_OPAQUE:
                  for (x = fix_cint(ti->noname5.left.x); x < fix_cint(ti->noname6.right.x); x++) {
                      int j = gr_index_rgb(r, g, b);
                      ti->noname1.d[x] = grd_ipal[j];
                      r += dr, g += dg, b += db;
                  }
                  break;
              case GRL_CLUT:
                  for (x = fix_cint(ti->noname5.left.x); x < fix_cint(ti->noname6.right.x); x++) {
                      int j = gr_index_rgb(r, g, b);
                      ti->noname1.d[x] = ti->clut[grd_ipal[j]];
                      r += dr, g += dg, b += db;
                  }
                  break;
            }
        } else if (d < 0) {
            return TRUE;
        }
        /* update span extrema and destination. */
        ti->noname5.left.x  += ti->noname5.left.noname.dx;
        ti->noname6.right.x += ti->noname6.right.noname.dx;
        ti->noname5.left.u  += ti->noname5.left.du;
        ti->noname6.right.u += ti->noname6.right.du;
        ti->noname5.left.v  += ti->noname5.left.dv;
        ti->noname6.right.v += ti->noname6.right.dv;
        ti->noname5.left.i  += ti->noname5.left.di;
        ti->noname6.right.i += ti->noname6.right.di;
        ti->noname1.d       += grd_bm.row;
    } while ((--(ti->n)) > 0);
    return FALSE;
}

void gri_cpoly_init(grs_tmap_loop_info *ti) {
    ti->noname4.bm.hlog = GRL_OPAQUE;
    ti->noname1.d = ti->noname1.y * grd_bm.row + grd_bm.bits;
    ti->loop_func = (void (*)())gri_cpoly_loop;
    ti->noname8.top_edge_func = (void (*)())gri_rgbx_edge;
    ti->noname9.bot_edge_func = (void (*)())gri_rgbx_edge;
}

void gri_clut_cpoly_init(grs_tmap_loop_info *ti) {
    ti->noname4.bm.hlog = GRL_CLUT;
    ti->noname1.d = ti->noname1.y * grd_bm.row + grd_bm.bits;
    ti->loop_func = (void (*)())gri_cpoly_loop;
    ti->noname8.top_edge_func = (void (*)())gri_rgbx_edge;
    ti->noname9.bot_edge_func = (void (*)())gri_rgbx_edge;
}
