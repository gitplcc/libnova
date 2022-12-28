/*
 * test_nutation.c
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

#define MARCSEC  (0.001 / 3600.0)

void setUp()
{
}

void tearDown()
{
}

void test_ln_get_equ_nut(void)
{
  struct ln_equ_posn mean_position = {.ra = 41.5472125, .dec = 49.34848333}; /* Tetha Persei */
  double JD = 2462088.69;  /* 2028/11/13.19 TD */

  struct ln_equ_posn position;
  ln_get_equ_nut(&mean_position, JD, &position);

  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, 41.55161333, position.ra);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, 49.35021056, position.dec);
}

void test_ln_get_nutation(void)
{
  double JD = 2446895.5;  /* 1987/4/10 00:00:0.0 TD */

  struct ln_nutation nutation;
  ln_get_nutation(JD, &nutation);

  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, -1.052222222e-3, nutation.longitude);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC,  2.623055556e-3, nutation.obliquity);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC,  2.344094639e+1, nutation.ecliptic);

  JD = 2448908.5;  /* 1992/10/13 00:00:0.0 TD */

  ln_get_nutation(JD, &nutation);

  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC,  4.418888889e-3, nutation.longitude);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, -8.5555556e-5, nutation.obliquity);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC,  2.344023e+1, nutation.ecliptic);

  JD = 2462088.69;  /* 2028/11/13.19 TD */

  ln_get_nutation(JD, &nutation);

  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC,  4.128055556e-3, nutation.longitude);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC,  7.513888889e-4, nutation.obliquity);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC,  2.34355372e+1, nutation.ecliptic);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_equ_nut);
  RUN_TEST(test_ln_get_nutation);

  return UNITY_END();
}
