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
 * $Source: r:/prj/lib/src/2d/RCS/fl8ns.c $
 * $Revision: 1.1 $
 * $Author: kevin $
 * $Date: 1994/08/16 13:13:12 $
 *
 * Routines to scale a flat8 bitmap to a generic canvas.
 *
 * This file is part of the 2d library.
 *
 */

#include "cnvdat.h"
#include "fl8tf.h"
#include "gente.h"
#include "grnull.h"
#include "poly.h"
#include "tlucdat.h"
#include "tmapint.h"
#include "vtab.h"

// prototypes
int gri_tluc8_scale_umap_loop(grs_tmap_loop_info *tli);

int gri_tluc8_scale_umap_loop(grs_tmap_loop_info *tli) {
    fix u, ul, du;
    uchar *pl, *pr;

    pl = tli->noname1.d + fix_cint(tli->noname5.left.x);
    pr = tli->noname1.d + fix_cint(tli->noname6.right.x);
    if (pr <= pl)
        return TRUE;
    ul = tli->noname5.left.u;
    du = fix_div(tli->noname6.right.u - ul, tli->noname6.right.x - tli->noname5.left.x);
    ul += fix_mul(du, fix_ceil(tli->noname5.left.x) - tli->noname5.left.x);
    do {
        uchar *p_dst, k;
        uchar *p_src = tli->noname4.bm.bits + tli->noname4.bm.row * fix_int(tli->noname5.left.v);
        switch (tli->noname4.bm.hlog) {
          case GRL_OPAQUE:
              for (p_dst = pl, u = ul; p_dst < pr; p_dst++) {
                  k = p_src[fix_fint(u)];
                  if (tluc8tab[k] != NULL)
                      *p_dst = tluc8tab[k][*p_dst];
                  else
                      *p_dst = k;
                  u += du;
              }
              break;
          case GRL_TRANS:
              for (p_dst = pl, u = ul; p_dst < pr; p_dst++) {
                  if ((k = p_src[fix_fint(u)])) {
                      if (tluc8tab[k] != NULL)
                          *p_dst = tluc8tab[k][*p_dst];
                      else
                          *p_dst = k;
                  }
                  u += du;
              }
              break;
          case GRL_OPAQUE | GRL_CLUT:
              for (p_dst = pl, u = ul; p_dst < pr; p_dst++) {
                  k = p_src[fix_fint(u)];
                  if (tluc8tab[k] != NULL)
                      *p_dst = tli->clut[tluc8tab[k][*p_dst]];
                  else
                      *p_dst = tli->clut[k];
                  u += du;
              }
              break;
          case GRL_TRANS | GRL_CLUT:
              for (p_dst = pl, u = ul; p_dst < pr; p_dst++) {
                  if ((k = p_src[fix_fint(u)])) {
                      if (tluc8tab[k] != NULL)
                          *p_dst = tli->clut[tluc8tab[k][*p_dst]];
                      else
                          *p_dst = tli->clut[k];
                  }
                  u += du;
              }
              break;
        }
        tli->noname5.left.v += tli->noname5.left.dv;
        pl += grd_bm.row;
        pr += grd_bm.row;
    } while (--(tli->n) > 0);
    return FALSE; /* tmap OK */
}

void gri_tluc8_trans_scale_umap_init(grs_tmap_loop_info *tli) {
    tli->noname4.bm.hlog = GRL_TRANS;
    tli->noname1.d = grd_bm.bits + grd_bm.row * tli->noname1.y;
    tli->loop_func = (void (*)())gri_tluc8_scale_umap_loop;
    tli->noname9.right_edge_func = gr_null;
    tli->noname8.left_edge_func = (void (*)())gri_scale_edge;
}

void gri_tluc8_opaque_scale_umap_init(grs_tmap_loop_info *tli) {
    tli->noname4.bm.hlog = GRL_OPAQUE;
    tli->noname1.d = grd_bm.bits + grd_bm.row * tli->noname1.y;
    tli->loop_func = (void (*)())gri_tluc8_scale_umap_loop;
    tli->noname9.right_edge_func = gr_null;
    tli->noname8.left_edge_func = (void (*)())gri_scale_edge;
}

void gri_tluc8_trans_clut_scale_umap_init(grs_tmap_loop_info *tli) {
    tli->noname4.bm.hlog = GRL_TRANS | GRL_CLUT;
    tli->noname1.d = grd_bm.bits + grd_bm.row * tli->noname1.y;
    tli->loop_func = (void (*)())gri_tluc8_scale_umap_loop;
    tli->noname9.right_edge_func = gr_null;
    tli->noname8.left_edge_func = (void (*)())gri_scale_edge;
}

void gri_tluc8_opaque_clut_scale_umap_init(grs_tmap_loop_info *tli) {
    tli->noname4.bm.hlog = GRL_OPAQUE | GRL_CLUT;
    tli->noname1.d = grd_bm.bits + grd_bm.row * tli->noname1.y;
    tli->loop_func = (void (*)())gri_tluc8_scale_umap_loop;
    tli->noname9.right_edge_func = gr_null;
    tli->noname8.left_edge_func = (void (*)())gri_scale_edge;
}
