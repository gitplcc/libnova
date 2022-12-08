/*
 * test_parabolic_motion.c
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

void setUp()
{
}

void tearDown()
{
}

void test_parabolic_motion(void)
{
  struct ln_date obs_date = {
    .years = 2003, .months  = 1, .days    = 11,
    .hours =    0, .minutes = 0, .seconds =  0
  };
  double o_JD = ln_get_julian_day(&obs_date);

  struct ln_date epoch_date = {
    .years = 2003, .months  = 1, .days    = 29,
    .hours =    0, .minutes = 6, .seconds = 37.44
  };
  double e_JD = ln_get_julian_day(&epoch_date);

  struct ln_par_orbit orbit = {
    .q     =   0.190082,
    .i     =  94.1511,
    .w     = 187.5613,
    .omega = 119.0676,
    .JD    = e_JD
  };

  double v = ln_get_par_true_anomaly(orbit.q, o_JD - e_JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 247.18968605, v, "(True Anomaly) v when e is 0.1 and E is 5.5545"
  );

  double r = ln_get_par_radius_vector(orbit.q, o_JD - e_JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.62085992, r, "(Radius Vector) r when v is , e is 0.1 and E is 5.5545"
  );

  struct ln_rect_posn posn;
  ln_get_par_geo_rect_posn(&orbit, o_JD, &posn);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.29972461, posn.X,
    "(Geocentric Rect Coords X) for comet C/2002 X5 (Kudo-Fujikawa)"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, -0.93359772, posn.Y,
    "(Geocentric Rect Coords Y) for comet C/2002 X5 (Kudo-Fujikawa)"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.24639194, posn.Z,
    "(Geocentric Rect Coords Z) for comet C/2002 X5 (Kudo-Fujikawa)"
  );

  ln_get_par_helio_rect_posn(&orbit, o_JD, &posn);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, -0.04143700, posn.X,
    "(Heliocentric Rect Coords X) for comet C/2002 X5 (Kudo-Fujikawa)"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, -0.08736588, posn.Y,
    "(Heliocentric Rect Coords Y) for comet C/2002 X5 (Kudo-Fujikawa)"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.61328397, posn.Z,
    "(Heliocentric Rect Coords Z) for comet C/2002 X5 (Kudo-Fujikawa)"
  );

  struct ln_equ_posn equ_posn;
  ln_get_par_body_equ_coords (o_JD, &orbit, &equ_posn);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 287.79617309, equ_posn.ra, "(RA) for comet C/2002 X5 (Kudo-Fujikawa)"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 14.11800859, equ_posn.dec, "(Dec) for comet C/2002 X5 (Kudo-Fujikawa)"
  );

  double dist = ln_get_par_body_solar_dist(o_JD, &orbit);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 0.62085992, dist,
    "(Body Solar Dist) for comet C/2002 X5 (Kudo-Fujikawa) in AU"
  );

  dist = ln_get_par_body_earth_dist(o_JD, &orbit);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 1.01101362, dist,
    "(Body Earth Dist) for comet C/2002 X5 (Kudo-Fujikawa) in AU"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_parabolic_motion);

  return UNITY_END();
}
