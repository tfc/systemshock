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
// Implementation solid transparent mappers
//
// This file is part of the 2d library.
//

#include "cnvdat.h"
#include "fl8tf.h"
#include "gente.h"
#include "grpix.h"
#include "poly.h"
#include "tmapint.h"
#include "vtab.h"

// prototypes
int gri_trans_solid_lin_umap_loop(grs_tmap_loop_info *tli);
int gri_trans_solid_floor_umap_loop(grs_tmap_loop_info *tli);
int gri_solid_wall_umap_loop(grs_tmap_loop_info *tli);
void gri_trans_solid_per_umap_hscan_scanline(grs_per_info *pi, grs_bitmap *bm);
void gri_trans_solid_per_umap_vscan_scanline(grs_per_info *pi, grs_bitmap *bm);

int gri_trans_solid_lin_umap_loop(grs_tmap_loop_info *tli) {
    fix u, v, du, dv, dx, d;

    // locals used to store copies of tli-> stuff, so its in registers on the PPC
    int x;
    uchar solid_color;
    int t_xl, t_xr;
    uchar *p_dest;
    long *t_vtab;
    uchar *t_bits;
    //uchar *t_clut;
    uchar t_wlog;
    ulong t_mask;
    long gr_row;
    uchar *start_pdest;

    //solid_color = (uchar)tli->clut;
    solid_color = *(uchar *)tli->clut;
    u = tli->noname5.left.u;
    du = tli->noname6.right.u - u;
    v = tli->noname5.left.v;
    dv = tli->noname6.right.v - v;
    dx = tli->noname6.right.x - tli->noname5.left.x;

    t_vtab = tli->vtab;
    t_mask = tli->noname7.mask;
    t_wlog = tli->noname4.bm.wlog;

    t_bits = tli->noname4.bm.bits;
    gr_row = grd_bm.row;
    start_pdest = grd_bm.bits + (gr_row * (tli->noname1.y));

    do {
        if ((d = fix_ceil(tli->noname6.right.x) - fix_ceil(tli->noname5.left.x)) > 0) {
            d = fix_ceil(tli->noname5.left.x) - tli->noname5.left.x;
            du = fix_div(du, dx);
            dv = fix_div(dv, dx);
            u += fix_mul(du, d);
            v += fix_mul(dv, d);

            // copy out tli-> stuff into locals
            t_xl = fix_cint(tli->noname5.left.x);
            t_xr = fix_cint(tli->noname6.right.x);
            p_dest = start_pdest + t_xl;

            if (tli->noname4.bm.hlog == GRL_TRANS) {
                for (x = t_xl; x < t_xr; x++) {
                    if (t_bits[t_vtab[fix_fint(v)] + fix_fint(u)])
                        *p_dest = solid_color; // gr_fill_upixel(t_bits[k],x,y);
                    p_dest++;
                    u += du;
                    v += dv;
                }
            } else {
                for (x = t_xl; x < t_xr; x++) {
                    if (t_bits[((fix_fint(v) << t_wlog) + fix_fint(u)) & t_mask])
                        *p_dest = solid_color; // gr_fill_upixel(t_bits[k],x,y);
                    p_dest++;
                    u += du;
                    v += dv;
                }
            }
        } else if (d < 0)
            return TRUE; /* punt this tmap */

        u = (tli->noname5.left.u += tli->noname5.left.du);
        tli->noname6.right.u += tli->noname6.right.du;
        du = tli->noname6.right.u - u;
        v = (tli->noname5.left.v += tli->noname5.left.dv);
        tli->noname6.right.v += tli->noname6.right.dv;
        dv = tli->noname6.right.v - v;
        tli->noname5.left.x += tli->noname5.left.noname.dx;
        tli->noname6.right.x += tli->noname6.right.noname.dx;
        dx = tli->noname6.right.x - tli->noname5.left.x;
        tli->noname1.y++;
        start_pdest += gr_row;
    } while (--(tli->n) > 0);

    return FALSE; /* tmap OK */
}

void gri_trans_solid_lin_umap_init(grs_tmap_loop_info *tli) {
    if ((tli->noname4.bm.row == (1 << tli->noname4.bm.wlog)) && (tli->noname4.bm.h == (1 << tli->noname4.bm.hlog))) {
        tli->noname7.mask = (1 << (tli->noname4.bm.hlog + tli->noname4.bm.wlog)) - 1;
        tli->noname4.bm.hlog = GRL_TRANS | GRL_LOG2;
    } else {
        tli->vtab = gr_make_vtab(&(tli->noname4.bm));
        tli->noname4.bm.hlog = GRL_TRANS;
    }
    tli->loop_func = (void (*)())gri_trans_solid_lin_umap_loop;
    tli->noname9.right_edge_func = (void (*)())gri_uvx_edge;
    tli->noname8.left_edge_func = (void (*)())gri_uvx_edge;
}

int gri_trans_solid_floor_umap_loop(grs_tmap_loop_info *tli) {
    fix u, v, du, dv, dx, d;
    uchar solid_color;
    int x;
    // locals used to store copies of tli-> stuff, so its in registers on the PPC
    int t_xl, t_xr, t_y, gr_row;
    long *t_vtab;
    uchar *t_bits;
    uchar *p_dest;
    //uchar temp_pix;
    uchar t_wlog;
    ulong t_mask;

    (void)gr_row;

    //solid_color = (uchar)tli->clut;
    solid_color = *(uchar *)tli->clut;
    u = fix_div(tli->noname5.left.u, tli->w);
    du = fix_div(tli->noname6.right.u, tli->w) - u;
    v = fix_div(tli->noname5.left.v, tli->w);
    dv = fix_div(tli->noname6.right.v, tli->w) - v;
    dx = tli->noname6.right.x - tli->noname5.left.x;

    t_mask = tli->noname7.mask;
    t_wlog = tli->noname4.bm.wlog;
    t_vtab = tli->vtab;
    t_bits = tli->noname4.bm.bits;
    gr_row = grd_bm.row;

    do {
        if ((d = fix_ceil(tli->noname6.right.x) - fix_ceil(tli->noname5.left.x)) > 0) {
            d = fix_ceil(tli->noname5.left.x) - tli->noname5.left.x;
            du = fix_div(du, dx);
            u += fix_mul(du, d);
            dv = fix_div(dv, dx);
            v += fix_mul(dv, d);

            // copy out tli-> stuff into locals
            t_xl = fix_cint(tli->noname5.left.x);
            t_xr = fix_cint(tli->noname6.right.x);
            t_y = tli->noname1.y;
            p_dest = grd_bm.bits + (grd_bm.row * t_y) + t_xl;

            if (tli->noname4.bm.hlog == GRL_TRANS) {
                for (x = t_xl; x < t_xr; x++) {
                    int k = t_vtab[fix_fint(v)] + fix_fint(u);
                    if (t_bits[k])
                        *p_dest = solid_color; // gr_fill_upixel(t_bits[k],x,t_y);
                    p_dest++;
                    u += du;
                    v += dv;
                }
            } else {
                for (x = t_xl; x < t_xr; x++) {
                    int k = ((fix_fint(v) << t_wlog) + fix_fint(u)) & t_mask;
                    if (t_bits[k])
                        *p_dest = solid_color; // gr_fill_upixel(t_bits[k],x,t_y);
                    p_dest++;
                    u += du;
                    v += dv;
                }
            }
        } else if (d < 0)
            return TRUE; /* punt this tmap */
        tli->w += tli->dw;
        u = fix_div((tli->noname5.left.u += tli->noname5.left.du), tli->w);
        tli->noname6.right.u += tli->noname6.right.du;
        du = fix_div(tli->noname6.right.u, tli->w) - u;
        v = fix_div((tli->noname5.left.v += tli->noname5.left.dv), tli->w);
        tli->noname6.right.v += tli->noname6.right.dv;
        dv = fix_div(tli->noname6.right.v, tli->w) - v;
        tli->noname5.left.x += tli->noname5.left.noname.dx;
        tli->noname6.right.x += tli->noname6.right.noname.dx;
        dx = tli->noname6.right.x - tli->noname5.left.x;
        tli->noname1.y++;
    } while (--(tli->n) > 0);
    return FALSE; /* tmap OK */
}

void gri_trans_solid_floor_umap_init(grs_tmap_loop_info *tli) {
    if ((tli->noname4.bm.row == (1 << tli->noname4.bm.wlog)) && (tli->noname4.bm.h == (1 << tli->noname4.bm.hlog))) {
        tli->noname7.mask = (1 << (tli->noname4.bm.hlog + tli->noname4.bm.wlog)) - 1;
        tli->noname4.bm.hlog = GRL_TRANS | GRL_LOG2;
    } else {
        tli->vtab = gr_make_vtab(&(tli->noname4.bm));
        tli->noname4.bm.hlog = GRL_TRANS;
    }
    tli->loop_func = (void (*)())gri_trans_solid_floor_umap_loop;
    tli->noname8.left_edge_func = (void (*)())gri_uvwx_edge;
    tli->noname9.right_edge_func = (void (*)())gri_uvwx_edge;
}

int gri_solid_wall_umap_loop(grs_tmap_loop_info *tli) {
    fix u, v, du, dv, dy, d;
    uchar solid_color;

    // locals used to store copies of tli-> stuff, so its in registers on the PPC
    int t_yl, t_yr;
    long *t_vtab;
    uchar *t_bits;
    uchar *p_dest;
    //uchar *t_clut;
    uchar t_wlog;
    ulong t_mask;
    long gr_row;
    int y;

    //solid_color = (uchar)tli->clut;
    solid_color = *(uchar *)tli->clut;
    u = fix_div(tli->noname5.left.u, tli->w);
    du = fix_div(tli->noname6.right.u, tli->w) - u;
    v = fix_div(tli->noname5.left.v, tli->w);
    dv = fix_div(tli->noname6.right.v, tli->w) - v;
    dy = tli->noname6.right.y - tli->noname5.left.y;

    t_bits = tli->noname4.bm.bits;
    t_vtab = tli->vtab;
    t_mask = tli->noname7.mask;
    t_wlog = tli->noname4.bm.wlog;

    gr_row = grd_bm.row;

    // handle PowerPC loop
    do {
        if ((d = fix_ceil(tli->noname6.right.y) - fix_ceil(tli->noname5.left.y)) > 0) {

            d = fix_ceil(tli->noname5.left.y) - tli->noname5.left.y;
            du = fix_div(du, dy);
            dv = fix_div(dv, dy);
            u += fix_mul(du, d);
            v += fix_mul(dv, d);

            t_yl = fix_cint(tli->noname5.left.y);
            t_yr = fix_cint(tli->noname6.right.y);
            p_dest = grd_bm.bits + (gr_row * t_yl) + tli->noname1.x;

            if (tli->noname4.bm.hlog == GRL_TRANS) {
                for (y = t_yl; y < t_yr; y++) {
                    int k = t_vtab[fix_fint(v)] + fix_fint(u);
                    if (t_bits[k])
                        *p_dest = solid_color; // gr_fill_upixel(t_bits[k],t_x,y);
                    p_dest += gr_row;
                    u += du;
                    v += dv;
                }
            } else {
                for (y = t_yl; y < t_yr; y++) {
                    int k = ((fix_fint(v) << t_wlog) + fix_fint(u)) & t_mask;
                    if (t_bits[k])
                        *p_dest = solid_color; // gr_fill_upixel(t_bits[k],t_x,y);
                    p_dest += gr_row;
                    u += du;
                    v += dv;
                }
            }
        } else if (d < 0)
            return TRUE; /* punt this tmap */

        tli->w += tli->dw;
        u = fix_div((tli->noname5.left.u += tli->noname5.left.du), tli->w);
        tli->noname6.right.u += tli->noname6.right.du;
        du = fix_div(tli->noname6.right.u, tli->w) - u;
        v = fix_div((tli->noname5.left.v += tli->noname5.left.dv), tli->w);
        tli->noname6.right.v += tli->noname6.right.dv;
        dv = fix_div(tli->noname6.right.v, tli->w) - v;
        tli->noname5.left.y += tli->noname5.left.noname.dy;
        tli->noname6.right.y += tli->noname6.right.noname.dy;
        dy = tli->noname6.right.y - tli->noname5.left.y;
        tli->noname1.x++;

    } while (--(tli->n) > 0);

    return FALSE;
}

void gri_trans_solid_wall_umap_init(grs_tmap_loop_info *tli) {
    if ((tli->noname4.bm.row == (1 << tli->noname4.bm.wlog)) && (tli->noname4.bm.h == (1 << tli->noname4.bm.hlog))) {
        tli->noname7.mask = (1 << (tli->noname4.bm.hlog + tli->noname4.bm.wlog)) - 1;
        tli->noname4.bm.hlog = GRL_TRANS | GRL_LOG2;
    } else {
        tli->vtab = gr_make_vtab(&(tli->noname4.bm));
        tli->noname4.bm.hlog = GRL_TRANS;
    }
    tli->loop_func = (void (*)())gri_solid_wall_umap_loop;
    tli->noname8.left_edge_func = (void (*)())gri_uvwy_edge;
    tli->noname9.right_edge_func = (void (*)())gri_uvwy_edge;
}

void gri_trans_solid_per_umap_hscan_scanline(grs_per_info *pi, grs_bitmap *bm) {
    int y_cint;
    uchar *p;
    uchar solid_color;

    // locals used to speed PPC code
    fix l_u, l_v, l_du, l_dv, l_y_fix, l_scan_slope, l_dtl, l_dxl, l_dyl, l_dtr, l_dyr;
    int l_x, l_xl, l_xr, l_xr0, l_u_mask, l_v_mask, l_v_shift;
    int gr_row, temp_y;
    uchar *bm_bits;

    //solid_color = (uchar)pi->noname2.clut;
    solid_color = *(uchar *)pi->noname2.clut;
    gr_row = grd_bm.row;
    bm_bits = bm->bits;
    l_dyr = pi->dyr;
    l_dtr = pi->dtr;
    l_dyl = pi->dyl;
    l_dxl = pi->dxl;
    l_dtl = pi->dtl;
    l_scan_slope = pi->scan_slope;
    l_y_fix = pi->noname1.y_fix;
    l_v_shift = pi->v_shift;
    l_v_mask = pi->v_mask;
    l_u_mask = pi->u_mask;
    l_xr0 = pi->noname7.xr0;
    l_x = pi->noname4.x;
    l_xl = pi->noname5.xl;
    l_xr = pi->noname6.xr;
    l_u = pi->u;
    l_v = pi->v;
    l_du = pi->du;
    l_dv = pi->dv;

    l_y_fix = l_x * l_scan_slope + fix_make(pi->noname3.yp, 0xffff);
    l_u = pi->u0 + fix_div(pi->unum, pi->denom);
    l_v = pi->v0 + fix_div(pi->vnum, pi->denom);
    l_du = fix_div(pi->dunum, pi->denom);
    l_dv = fix_div(pi->dvnum, pi->denom);
    l_u += l_x * l_du;
    l_v += l_x * l_dv;

    y_cint = fix_int(l_y_fix);
    if (l_scan_slope < 0)
        gr_row = -gr_row;

    p = grd_bm.bits + l_x + y_cint * grd_bm.row;
    if (l_x < l_xl) {
        fix test = l_x * l_dyl - y_cint * l_dxl + pi->cl;
        for (; l_x < l_xl; l_x++) {
            if (test <= 0) {
                int k = (l_u >> 16) & l_u_mask;
                k += (l_v >> l_v_shift) & l_v_mask;
                if (bm_bits[k])
                    *p = solid_color; // gr_fill_upixel(bm_bits[k],l_x,y_cint);
            }
            temp_y = y_cint;
            y_cint = fix_int(l_y_fix += l_scan_slope);
            if (temp_y != y_cint) {
                test += l_dtl;
                p += gr_row;
            } else
                test += l_dyl;

            p++;
            l_u += l_du;
            l_v += l_dv;
        }
    }

    for (; l_x < l_xr0; l_x++) {
        int k = (l_u >> 16) & l_u_mask;
        k += (l_v >> l_v_shift) & l_v_mask;
        if (bm_bits[k])
            *p = solid_color; // gr_fill_upixel(bm_bits[k],l_x,y_cint);
        temp_y = y_cint;
        y_cint = fix_int(l_y_fix += l_scan_slope);
        if (temp_y != y_cint) // y_cint=fix_int((l_y_fix+=l_scan_slope));
        {
            temp_y -= y_cint;
            p += gr_row;
        }

        p++;
        l_u += l_du;
        l_v += l_dv;
    }

    if (l_x < l_xr) {
        fix test = l_x * l_dyr - y_cint * pi->dxr + pi->cr;
        p = grd_bm.bits + l_x + y_cint * grd_bm.row;
        for (; l_x < l_xr; l_x++) {
            if (test >= 0) {
                int k = (l_u >> 16) & l_u_mask;
                k += (l_v >> l_v_shift) & l_v_mask;
                if (bm_bits[k])
                    *p = solid_color; // gr_fill_upixel(bm_bits[k],l_x,y_cint);
            }
            temp_y = y_cint;
            y_cint = fix_int(l_y_fix += l_scan_slope);
            if (temp_y != y_cint) {
                test += l_dtr;
                p += gr_row;
            } else
                test += l_dyr;

            p++;
            l_u += l_du;
            l_v += l_dv;
        }
    }

    pi->noname1.y_fix = l_y_fix;
    pi->noname4.x = l_x;
    pi->u = l_u;
    pi->v = l_v;
    pi->du = l_du;
    pi->dv = l_dv;
}

void gri_trans_solid_per_umap_vscan_scanline(grs_per_info *pi, grs_bitmap *bm) {
    int x_cint;
    uchar solid_color;

    // locals used to speed PPC code
    fix l_dxr, l_x_fix, l_u, l_v, l_du, l_dv, l_scan_slope, l_dtl, l_dxl, l_dyl, l_dtr, l_dyr;
    int l_yl, l_yr0, l_yr, l_y, l_u_mask, l_v_mask, l_v_shift;
    int gr_row, temp_x;
    uchar *bm_bits;
    uchar *p;

    //solid_color = (uchar)pi->noname2.clut;
    solid_color = *(uchar *)pi->noname2.clut;
    gr_row = grd_bm.row;
    bm_bits = bm->bits;
    l_dxr = pi->dxr;
    l_x_fix = pi->noname1.x_fix;
    l_y = pi->noname4.y;
    l_yr = pi->noname6.yr;
    l_yr0 = pi->noname7.yr0;
    l_yl = pi->noname5.yl;
    l_dyr = pi->dyr;
    l_dtr = pi->dtr;
    l_dyl = pi->dyl;
    l_dxl = pi->dxl;
    l_dtl = pi->dtl;
    l_scan_slope = pi->scan_slope;
    l_v_shift = pi->v_shift;
    l_v_mask = pi->v_mask;
    l_u_mask = pi->u_mask;
    l_u = pi->u;
    l_v = pi->v;
    l_du = pi->du;
    l_dv = pi->dv;

    l_x_fix = l_y * l_scan_slope + fix_make(pi->noname3.xp, 0xffff);

    l_u = pi->u0 + fix_div(pi->unum, pi->denom);
    l_v = pi->v0 + fix_div(pi->vnum, pi->denom);
    l_du = fix_div(pi->dunum, pi->denom);
    l_dv = fix_div(pi->dvnum, pi->denom);
    l_u += l_y * l_du;
    l_v += l_y * l_dv;

    x_cint = fix_int(l_x_fix);
    p = grd_bm.bits + x_cint + l_y * gr_row;
    if (l_y < l_yl) {
        fix test = l_y * l_dxl - x_cint * l_dyl + pi->cl;
        for (; l_y < l_yl; l_y++) {
            if (test <= 0) {
                int k = (l_u >> 16) & l_u_mask;
                k += (l_v >> l_v_shift) & l_v_mask;
                if (bm_bits[k])
                    *p = solid_color; // gr_fill_upixel(bm_bits[k],x_cint,l_y);
            }
            temp_x = x_cint;
            x_cint = fix_int(l_x_fix += l_scan_slope);
            if (temp_x != x_cint) {
                test += l_dtl;
                p -= (temp_x - x_cint);
            } else
                test += l_dxl;

            p += gr_row;
            l_u += l_du;
            l_v += l_dv;
        }
    }

    for (; l_y < l_yr0; l_y++) {
        int k = (l_u >> 16) & l_u_mask;
        k += (l_v >> l_v_shift) & l_v_mask;
        if (bm_bits[k])
            *p = solid_color; // gr_fill_upixel(bm_bits[k],x_cint,l_y);

        temp_x = x_cint;
        x_cint = fix_int(l_x_fix += l_scan_slope);
        if (temp_x != x_cint)
            p -= (temp_x - x_cint);

        p += gr_row;
        l_u += l_du;
        l_v += l_dv;
    }

    if (l_y < l_yr) {
        fix test = l_y * l_dxr - x_cint * l_dyr + pi->cr;
        p = grd_bm.bits + x_cint + l_y * gr_row;
        for (; l_y < l_yr; l_y++) {
            if (test >= 0) {
                int k = (l_u >> 16) & l_u_mask;
                k += (l_v >> l_v_shift) & l_v_mask;
                if (bm_bits[k])
                    *p = solid_color; // gr_fill_upixel(bm_bits[k],x_cint,l_y);
            }

            temp_x = x_cint;
            x_cint = fix_int(l_x_fix += l_scan_slope);
            if (temp_x != x_cint) {
                test += l_dtr;
                p -= (temp_x - x_cint);
            } else
                test += l_dxr;

            p += gr_row;
            l_u += l_du;
            l_v += l_dv;
        }
    }

    pi->noname1.x_fix = l_x_fix;
    pi->noname4.y = l_y;
    pi->u = l_u;
    pi->v = l_v;
    pi->du = l_du;
    pi->dv = l_dv;
}

extern void gri_per_umap_hscan(grs_bitmap *bm, int n, grs_vertex **vpl, grs_per_setup *ps);
extern void gri_per_umap_vscan(grs_bitmap *bm, int n, grs_vertex **vpl, grs_per_setup *ps);

void gri_trans_solid_per_umap_hscan_init(grs_bitmap *bm, grs_per_setup *ps) {
    (void)bm;
    ps->shell_func = (void (*)())gri_per_umap_hscan;
    ps->scanline_func = (void (*)())gri_trans_solid_per_umap_hscan_scanline;
}

void gri_trans_solid_per_umap_vscan_init(grs_bitmap *bm, grs_per_setup *ps) {
    (void)bm;
    ps->shell_func = (void (*)())gri_per_umap_vscan;
    ps->scanline_func = (void (*)())gri_trans_solid_per_umap_vscan_scanline;
}
