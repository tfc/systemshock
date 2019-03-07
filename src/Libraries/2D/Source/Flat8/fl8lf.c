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
 * $Source: r:/prj/lib/src/2d/RCS/genlf.c $
 * $Revision: 1.3 $
 * $Author: kevin $
 * $Date: 1994/08/16 12:50:14 $
 *
 * Routines to floor texture map a flat8 bitmap to a generic canvas.
 *
 * This file is part of the 2d library.
 *
 */

#include "cnvdat.h"
#include "fl8tf.h"
#include "fl8tmapdv.h"
#include "gente.h"
#include "grpix.h"
#include "poly.h"
#include "scrmac.h"
#include "tmapint.h"
#include "vtab.h"

int gri_lit_floor_umap_loop(grs_tmap_loop_info *tli);

int gri_lit_floor_umap_loop(grs_tmap_loop_info *tli) {

#if InvDiv
    fix inv = fix_div(fix_make(1, 0), tli->w);
    fix u = fix_mul_asm_safe(tli->noname5.left.u, inv);
    fix du = fix_mul_asm_safe(tli->noname6.right.u, inv) - u;
    fix v = fix_mul_asm_safe(tli->noname5.left.v, inv);
    fix dv = fix_mul_asm_safe(tli->noname6.right.v, inv) - v;
    fix i = fix_mul_asm_safe(tli->noname5.left.i, inv);
    fix di = fix_mul_asm_safe(tli->noname6.right.i, inv) - i;
#else
    fix u = fix_div(tli->noname5.left.u, tli->w);
    fix du = fix_div(tli->noname6.right.u, tli->w) - u;
    fix v = fix_div(tli->noname5.left.v, tli->w);
    fix dv = fix_div(tli->noname6.right.v, tli->w) - v;
    fix i = fix_div(tli->noname5.left.i, tli->w);
    fix di = fix_div(tli->noname6.right.i, tli->w) - i;
#endif

    ulong t_mask = tli->noname7.mask;
    uchar t_wlog = tli->noname4.bm.wlog;
    uchar *g_ltab = grd_screen->ltab;
    long *t_vtab = tli->vtab;
    uchar *t_bits = tli->noname4.bm.bits;

    do {
        fix dx = tli->noname6.right.x - tli->noname5.left.x;
        if (dx > 0)
        {

#if InvDiv
            inv = fix_div(fix_make(1, 0) << 8, dx);
            di = fix_mul_asm_safe_light(di, inv);
            inv >>= 8;
            du = fix_mul_asm_safe(du, inv);
            dv = fix_mul_asm_safe(dv, inv);
#else
            du = fix_div(du, dx);
            dv = fix_div(dv, dx);
            di = fix_div(di, dx);
#endif

            fix d = fix_ceil(tli->noname5.left.x) - tli->noname5.left.x;
            u += fix_mul(du, d);
            v += fix_mul(dv, d);
            i += fix_mul(di, d);

            uchar *p_dest = grd_bm.bits + (grd_bm.row * tli->noname1.y) + fix_cint(tli->noname5.left.x);

            int x = fix_cint(tli->noname6.right.x) - fix_cint(tli->noname5.left.x);

            switch (tli->noname4.bm.hlog) {
              case GRL_OPAQUE:
                  for (; x > 0; x--) {
                      int k = t_vtab[fix_fint(v)] + fix_fint(u);
                      *(p_dest++) = g_ltab[t_bits[k] + fix_light(i)];
                      // gr_fill_upixel(g_ltab[t_bits[k]+fix_light(i)],x,t_y);
                  }
                  break;

              case GRL_TRANS:
                  for (; x > 0; x--) {
                      int k = t_vtab[fix_fint(v)] + fix_fint(u);
                      if ((k = t_bits[k])) *p_dest = g_ltab[k + fix_light(i)];
                      // gr_fill_upixel(g_ltab[k+fix_light(i)],x,t_y);
                      p_dest++;
                      u += du;
                      v += dv;
                      i += di;
                  }
                  break;

              case GRL_OPAQUE|GRL_LOG2:
                  for (; x > 0; x--) {
                      int k = ((fix_fint(v)<<t_wlog)+fix_fint(u))&t_mask;
                      *(p_dest++) = g_ltab[t_bits[k]+fix_light(i)];
                      // gr_fill_upixel(g_ltab[t_bits[k]+fix_light(i)],x,t_y);
                      u += du;
                      v += dv;
                      i += di;
                  }
                  break;

              case GRL_TRANS | GRL_LOG2:
                  for (; x > 0; x--) {
                      int k = ((fix_fint(v) << t_wlog) + fix_fint(u)) & t_mask;
                      if ((k = t_bits[k])) *p_dest = g_ltab[k + fix_light(i)];
                      // gr_fill_upixel(g_ltab[k+fix_light(i)],x,t_y);
                      p_dest++;
                      u += du;
                      v += dv;
                      i += di;
                  }
                  break;
            }
        }

        tli->w += tli->dw;

#if InvDiv
        inv = fix_div(fix_make(1, 0), tli->w);
        u = fix_mul_asm_safe((tli->noname5.left.u += tli->noname5.left.du), inv);
        tli->noname6.right.u += tli->noname6.right.du;
        du = fix_mul_asm_safe(tli->noname6.right.u, inv) - u;
        v = fix_mul_asm_safe((tli->noname5.left.v += tli->noname5.left.dv), inv);
        tli->noname6.right.v += tli->noname6.right.dv;
        dv = fix_mul_asm_safe(tli->noname6.right.v, inv) - v;
        i = fix_mul_asm_safe((tli->noname5.left.i += tli->noname5.left.di), inv);
        tli->noname6.right.i += tli->noname6.right.di;
        di = fix_mul_asm_safe(tli->noname6.right.i, inv) - i;
#else
        u = fix_div((tli->noname5.left.u += tli->noname5.left.du), tli->w);
        tli->noname6.right.u += tli->noname6.right.du;
        du = fix_div(tli->noname6.right.u, tli->w) - u;
        v = fix_div((tli->noname5.left.v += tli->noname5.left.dv), tli->w);
        tli->noname6.right.v += tli->noname6.right.dv;
        dv = fix_div(tli->noname6.right.v, tli->w) - v;
        i = fix_div((tli->noname5.left.i += tli->noname5.left.di), tli->w);
        tli->noname6.right.i += tli->noname6.right.di;
        di = fix_div(tli->noname6.right.i, tli->w) - i;
#endif

        tli->noname5.left.x += tli->noname5.left.noname.dx;
        tli->noname6.right.x += tli->noname6.right.noname.dx;

        tli->noname1.y++;

    } while (--(tli->n) > 0);

    return FALSE; // tmap OK
}

void gri_trans_lit_floor_umap_init(grs_tmap_loop_info *tli) {
    if ((tli->noname4.bm.row == (1 << tli->noname4.bm.wlog)) && (tli->noname4.bm.h == (1 << tli->noname4.bm.hlog))) {
        tli->noname7.mask = (1 << (tli->noname4.bm.hlog + tli->noname4.bm.wlog)) - 1;
        tli->noname4.bm.hlog = GRL_TRANS | GRL_LOG2;
    } else {
        tli->vtab = gr_make_vtab(&(tli->noname4.bm));
        tli->noname4.bm.hlog = GRL_TRANS;
    }
    tli->loop_func = (void (*)())gri_lit_floor_umap_loop;
    tli->noname8.left_edge_func = (void (*)())gri_uviwx_edge;
    tli->noname9.right_edge_func = (void (*)())gri_uviwx_edge;
}

void gri_opaque_lit_floor_umap_init(grs_tmap_loop_info *tli) {
    if ((tli->noname4.bm.row == (1 << tli->noname4.bm.wlog)) && (tli->noname4.bm.h == (1 << tli->noname4.bm.hlog))) {
        tli->noname7.mask = (1 << (tli->noname4.bm.hlog + tli->noname4.bm.wlog)) - 1;
        tli->noname4.bm.hlog = GRL_OPAQUE | GRL_LOG2;
    } else {
        tli->vtab = gr_make_vtab(&(tli->noname4.bm));
        tli->noname4.bm.hlog = GRL_OPAQUE;
    }
    tli->loop_func = (void (*)())gri_lit_floor_umap_loop;
    tli->noname8.left_edge_func = (void (*)())gri_uviwx_edge;
    tli->noname9.right_edge_func = (void (*)())gri_uviwx_edge;
}
