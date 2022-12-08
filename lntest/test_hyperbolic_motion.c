/*
 * test_hyperbolic_motion.c
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

void test_hyperbolic_motion_meeus_example(void)
{
  /* data from Meeus, chapter 35 */
  // the one from Meeus..
  struct ln_hyp_orbit orbit = {.q = 3.363943, .e = 1.05731};

  double v = ln_get_hyp_true_anomaly(orbit.q, orbit.e, 1237.1);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 109.40598, v,
    "(True Anomaly) v when q is 3.363943 and e is 1.05731"
  );

  double r = ln_get_hyp_radius_vector (orbit.q, orbit.e, 1237.1);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 10.668551, r,
    "(Radius Vector) r when q is 3.363943 and e is 1.05731"
  );
}

void test_hyperbolic_motion_c2001Q4neat(void)
{
  // and now something real.. C/2001 Q4 (NEAT)
  struct ln_date obs_date = {
    .years = 2004, .months  = 5, .days    = 15,
    .hours =    0, .minutes = 0, .seconds =  0
  };
  double o_JD = ln_get_julian_day (&obs_date);

  struct ln_date epoch_date = {
    .years = 2004, .months  =  5, .days    = 15,
    .hours =   23, .minutes = 12, .seconds = 37.44
  };
  double e_JD = ln_get_julian_day (&epoch_date);

  struct ln_hyp_orbit orbit = {
    .q     =   0.961957,
    .e     =   1.000744,
    .i     =  99.6426,
    .w     =   1.2065,
    .omega = 210.2785,
    .JD    = e_JD
  };

  double r = ln_get_hyp_radius_vector (orbit.q, orbit.e, o_JD - e_JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.001, 0.962, r, "(Radius Vector) r for C/2001 Q4 (NEAT)"
  );

  struct ln_equ_posn equ_posn;
  ln_get_hyp_body_equ_coords (o_JD, &orbit, &equ_posn);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.01, 128.01, equ_posn.ra, "(RA) for comet C/2001 Q4 (NEAT)"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.03, 18.3266666666, equ_posn.dec, "(Dec) for comet C/2001 Q4 (NEAT)"
  );

  double dist = ln_get_hyp_body_solar_dist (o_JD, &orbit);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.001, 0.962, dist, "(Body Solar Dist) for comet C/2001 Q4 (NEAT) in AU"
  );

  obs_date.years = 2005;
  obs_date.months = 1;

  o_JD = ln_get_julian_day (&obs_date);

  r = ln_get_hyp_radius_vector (orbit.q, orbit.e, o_JD - e_JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.001, 3.581, r, "(Radius Vector) r for C/2001 Q4 (NEAT)"
  );

  ln_get_hyp_body_equ_coords (o_JD, &orbit, &equ_posn);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.01, 332.9025, equ_posn.ra, "(RA) for comet C/2001 Q4 (NEAT)"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.001, 58.6116666666, equ_posn.dec, "(Dec) for comet C/2001 Q4 (NEAT)"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_hyperbolic_motion_meeus_example);
  RUN_TEST(test_hyperbolic_motion_c2001Q4neat);

  return UNITY_END();
}
