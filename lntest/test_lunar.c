/*
 * test_lunar.c
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

void setUp()
{
  JD = 2448724.5;
}

void tearDown()
{
}

void test_ln_get_lunar_geo_posn(void)
{
  struct ln_rect_posn moon;
  ln_get_lunar_geo_posn(JD, &moon, 0);
  TEST_PRINTF("lunar x %f y %f z %f", moon.X, moon.Y, moon.Z);

  struct ln_lnlat_posn ecl;
  ln_get_lunar_ecl_coords(JD, &ecl, 0);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(0.2, 133.166667, ecl.lng, "lunar Long");
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(0.1,  -3.229167, ecl.lat, "lunar Lat");

  struct ln_equ_posn equ;
  ln_get_lunar_equ_coords_prec(JD, &equ, 0);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.2, 134.688470, equ.ra, "lunar apparent RA"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.1, 13.768368, equ.dec, "lunar apparent Dec"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.1, 368405.6, ln_get_lunar_earth_dist(JD), "lunar distance in km"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.01, 0.6786, ln_get_lunar_disk(JD), "lunar disk"
  );

  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.02, 69.075, ln_get_lunar_phase(JD), "lunar phase"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.1, 285.0, ln_get_lunar_bright_limb(JD), "lunar bright limb"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_lunar_geo_posn);

  return UNITY_END();
}
