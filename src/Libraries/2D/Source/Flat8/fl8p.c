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
 * $Source: r:/prj/lib/src/2d/RCS/fl8p.c $
 * $Revision: 1.15 $
 * $Author: kevin $
 * $Date: 1994/11/02 19:38:32 $
 *
 * full perspective texture mapper.
 *
 */

// ************************************************************************************
// ************************************************************************************
//
//  MLA - don't think we need to optimize this, its in C on the PC too
//
// ************************************************************************************
// ************************************************************************************

#include "cnvdat.h"
#include "pertyp.h"
#include "plytyp.h"
#define safe_fix_cint(x) ((fix_frac(x) == 0) ? (fix_int(x)) : (fix_int(x) + 1))
#define fix_16_20(a) ((a) >> 4)

// prototypes
void gri_per_umap_hscan(grs_bitmap *bm, int n, grs_vertex **vpl, grs_per_setup *ps);
void gri_per_umap_vscan(grs_bitmap *bm, int n, grs_vertex **vpl, grs_per_setup *ps);

/**************************************************************
Routines to scan polygon.  hscan=standard horizontal scanlines.
vscan=vertical scanlines.
**************************************************************/

void gri_per_umap_hscan(grs_bitmap *bm, int n, grs_vertex **vpl, grs_per_setup *ps) {
    grs_per_info pi;
    fix y_prime[10];
    fix yp_left = 0, yp_right = 0;
    fix x_left = 0, x_right = 0;
    fix y_left = 0, y_right = 0;
    fix dx_left = 0, dx_right = 0;
    int yp_min = 0, yp_max = 0, yp_next = 0;
    int x_min = 0, x_max = 0, xr_min = 0, xr_max = 0, xl_min = 0, xl_max = 0;
    int n_min = 0, n_left = 0, n_right = 0;
    int j = 0;

    pi.scale = grd_bm.w;
    pi.scan_slope = ps->scan_slope;
    pi.dp = ps->dp;
    pi.noname2.clut = ps->noname1.clut;
    if (fix_abs(pi.scan_slope) > FIX_UNIT)
        return;

    if (bm->row != 1 << (bm->wlog))
        return;
    if (bm->h != 1 << (bm->hlog))
        return;
    pi.u_mask = bm->row - 1;
    pi.v_mask = (bm->h - 1) << bm->wlog;
    pi.v_shift = 16 - bm->wlog;

    n_min = 0;
    y_prime[n_min] = vpl[n_min]->y - fix_mul(vpl[n_min]->x, ps->scan_slope);
    yp_min = yp_max = fix_cint(y_prime[n_min]);

    for (j = 1; j < n; j++) {
        y_prime[j] = vpl[j]->y - fix_mul(vpl[j]->x, ps->scan_slope);
        pi.noname3.yp = fix_cint(y_prime[j]);
        if (pi.noname3.yp < yp_min) {
            yp_min = pi.noname3.yp;
            n_min = j;
        }
        if (pi.noname3.yp > yp_max)
            yp_max = pi.noname3.yp;
    }

    if (yp_max == yp_min)
        return;

    pi.denom = fix_16_20(ps->c + fix_mul(ps->b, y_prime[0]));
    pi.u0 =
        vpl[0]->u - fix_div(fix_mul(vpl[0]->x, ps->alpha_u) + fix_mul(vpl[0]->y, ps->beta_u) + ps->gamma_u, pi.denom);
    pi.v0 =
        vpl[0]->v - fix_div(fix_mul(vpl[0]->x, ps->alpha_v) + fix_mul(vpl[0]->y, ps->beta_v) + ps->gamma_v, pi.denom);

    n_left = n_right = n_min;
    pi.noname3.yp = yp_min;
    while (fix_cint(y_prime[(n_left + n - 1) % n]) == pi.noname3.yp)
        n_left = (n_left + n - 1) % n;
    while (fix_cint(y_prime[(n_right + 1) % n]) == pi.noname3.yp)
        n_right = (n_right + 1) % n;

    pi.noname3.yp--;
    pi.denom = fix_16_20(ps->c + pi.noname3.yp * ps->b);
    pi.unum = ps->gamma_u + ps->beta_u * pi.noname3.yp;
    pi.dunum = ps->alpha_u + fix_mul(ps->scan_slope, ps->beta_u);
    pi.vnum = ps->gamma_v + ps->beta_v * pi.noname3.yp;
    pi.dvnum = ps->alpha_v + fix_mul(ps->scan_slope, ps->beta_v);

    if (n_right != n_left) {

        pi.dxl = (vpl[n_right]->x - vpl[n_left]->x) / pi.scale;
        pi.dyl = (vpl[n_right]->y - vpl[n_left]->y) / pi.scale;
        pi.noname4.x = fix_cint(vpl[n_left]->x);
        pi.noname5.xl = fix_cint(vpl[n_right]->x);
        if (pi.scan_slope > 0) {
            x_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.bot - 1) - pi.noname3.yp, 1), pi.scan_slope));
            x_min = fix_int(fix_div(fix_make((grd_int_clip.top - 1) - pi.noname3.yp, 1), pi.scan_slope)) + 1;
        } else {
            x_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.top - 1) - pi.noname3.yp, 1), pi.scan_slope));
            x_min = fix_int(fix_div(fix_make((grd_int_clip.bot - 1) - pi.noname3.yp, 1), pi.scan_slope)) + 1;
        }
        if (pi.noname4.x < x_min)
            pi.noname4.x = x_min;
        if (pi.noname5.xl > x_max)
            pi.noname5.xl = x_max;
        pi.noname7.xr0 = pi.noname6.xr = pi.noname5.xl;
        pi.cl = fix_mul(pi.dxl, vpl[n_left]->y) - fix_mul(pi.dyl, vpl[n_left]->x);
        if (pi.noname4.x * pi.dyl - fix_mul(fix_make(pi.noname3.yp - 1, 0) + pi.noname4.x * pi.scan_slope, pi.dxl) + pi.cl < 0) {
            pi.dyl = -pi.dyl;
            pi.dxl = -pi.dxl;
            pi.cl = -pi.cl;
        }
        if (pi.scan_slope > 0)
            pi.dtl = pi.dyl - pi.dxl;
        else
            pi.dtl = pi.dyl + pi.dxl;

        ((void (*)(grs_per_info *, grs_bitmap *))(ps->scanline_func))(&pi, bm);

        pi.denom += fix_16_20(ps->b), pi.unum += ps->beta_u, pi.vnum += ps->beta_v;
        yp_min--; /* first line already done */
    }
    pi.noname3.yp++;
    while (pi.noname3.yp < yp_max) {
        /* check left edge */
        if (fix_cint(y_prime[n_left]) <= pi.noname3.yp) {
            int n_prev = 0;
            fix d;
            do {
                if (fix_cint(y_prime[n_left]) == pi.noname3.yp)
                    n_prev = n_left;
                if (--n_left < 0)
                    n_left = n - 1;
            } while (fix_cint(y_prime[n_left]) <= pi.noname3.yp);
            yp_left = y_prime[n_prev];
            x_left = vpl[n_prev]->x;
            y_left = vpl[n_prev]->y;
            xl_min = fix_cint(x_left);
            xl_max = fix_cint(vpl[n_left]->x);
            pi.dxl = (vpl[n_left]->x - x_left) / pi.scale;
            pi.dyl = (vpl[n_left]->y - y_left) / pi.scale;
            pi.cl = fix_mul(pi.dxl, y_left) - fix_mul(pi.dyl, x_left);
            d = pi.dyl - fix_mul(pi.dxl, pi.scan_slope);
            dx_left = fix_div(pi.dxl, d);
            x_left = fix_div(fix_mul(pi.dyl, x_left) + fix_mul(pi.dxl, fix_ceil(yp_left) - y_left), d);
            if (xl_max < xl_min) {
                fix foo = xl_min;
                xl_min = xl_max;
                xl_max = foo;
            }
            if (fix_mul(vpl[n_left]->x - FIX_UNIT, pi.dyl) - fix_mul(vpl[n_left]->y - pi.scan_slope, pi.dxl) + pi.cl <
                0) {
                pi.dyl = -pi.dyl;
                pi.dxl = -pi.dxl;
                pi.cl = -pi.cl;
            }
            if (pi.scan_slope > 0)
                pi.dtl = pi.dyl - pi.dxl;
            else
                pi.dtl = pi.dyl + pi.dxl;
            yp_left = y_prime[n_left];
        }

        /* check right edge */
        if (fix_cint(y_prime[n_right]) <= pi.noname3.yp) {
            int n_prev = 0;
            fix d;
            do {
                if (fix_cint(y_prime[n_right]) == pi.noname3.yp)
                    n_prev = n_right;
                if (++n_right == n)
                    n_right = 0;
            } while (fix_cint(y_prime[n_right]) <= pi.noname3.yp);
            yp_right = y_prime[n_prev];
            x_right = vpl[n_prev]->x;
            y_right = vpl[n_prev]->y;
            xr_min = fix_cint(x_right);
            xr_max = fix_cint(vpl[n_right]->x);
            pi.dxr = (vpl[n_right]->x - x_right) / pi.scale;
            pi.dyr = (vpl[n_right]->y - y_right) / pi.scale;
            pi.cr = fix_mul(pi.dxr, y_right) - fix_mul(pi.dyr, x_right);
            d = pi.dyr - fix_mul(pi.dxr, pi.scan_slope);
            dx_right = fix_div(pi.dxr, d);
            x_right = fix_div(fix_mul(pi.dyr, x_right) + fix_mul(pi.dxr, fix_ceil(yp_right) - y_right), d);
            if (xr_max < xr_min) {
                fix foo = xr_min;
                xr_min = xr_max;
                xr_max = foo;
            }
            if (fix_mul(vpl[n_right]->x - FIX_UNIT, pi.dyr) - fix_mul(vpl[n_right]->y - pi.scan_slope, pi.dxr) + pi.cr <
                0) {
                pi.dyr = -pi.dyr;
                pi.dxr = -pi.dxr;
                pi.cr = -pi.cr;
            }
            if (pi.scan_slope > 0)
                pi.dtr = pi.dyr - pi.dxr;
            else
                pi.dtr = pi.dyr + pi.dxr;
            yp_right = y_prime[n_right];
        }
        yp_next = (yp_right < yp_left) ? fix_cint(yp_right) : fix_cint(yp_left);

        /* do 0th scanline if at yp_min */
        if (pi.noname3.yp == yp_min) {
            x_left -= dx_left;
            x_right -= dx_right;
            pi.noname3.yp--;
        }
        for (; pi.noname3.yp < yp_next; pi.noname3.yp++) {
            if ((pi.noname3.yp + 1 == yp_max) && (n_left != n_right)) {
                pi.dxl = (vpl[n_right]->x - vpl[n_left]->x) / pi.scale;
                pi.dyl = (vpl[n_right]->y - vpl[n_left]->y) / pi.scale;
                pi.noname4.x = fix_cint(vpl[n_left]->x);
                pi.noname5.xl = fix_cint(vpl[n_right]->x);
                if (pi.scan_slope > 0) {
                    x_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.bot - 1) - pi.noname3.yp, 1), pi.scan_slope));
                    x_min = fix_int(fix_div(fix_make((grd_int_clip.top - 1) - pi.noname3.yp, 1), pi.scan_slope)) + 1;
                } else {
                    x_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.top - 1) - pi.noname3.yp, 1), pi.scan_slope));
                    x_min = fix_int(fix_div(fix_make((grd_int_clip.bot - 1) - pi.noname3.yp, 1), pi.scan_slope)) + 1;
                }
                if (pi.noname4.x < x_min)
                    pi.noname4.x = x_min;
                if (pi.noname5.xl > x_max)
                    pi.noname5.xl = x_max;
                pi.noname7.xr0 = pi.noname6.xr = pi.noname5.xl;
                pi.cl = fix_mul(pi.dxl, vpl[n_left]->y) - fix_mul(pi.dyl, vpl[n_left]->x);
                if (pi.noname4.x * pi.dyl - fix_mul(fix_make(pi.noname3.yp + 1, 0) + pi.noname4.x * pi.scan_slope, pi.dxl) + pi.cl < 0) {
                    pi.dyl = -pi.dyl;
                    pi.dxl = -pi.dxl;
                    pi.cl = -pi.cl;
                }
                if (pi.scan_slope > 0)
                    pi.dtl = pi.dyl - pi.dxl;
                else
                    pi.dtl = pi.dyl + pi.dxl;

                ((void (*)(grs_per_info *, grs_bitmap *))(ps->scanline_func))(&pi, bm);

                pi.noname3.yp = yp_max;
                break;
            }

            if (dx_left > 0) {
                pi.noname4.x = fix_fint(x_left);
                pi.noname5.xl = fix_cint(x_left + dx_left);
            } else {
                pi.noname4.x = fix_fint(x_left + dx_left);
                pi.noname5.xl = fix_cint(x_left);
            }
            if (dx_right > 0) {
                pi.noname7.xr0 = fix_fint(x_right);
                pi.noname6.xr = fix_cint(x_right + dx_right);
            } else {
                pi.noname7.xr0 = fix_fint(x_right + dx_right);
                pi.noname6.xr = fix_cint(x_right);
            }
            if (pi.scan_slope > 0) {
                x_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.bot - 1) - pi.noname3.yp, 1), pi.scan_slope));
                x_min = fix_int(fix_div(fix_make((grd_int_clip.top - 1) - pi.noname3.yp, 1), pi.scan_slope)) + 1;
            } else {
                x_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.top - 1) - pi.noname3.yp, 1), pi.scan_slope));
                x_min = fix_int(fix_div(fix_make((grd_int_clip.bot - 1) - pi.noname3.yp, 1), pi.scan_slope)) + 1;
            }
            if (xl_min > x_min)
                x_min = xl_min;
            if (xr_max < x_max)
                x_max = xr_max;
            if (pi.noname6.xr > x_max)
                pi.noname6.xr = x_max;
            if (pi.noname7.xr0 > x_max)
                pi.noname7.xr0 = x_max;
            if (pi.noname5.xl > pi.noname7.xr0)
                pi.noname5.xl = pi.noname7.xr0;
            if (pi.noname4.x < x_min)
                pi.noname4.x = x_min;

            ((void (*)(grs_per_info *, grs_bitmap *))(ps->scanline_func))(&pi, bm);

            x_left += dx_left, x_right += dx_right;
            pi.denom += fix_16_20(ps->b), pi.unum += ps->beta_u, pi.vnum += ps->beta_v;
        }
    }
}

void gri_per_umap_vscan(grs_bitmap *bm, int n, grs_vertex **vpl, grs_per_setup *ps) {
    grs_per_info pi;
    fix x_prime[10];
    fix xp_top = 0, xp_bot = 0;
    fix x_top = 0, x_bot = 0;
    fix y_top = 0, y_bot = 0;
    fix dy_top = 0, dy_bot = 0;
    int xp_min = 0, xp_max = 0, xp_next = 0;
    int y_min = 0, y_max = 0, yr_min = 0, yr_max = 0, yl_min = 0, yl_max = 0;
    int n_min = 0, n_top = 0, n_bot = 0;
    int j = 0;

    pi.scale = grd_bm.w;
    pi.scan_slope = ps->scan_slope;
    pi.dp = ps->dp;
    pi.noname2.clut = ps->noname1.clut;
    if (fix_abs(pi.scan_slope) >= FIX_UNIT)
        return;

    if (bm->row != 1 << (bm->wlog))
        return;
    if (bm->h != 1 << (bm->hlog))
        return;
    pi.u_mask = bm->row - 1;
    pi.v_mask = (bm->h - 1) << bm->wlog;
    pi.v_shift = 16 - bm->wlog;

    xp_min = xp_max = fix_cint(x_prime[n_min = 0] = vpl[0]->x - fix_mul(vpl[0]->y, ps->scan_slope));
    for (j = 1; j < n; j++) {
        pi.noname3.xp = fix_cint(x_prime[j] = vpl[j]->x - fix_mul(vpl[j]->y, ps->scan_slope));
        if (pi.noname3.xp < xp_min) {
            xp_min = pi.noname3.xp;
            n_min = j;
        }
        if (pi.noname3.xp > xp_max)
            xp_max = pi.noname3.xp;
    }
    if (xp_max == xp_min)
        return;
    pi.denom = fix_16_20(ps->c + fix_mul(ps->a, x_prime[0]));
    pi.u0 =
        vpl[0]->u - fix_div(fix_mul(vpl[0]->x, ps->alpha_u) + fix_mul(vpl[0]->y, ps->beta_u) + ps->gamma_u, pi.denom);
    pi.v0 =
        vpl[0]->v - fix_div(fix_mul(vpl[0]->x, ps->alpha_v) + fix_mul(vpl[0]->y, ps->beta_v) + ps->gamma_v, pi.denom);

    n_top = n_bot = n_min;
    pi.noname3.xp = xp_min;
    while (fix_cint(x_prime[(n_top + 1) % n]) == pi.noname3.xp)
    {
        n_top = (n_top + 1) % n;
    }
    while (fix_cint(x_prime[(n_bot + n - 1) % n]) == pi.noname3.xp)
    {
        n_bot = (n_bot + n - 1) % n;
    }

    pi.noname3.xp--;
    pi.denom = fix_16_20(ps->c + pi.noname3.xp * ps->a);
    pi.unum = ps->gamma_u + ps->alpha_u * pi.noname3.xp;
    pi.dunum = ps->beta_u + fix_mul(ps->scan_slope, ps->alpha_u);
    pi.vnum = ps->gamma_v + ps->alpha_v * pi.noname3.xp;
    pi.dvnum = ps->beta_v + fix_mul(ps->scan_slope, ps->alpha_v);

    if (n_bot != n_top) {

        pi.dxl = (vpl[n_bot]->x - vpl[n_top]->x) / pi.scale;
        pi.dyl = (vpl[n_bot]->y - vpl[n_top]->y) / pi.scale;
        pi.noname4.y = fix_cint(vpl[n_top]->y);
        pi.noname5.yl = fix_cint(vpl[n_bot]->y);
        if (pi.scan_slope > 0) {
            y_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.right - 1) - pi.noname3.xp, 1), pi.scan_slope));
            y_min = fix_int(fix_div(fix_make((grd_int_clip.left - 1) - pi.noname3.xp, 1), pi.scan_slope)) + 1;
        } else {
            y_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.left - 1) - pi.noname3.xp, 1), pi.scan_slope));
            y_min = fix_int(fix_div(fix_make((grd_int_clip.right - 1) - pi.noname3.xp, 1), pi.scan_slope)) + 1;
        }
        if (pi.noname4.y < y_min)
            pi.noname4.y = y_min;
        if (pi.noname5.yl > y_max)
            pi.noname5.yl = y_max;
        pi.noname7.yr0 = pi.noname6.yr = pi.noname5.yl;

        pi.cl = -fix_mul(pi.dxl, vpl[n_top]->y) + fix_mul(pi.dyl, vpl[n_top]->x);
        if (pi.noname4.y * pi.dxl - fix_mul(fix_make(pi.noname3.xp - 1, 0) + pi.noname4.y * pi.scan_slope, pi.dyl) + pi.cl < 0) {
            pi.dyl = -pi.dyl;
            pi.dxl = -pi.dxl;
            pi.cl = -pi.cl;
        }
        if (pi.scan_slope > 0)
            pi.dtl = pi.dxl - pi.dyl;
        else
            pi.dtl = pi.dyl + pi.dxl;

        ((void (*)(grs_per_info *, grs_bitmap *))(ps->scanline_func))(&pi, bm);

        pi.denom += fix_16_20(ps->a), pi.unum += ps->alpha_u, pi.vnum += ps->alpha_v;
        xp_min--; /* first line already done */
    }
    pi.noname3.xp++;
    while (pi.noname3.xp < xp_max) {
        /* check top edge */
        if (fix_cint(x_prime[n_top]) <= pi.noname3.xp) {
            int n_prev = 0;
            fix d;
            do {
                if (fix_cint(x_prime[n_top]) == pi.noname3.xp)
                    n_prev = n_top;
                if (++n_top == n)
                    n_top = 0;
            } while (fix_cint(x_prime[n_top]) <= pi.noname3.xp);
            xp_top = x_prime[n_prev];
            x_top = vpl[n_prev]->x;
            y_top = vpl[n_prev]->y;
            yl_min = fix_cint(y_top);
            yl_max = fix_cint(vpl[n_top]->y);
            pi.dxl = (vpl[n_top]->x - x_top) / pi.scale;
            pi.dyl = (vpl[n_top]->y - y_top) / pi.scale;
            pi.cl = -fix_mul(pi.dxl, y_top) + fix_mul(pi.dyl, x_top);
            d = pi.dxl - fix_mul(pi.dyl, pi.scan_slope);
            dy_top = fix_div(pi.dyl, d);
            y_top = fix_div(fix_mul(pi.dxl, y_top) + fix_mul(pi.dyl, fix_ceil(xp_top) - x_top), d);
            if (yl_max < yl_min) {
                fix foo = yl_min;
                yl_min = yl_max;
                yl_max = foo;
            }
            if (fix_mul(vpl[n_top]->y - FIX_UNIT, pi.dxl) - fix_mul(vpl[n_top]->x - pi.scan_slope, pi.dyl) + pi.cl <
                0) {
                pi.dyl = -pi.dyl;
                pi.dxl = -pi.dxl;
                pi.cl = -pi.cl;
            }
            if (pi.scan_slope > 0)
                pi.dtl = pi.dxl - pi.dyl;
            else
                pi.dtl = pi.dyl + pi.dxl;
            xp_top = x_prime[n_top];
        }

        /* check bot edge */
        if (fix_cint(x_prime[n_bot]) <= pi.noname3.xp) {
            int n_prev = 0;
            fix d;
            do {
                if (fix_cint(x_prime[n_bot]) == pi.noname3.xp)
                    n_prev = n_bot;
                if (--n_bot < 0)
                    n_bot = n - 1;
            } while (fix_cint(x_prime[n_bot]) <= pi.noname3.xp);
            xp_bot = x_prime[n_prev];
            x_bot = vpl[n_prev]->x;
            y_bot = vpl[n_prev]->y;
            yr_min = fix_cint(y_bot);
            yr_max = fix_cint(vpl[n_bot]->y);
            pi.dxr = (vpl[n_bot]->x - x_bot) / pi.scale;
            pi.dyr = (vpl[n_bot]->y - y_bot) / pi.scale;
            pi.cr = -fix_mul(pi.dxr, y_bot) + fix_mul(pi.dyr, x_bot);
            d = pi.dxr - fix_mul(pi.dyr, pi.scan_slope);
            dy_bot = fix_div(pi.dyr, d);
            y_bot = fix_div(fix_mul(pi.dxr, y_bot) + fix_mul(pi.dyr, fix_ceil(xp_bot) - x_bot), d);
            if (yr_max < yr_min) {
                fix foo = yr_min;
                yr_min = yr_max;
                yr_max = foo;
            }
            if (fix_mul(vpl[n_bot]->y - FIX_UNIT, pi.dxr) - fix_mul(vpl[n_bot]->x - pi.scan_slope, pi.dyr) + pi.cr <
                0) {
                pi.dyr = -pi.dyr;
                pi.dxr = -pi.dxr;
                pi.cr = -pi.cr;
            }
            if (pi.scan_slope > 0)
                pi.dtr = pi.dxr - pi.dyr;
            else
                pi.dtr = pi.dyr + pi.dxr;
            xp_bot = x_prime[n_bot];
        }
        xp_next = (xp_bot < xp_top) ? fix_cint(xp_bot) : fix_cint(xp_top);

        /* do 0th scanline if at xp_min */
        if (pi.noname3.xp == xp_min) {
            y_top -= dy_top;
            y_bot -= dy_bot;
            pi.noname3.xp--;
        }
        for (; pi.noname3.xp < xp_next; pi.noname3.xp++) {
            if ((pi.noname3.xp + 1 == xp_max) && (n_top != n_bot)) {
                pi.dxl = (vpl[n_bot]->x - vpl[n_top]->x) / pi.scale;
                pi.dyl = (vpl[n_bot]->y - vpl[n_top]->y) / pi.scale;
                pi.noname4.y = fix_cint(vpl[n_top]->y);
                pi.noname5.yl = fix_cint(vpl[n_bot]->y);
                if (pi.scan_slope > 0) {
                    y_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.right - 1) - pi.noname3.xp, 1), pi.scan_slope));
                    y_min = fix_int(fix_div(fix_make((grd_int_clip.left - 1) - pi.noname3.xp, 1), pi.scan_slope)) + 1;
                } else {
                    y_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.left - 1) - pi.noname3.xp, 1), pi.scan_slope));
                    y_min = fix_int(fix_div(fix_make((grd_int_clip.right - 1) - pi.noname3.xp, 1), pi.scan_slope)) + 1;
                }
                if (pi.noname4.y < y_min)
                    pi.noname4.y = y_min;
                if (pi.noname5.yl > y_max)
                    pi.noname5.yl = y_max;
                pi.noname7.yr0 = pi.noname6.yr = pi.noname5.yl;
                pi.cl = -fix_mul(pi.dxl, vpl[n_top]->y) + fix_mul(pi.dyl, vpl[n_top]->x);
                if (pi.noname4.y * pi.dxl - fix_mul(fix_make(pi.noname3.xp + 1, 0) + pi.noname4.y * pi.scan_slope, pi.dyl) + pi.cl < 0) {
                    pi.dyl = -pi.dyl;
                    pi.dxl = -pi.dxl;
                    pi.cl = -pi.cl;
                }
                if (pi.scan_slope > 0)
                    pi.dtl = pi.dxl - pi.dyl;
                else
                    pi.dtl = pi.dyl + pi.dxl;

                ((void (*)(grs_per_info *, grs_bitmap *))(ps->scanline_func))(&pi, bm);

                pi.noname3.xp = xp_max;
                break;
            }

            if (dy_top > 0) {
                pi.noname4.y = fix_fint(y_top);
                pi.noname5.yl = fix_cint(y_top + dy_top);
            } else {
                pi.noname4.y = fix_fint(y_top + dy_top);
                pi.noname5.yl = fix_cint(y_top);
            }
            if (dy_bot > 0) {
                pi.noname7.yr0 = fix_fint(y_bot);
                pi.noname6.yr = fix_cint(y_bot + dy_bot);
            } else {
                pi.noname7.yr0 = fix_fint(y_bot + dy_bot);
                pi.noname6.yr = fix_cint(y_bot);
            }
            if (pi.scan_slope > 0) {
                y_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.right - 1) - pi.noname3.xp, 1), pi.scan_slope));
                y_min = fix_int(fix_div(fix_make((grd_int_clip.left - 1) - pi.noname3.xp, 1), pi.scan_slope)) + 1;
            } else {
                y_max = safe_fix_cint(fix_div(fix_make((grd_int_clip.left - 1) - pi.noname3.xp, 1), pi.scan_slope));
                y_min = fix_int(fix_div(fix_make((grd_int_clip.right - 1) - pi.noname3.xp, 1), pi.scan_slope)) + 1;
            }
            if (yl_min > y_min)
                y_min = yl_min;
            if (yr_max < y_max)
                y_max = yr_max;
            if (pi.noname6.yr > y_max)
                pi.noname6.yr = y_max;
            if (pi.noname7.yr0 > y_max)
                pi.noname7.yr0 = y_max;
            if (pi.noname5.yl > pi.noname7.yr0)
                pi.noname5.yl = pi.noname7.yr0;
            if (pi.noname4.y < y_min)
                pi.noname4.y = y_min;

            ((void (*)(grs_per_info *, grs_bitmap *))(ps->scanline_func))(&pi, bm);

            y_top += dy_top, y_bot += dy_bot;
            pi.denom += fix_16_20(ps->a), pi.unum += ps->alpha_u, pi.vnum += ps->alpha_v;
        }
    }
}
