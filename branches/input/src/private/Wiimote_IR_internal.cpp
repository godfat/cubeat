
/* 
   Wiimote IR data calculation supplemental functions,
   internal use only. Adapted from Wiiuse lib 0.9 (wiiuse.net)
*/

#include "../../include/private/Wiimote_IR_internal.hpp"

/*
 *	wiiuse
 *
 *	Written By:
 *		Michael Laforest	< para >
 *		Email: < thepara (--AT--) g m a i l [--DOT--] com >
 *
 *	Copyright 2006-2007
 *
 *	This file is part of wiiuse.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 *
 */

/**
 *	@file
 *	@brief Handles IR data.
 */

#include <cstdio>
#define _USE_MATH_DEFINES 1
#include <cmath>

namespace {
void fix_rotated_ir_dots  (wiimote::ir::dot dots[4], float ang);
void get_ir_dot_avg       (wiimote::ir::dot dots[4], float& x, float& y);
void reorder_ir_dots      (wiimote::ir::dot dots[4]);
float cal_ir_distance     (wiimote::ir::dot dots[4]);

int ir_num_dots = 0;
int ir_state = 0;
int dots_order[4] = {0};
float ir_distance = 0.0f;
} //anonymous-namespace

/**
 *	@brief Interpret IR data into more user friendly variables.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 */
bool interpret_ir_data(wiimote& wm, float& out_x, float& out_y, float& out_z) 
{
    wiimote::ir::dot* dot = wm.IR.Dot;
	int i;
	float roll = 0.0f;
    int last_num_dots = ir_num_dots;

    roll = wm.Acceleration.Orientation.Roll;

	/* count visible dots */
	ir_num_dots = 0;
	for (i = 0; i < 4; ++i) {
        if (dot[i].bFound)
			ir_num_dots++;
	}

	switch (ir_num_dots) {        
		case 0:
		{
			ir_state = 0;

			/* reset the dot ordering */
			for (i = 0; i < 4; ++i)
				dots_order[i] = 0;

			out_x = 0.0f;
			out_y = 0.0f;
			out_z = 0.0f;

			return 0;
		}
		case 1:
		{
            fix_rotated_ir_dots(wm.IR.Dot, roll);

			if (ir_state < 2) {
				/*
				 *	Only 1 known dot, so use just that.
				 */
				for (i = 0; i < 4; ++i) {
                    if (dot[i].bFound) {
                        out_x = dot[i].X;
						out_y = dot[i].Y;
						break;
					}
				}
			} else {
				/*
				 *	Only see 1 dot but know theres 2.
				 *	Try to estimate where the other one
				 *	should be and use that.
				 */
				for (i = 0; i < 4; ++i) {
                    if (dot[i].bFound) {
						float ox, x, y;

						if (dots_order[i] == 1)
							/* visible is the left dot - estimate where the right is */
                            ox = dot[i].X + ir_distance;
						else if (dots_order[i] == 2)
							/* visible is the right dot - estimate where the left is */
							ox = dot[i].X - ir_distance;

						x = (dot[i].X + ox) / 2;
						y = dot[i].Y;

						out_x = x;
						out_y = y;
						break;
					}
				}
			}
			return 1;
		}
		case 2:
		case 3:
		case 4:
		{
			/*
			 *	Two (or more) dots known and seen.
			 *	Average them together to estimate the true location.
			 */
			float x, y;
			ir_state = 2;

            fix_rotated_ir_dots(wm.IR.Dot, roll);

			/* if there is at least 1 new dot, reorder them all */
            if (ir_num_dots > last_num_dots) {
				reorder_ir_dots(dot);
				out_x = 0.0f;
				out_y = 0.0f;
			}

			ir_distance = cal_ir_distance(dot);
			out_z = 1.0f - ir_distance;

            get_ir_dot_avg(wm.IR.Dot, x, y);

			out_x = x;
			out_y = y;
			return 1;
		}
		default:
		{
			return 0;
		}
	}
    return 1;
}

namespace {
/**
 *	@brief Fix the rotation of the IR dots.
 *
 *	@param dot		An array of 4 ir_dot_t objects.
 *	@param ang		The roll angle to correct by (-180, 180)
 *
 *	If there is roll then the dots are rotated
 *	around the origin and give a false cursor
 *	position. Correct for the roll.
 *
 *	If the accelerometer is off then obviously
 *	this will not do anything and the cursor
 *	position may be inaccurate.
 */
void fix_rotated_ir_dots(wiimote::ir::dot dots[4], float ang) 
{
	float s, c;
	float x, y;
	int i;

	if (!ang) {
		return;
	}

    s = (float)sin(ang * M_PI / 180.0f);
	c = (float)cos(ang * M_PI / 180.0f);

	/*
	 *	[ cos(theta)  -sin(theta) ][ ir->rx ]
	 *	[ sin(theta)  cos(theta)  ][ ir->ry ]
	 */

	for (i = 0; i < 4; ++i) {
        if (!dots[i].bFound)
			continue;

		x = dots[i].X - 0.5f;
		y = dots[i].Y - 0.5f;

		dots[i].X = (c * x) + (-s * y);
		dots[i].Y = (s * x) + (c * y);

		dots[i].X += 0.5f;
		dots[i].Y += 0.5f;
	}
}


/**
 *	@brief Average IR dots.
 *
 *	@param dot		An array of 4 ir_dot_t objects.
 *	@param x		[out] Average X
 *	@param y		[out] Average Y
 */
void get_ir_dot_avg(wiimote::ir::dot dots[4], float& x, float& y) 
{
	int vis = 0, i = 0;

	x = 0.0f;
	y = 0.0f;

	for (; i < 4; ++i) {
        if (dots[i].bFound) {
			x += dots[i].X;
			y += dots[i].Y;
			++vis;
		}
	}

	x /= vis;
	y /= vis;
}


/**
 *	@brief Reorder the IR dots.
 *
 *	@param dot		An array of 4 ir_dot_t objects.
 */
void reorder_ir_dots(wiimote::ir::dot dots[4]) 
{
	int i, j, order;

	/* reset the dot ordering */
	for (i = 0; i < 4; ++i)
		dots_order[i] = 0;

	for (order = 1; order < 5; ++order) {
		i = 0;

        for (; !dots[i].bFound || dots_order[i]; ++i)
		if (i > 4)
			return;

		for (j = 0; j < 4; ++j) {
            if (dots[j].bFound && !dots_order[j] && (dots[j].X < dots[i].X))
				i = j;
		}

		dots_order[i] = order;
	}
}


/**
 *	@brief Calculate the distance between the first 2 visible IR dots.
 *
 *	@param dot		An array of 4 ir_dot_t objects.
 */
float cal_ir_distance(wiimote::ir::dot dots[4]) 
{
	int i1, i2;
	float xd, yd;

	for (i1 = 0; i1 < 4; ++i1)
        if (dots[i1].bFound)
			break;
	if (i1 == 4)
		return 0.0f;

	for (i2 = i1+1; i2 < 4; ++i2)
        if (dots[i2].bFound)
			break;
	if (i2 == 4)
		return 0.0f;

	xd = dots[i2].X - dots[i1].Y;
	yd = dots[i2].X - dots[i1].Y;

	return sqrt(xd*xd + yd*yd);
}

} //anonymous-namespace