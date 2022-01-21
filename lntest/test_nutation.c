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

#define ARCSEC_TENTH  (0.1 / 3600.0)

void setUp()
{
}

void tearDown()
{
}

void test_ln_get_equ_nut()
{
  struct ln_equ_posn mean_position = {.ra = 41.5472125, .dec = 49.3484833}; /* Tetha Persei */
  double JD = 2462088.69;  /* 2028/11/13.19 TD */

  struct ln_equ_posn position;
  ln_get_equ_nut(&mean_position, JD, &position);

  TEST_ASSERT_DOUBLE_WITHIN(1.0e-6, 41.5599583, position.ra);
  TEST_ASSERT_DOUBLE_WITHIN(1.0e-6, 49.3484833, position.dec);
}

void test_ln_get_nutation()
{
  double JD = 2446895.5;  /* 1987/4/10 00:00:0.0 TD */

  struct ln_nutation nutation;
  ln_get_nutation(JD, &nutation);

  TEST_ASSERT_DOUBLE_WITHIN(1.0e-6, -1.052222222e-3, nutation.longitude);
  TEST_ASSERT_DOUBLE_WITHIN(1.0e-6,  2.623055556e-3, nutation.obliquity);
  TEST_ASSERT_DOUBLE_WITHIN(1.0e-6,  2.344094639e+1, nutation.ecliptic);

  JD = 2462088.69;  /* 2028/11/13.19 TD */

  ln_get_nutation(JD, &nutation);

  TEST_ASSERT_DOUBLE_WITHIN(1.0e-6,  7.513888889e-4, nutation.longitude);
  TEST_ASSERT_DOUBLE_WITHIN(1.0e-6,  4.128055556e-3, nutation.obliquity);
  TEST_ASSERT_DOUBLE_WITHIN(1.0e-6,  2.3436e+1, nutation.ecliptic);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_equ_nut);
  RUN_TEST(test_ln_get_nutation);

  return UNITY_END();
}
