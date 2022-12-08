/*
 * test_elliptic_motion.c
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

void test_elliptic_motion_enckle(void)
{
  struct ln_date obs_date = {
    .years = 1990, .months  = 10, .days    = 6,
    .hours =    0, .minutes =  0, .seconds = 0
  };
  double o_JD = ln_get_julian_day(&obs_date);

  struct ln_date epoch_date = {
    .years = 1990, .months  = 10, .days    = 28,
    .hours =   12, .minutes = 30, .seconds =  0
  };
  double e_JD = ln_get_julian_day(&epoch_date);

  struct ln_ell_orbit orbit = {
    .JD    = e_JD,
    .a     =   2.2091404,
    .e     =   0.8502196,
    .i     =  11.94525,
    .omega = 334.75006,
    .w     = 186.23352,
    .n     =   0
  };

  double E = ln_solve_kepler(0.1, 5.0);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-12, 5.554589253872320, E,
    "(Equation of kepler) E when e is 0.1 and M is 5.0"
  );

  double v = ln_get_ell_true_anomaly(0.1, E);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 6.13976152, v, "(True Anomaly) v when e is 0.1 and E is 5.5545"
  );

  double r = ln_get_ell_radius_vector (0.5, 0.1, E);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.45023478, r,"(Radius Vector) r when v is , e is 0.1 and E is 5.5545"
  );

  struct ln_rect_posn posn;
  ln_get_ell_geo_rect_posn(&orbit, o_JD, &posn);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.72549850, posn.X, "(Geocentric Rect Coords X) for comet Enckle"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, -0.28443537, posn.Y, "(Geocentric Rect Coords Y) for comet Enckle"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, -0.27031656, posn.Z, "(Geocentric Rect Coords Z) for comet Enckle"
  );

  ln_get_ell_helio_rect_posn(&orbit, o_JD, &posn);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.25017473, posn.X, "(Heliocentric Rect Coords X) for comet Enckle"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.48476422, posn.Y, "(Heliocentric Rect Coords Y) for comet Enckle"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.35716517, posn.Z, "(Heliocentric Rect Coords Z) for comet Enckle"
  );

  struct ln_equ_posn equ_posn;
  ln_get_ell_body_equ_coords(o_JD, &orbit, &equ_posn);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 158.58242653, equ_posn.ra, "(RA) for comet Enckle"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 19.13924815, equ_posn.dec, "(Dec) for comet Enckle"
  );

  double l = ln_get_ell_orbit_len(&orbit);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 10.85028112, l, "(Orbit Length) for comet Enckle in AU"
  );

  double V = ln_get_ell_orbit_pvel(&orbit);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 70.43130198, V, "(Orbit Perihelion Vel) for comet Enckle in kms"
  );

  V = ln_get_ell_orbit_avel(&orbit);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 5.70160892, V, "(Orbit Aphelion Vel) for comet Enckle in kms"
  );

  V = ln_get_ell_orbit_vel(o_JD, &orbit);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 48.16148331, V, "(Orbit Vel JD) for comet Enckle in kms"
  );

  double dist = ln_get_ell_body_solar_dist(o_JD, &orbit);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.65203581, dist, "(Body Solar Dist) for comet Enckle in AU"
  );

  dist = ln_get_ell_body_earth_dist(o_JD, &orbit);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.00000001, 0.82481670, dist, "(Body Earth Dist) for comet Enckle in AU"
  );
}

void test_elliptic_motion_tno(void)
{
        //~ // TNO http://www.cfa.harvard.edu/mpec/K05/K05O42.html

  struct ln_date obs_date = {
    .years = 2006, .months  =  5, .days    = 5,
    .hours =    0, .minutes =  0, .seconds = 0
  };
  double o_JD = ln_get_julian_day(&obs_date);

  struct ln_date epoch_date = {
    .years = 2006, .months  =  3, .days    =  6,
    .hours =    0, .minutes =  0, .seconds =  0
  };
  double e_JD = ln_get_julian_day(&epoch_date);

  struct ln_ell_orbit orbit = {
    .JD    = e_JD,
    .a     =  45.7082927,
    .e     =   0.1550125,
    .i     =  28.99870,
    .omega =  79.55499,
    .w     = 296.40937,
    .n     =   0.00318942
  };

  // MPO refers to Mean anomaly & epoch, we hence need to convert epoch
  // to perihelion pass

  orbit.JD -= 147.09926 / orbit.n;

  struct ln_equ_posn equ_posn;
  ln_get_ell_body_equ_coords(o_JD, &orbit, &equ_posn);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.001, 184.3699999995, equ_posn.ra, "(RA) for TNO K05F09Y"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.001, 30.3316666666, equ_posn.dec, "(Dec) for TNO K05F09Y"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_elliptic_motion_enckle);
  RUN_TEST(test_elliptic_motion_tno);

  return UNITY_END();
}
