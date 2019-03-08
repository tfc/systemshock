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
 * $Source: r:/prj/lib/src/2d/RCS/fl8nl.c $
 * $Revision: 1.3 $
 * $Author: kevin $
 * $Date: 1994/08/16 13:12:41 $
 *
 * Routines to linearly texture map a flat8 bitmap to a generic canvas.
 *
 * This file is part of the 2d library.
 *
 */

#include "cnvdat.h"
#include "fl8tf.h"
#include "fl8tmapdv.h"
#include "gente.h"
#include "plytyp.h"
#include "poly.h"
#include "scrdat.h"
#include "tlucdat.h"
#include "tmapint.h"
#include "vtab.h"

// prototypes
int gri_tluc8_lin_umap_loop(grs_tmap_loop_info *tli);

int gri_tluc8_lin_umap_loop(grs_tmap_loop_info *tli)
{
    fix u = tli->noname5.left.u, du = tli->noname6.right.u - u;
    fix v = tli->noname5.left.v, dv = tli->noname6.right.v - v;
    long *t_vtab = tli->vtab;
    uchar *t_bits = tli->noname4.bm.bits;
    uchar *t_clut = tli->clut;
    ulong t_mask = tli->noname7.mask;
    uchar t_wlog = tli->noname4.bm.wlog;
    //uchar temp_pix;

    while (tli->n)
    {
        fix dx = tli->noname6.right.x - tli->noname5.left.x;
        if (dx <= 0) return TRUE; //might divide by zero below; punt this tmap

        uchar *p       = tli->noname1.d + fix_cint(tli->noname5.left.x);
        uchar *p_final = tli->noname1.d + fix_cint(tli->noname6.right.x);

        du = fix_div(du, dx);
        dv = fix_div(dv, dx);

        switch (tli->noname4.bm.hlog)
        {
          case GRL_OPAQUE:
              for (; p < p_final; p++)
              {
                  int k = t_vtab[fix_fint(v)] + fix_fint(u);
                  k = t_bits[k];
                  if (tluc8tab[k] != NULL) *p = tluc8tab[k][*p]; else *p = k;
                  u += du;
                  v += dv;
              }
              break;

          case GRL_TRANS:
              for (; p < p_final; p++)
              {
                  int k = t_vtab[fix_fint(v)] + fix_fint(u);
                  if ((k = t_bits[k]))
                  {
                      if (tluc8tab[k] != NULL) *p = tluc8tab[k][*p]; else *p = k;
                  }
                  u += du;
                  v += dv;
              }
              break;

          case GRL_OPAQUE | GRL_LOG2:
              for (; p < p_final; p++)
              {
                  int k = ((fix_fint(v) << t_wlog) + fix_fint(u)) & t_mask;
                  k = t_bits[k];
                  if (tluc8tab[k] != NULL) *p = tluc8tab[k][*p]; else *p = k;
                  u += du;
                  v += dv;
              }
              break;

          case GRL_TRANS | GRL_LOG2:
              for (; p < p_final; p++)
              {
                  int k = ((fix_fint(v) << t_wlog) + fix_fint(u)) & t_mask;
                  if ((k = t_bits[k]))
                  {
                      if (tluc8tab[k] != NULL) *p = tluc8tab[k][*p]; else *p = k;
                  }
                  u += du;
                  v += dv;
              }
              break;

          case GRL_OPAQUE | GRL_CLUT:
              for (; p < p_final; p++)
              {
                  int k = t_vtab[fix_fint(v)] + fix_fint(u);
                  k = t_bits[k];
                  if (tluc8tab[k] != NULL) *p = t_clut[tluc8tab[k][*p]]; else *p = t_clut[k];
                  u += du;
                  v += dv;
              }
              break;

          case GRL_TRANS | GRL_CLUT:
              for (; p < p_final; p++)
              {
                  int k = t_vtab[fix_fint(v)] + fix_fint(u);
                  if ((k = t_bits[k]))
                  {
                      if (tluc8tab[k] != NULL) *p = t_clut[tluc8tab[k][*p]]; else *p = t_clut[k];
                  }
                  u += du;
                  v += dv;
              }
              break;

          case GRL_OPAQUE | GRL_LOG2 | GRL_CLUT:
              for (; p < p_final; p++)
              {
                  int k = ((fix_fint(v) << t_wlog) + fix_fint(u)) & t_mask;
                  k = t_bits[k];
                  if (tluc8tab[k] != NULL) *p = t_clut[tluc8tab[k][*p]]; else *p = t_clut[k];
                  u += du;
                  v += dv;
              }
              break;

          case GRL_TRANS | GRL_LOG2 | GRL_CLUT:
              for (; p < p_final; p++)
              {
                  int k = ((fix_fint(v) << t_wlog) + fix_fint(u)) & t_mask;
                  if ((k = t_bits[k]))
                  {
                      if (tluc8tab[k] != NULL) *p = t_clut[tluc8tab[k][*p]]; else *p = t_clut[k];
                  }
                  u += du;
                  v += dv;
              }
              break;
        }

        u = (tli->noname5.left.u += tli->noname5.left.du);
        tli->noname6.right.u += tli->noname6.right.du;
        du = tli->noname6.right.u - u;

        v = (tli->noname5.left.v += tli->noname5.left.dv);
        tli->noname6.right.v += tli->noname6.right.dv;
        dv = tli->noname6.right.v - v;

        tli->noname5.left.x += tli->noname5.left.noname.dx;
        tli->noname6.right.x += tli->noname6.right.noname.dx;

        tli->noname1.d += grd_bm.row;
        tli->n --;
    }

    return FALSE; //tmap OK
}

void gri_tluc8_trans_lin_umap_init(grs_tmap_loop_info *tli) {
    if ((tli->noname4.bm.row == (1 << tli->noname4.bm.wlog)) && (tli->noname4.bm.h == (1 << tli->noname4.bm.hlog))) {
        tli->noname7.mask = (1 << (tli->noname4.bm.hlog + tli->noname4.bm.wlog)) - 1;
        tli->noname4.bm.hlog = GRL_TRANS | GRL_LOG2;
    } else {
        tli->vtab = gr_make_vtab(&(tli->noname4.bm));
        tli->noname4.bm.hlog = GRL_TRANS;
    }
    tli->noname1.d = grd_bm.bits + grd_bm.row * tli->noname1.y;
    tli->loop_func = (void (*)())gri_tluc8_lin_umap_loop;
    tli->noname9.right_edge_func = (void (*)())gri_uvx_edge;
    tli->noname8.left_edge_func = (void (*)())gri_uvx_edge;
}

void gri_tluc8_opaque_lin_umap_init(grs_tmap_loop_info *tli) {
    if ((tli->noname4.bm.row == (1 << tli->noname4.bm.wlog)) && (tli->noname4.bm.h == (1 << tli->noname4.bm.hlog))) {
        tli->noname7.mask = (1 << (tli->noname4.bm.hlog + tli->noname4.bm.wlog)) - 1;
        tli->noname4.bm.hlog = GRL_OPAQUE | GRL_LOG2;
    } else {
        tli->vtab = gr_make_vtab(&(tli->noname4.bm));
        tli->noname4.bm.hlog = GRL_OPAQUE;
    }
    tli->noname1.d = grd_bm.bits + grd_bm.row * tli->noname1.y;
    tli->loop_func = (void (*)())gri_tluc8_lin_umap_loop;
    tli->noname9.right_edge_func = (void (*)())gri_uvx_edge;
    tli->noname8.left_edge_func = (void (*)())gri_uvx_edge;
}

void gri_tluc8_trans_clut_lin_umap_init(grs_tmap_loop_info *tli) {
    if ((tli->noname4.bm.row == (1 << tli->noname4.bm.wlog)) && (tli->noname4.bm.h == (1 << tli->noname4.bm.hlog))) {
        tli->noname7.mask = (1 << (tli->noname4.bm.hlog + tli->noname4.bm.wlog)) - 1;
        tli->noname4.bm.hlog = GRL_TRANS | GRL_LOG2 | GRL_CLUT;
    } else {
        tli->vtab = gr_make_vtab(&(tli->noname4.bm));
        tli->noname4.bm.hlog = GRL_TRANS | GRL_CLUT;
    }
    tli->noname1.d = grd_bm.bits + grd_bm.row * tli->noname1.y;
    tli->loop_func = (void (*)())gri_tluc8_lin_umap_loop;
    tli->noname9.right_edge_func = (void (*)())gri_uvx_edge;
    tli->noname8.left_edge_func = (void (*)())gri_uvx_edge;
}

void gri_tluc8_opaque_clut_lin_umap_init(grs_tmap_loop_info *tli) {
    if ((tli->noname4.bm.row == (1 << tli->noname4.bm.wlog)) && (tli->noname4.bm.h == (1 << tli->noname4.bm.hlog))) {
        tli->noname7.mask = (1 << (tli->noname4.bm.hlog + tli->noname4.bm.wlog)) - 1;
        tli->noname4.bm.hlog = GRL_OPAQUE | GRL_LOG2 | GRL_CLUT;
    } else {
        tli->vtab = gr_make_vtab(&(tli->noname4.bm));
        tli->noname4.bm.hlog = GRL_OPAQUE | GRL_CLUT;
    }
    tli->noname1.d = grd_bm.bits + grd_bm.row * tli->noname1.y;
    tli->loop_func = (void (*)())gri_tluc8_lin_umap_loop;
    tli->noname9.right_edge_func = (void (*)())gri_uvx_edge;
    tli->noname8.left_edge_func = (void (*)())gri_uvx_edge;
}
