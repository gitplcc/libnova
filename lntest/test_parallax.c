/*
 * test_parallax.c
 *
 * Copyright 2021 U-MARTE\lcastedo <lcastedo@marte>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include "config.h"

#include <libnova/libnova.h>

#include <unity.h>

double JD;
struct ln_lnlat_posn observer;

void setUp()
{
  struct ln_dms dms = {
    .neg = 0, .degrees = 33, .minutes = 21, .seconds = 22.0
  };
  observer.lat = ln_dms_to_deg(&dms);

  dms = (struct ln_dms) {
    .neg = 1, .degrees = 116, .minutes = 51, .seconds = 47.0
  };
  observer.lng = ln_dms_to_deg(&dms);

  struct ln_date date = {
    .years = 2003, .months  =  8, .days    = 28,
    .hours =    3, .minutes = 17, .seconds =  0.0
  };
  JD = ln_get_julian_day(&date);
}

void tearDown()
{
}

void test_ln_get_parallax(void)
{
  struct ln_equ_posn mars;
  ln_get_mars_equ_coords(JD, &mars);

  struct ln_equ_posn parallax;
  ln_get_parallax(
    &mars, ln_get_mars_earth_dist(JD), &observer, 1706, JD, &parallax
  );

  /* parallax is hard to calculate, so we allow relatively big error */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 0.003552, parallax.ra,
    "Mars RA parallax for Palomar observatory at 2003/08/28 3:17 UT"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    2.0e-5, -20.01 / 3600.0, parallax.dec,
    "Mars DEC parallax for Palomar observatory at 2003/08/28 3:17 UT"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_parallax);

  return UNITY_END();
}
