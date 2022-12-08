/*
 * test_dynamical_time.c
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

double JD;

void setUp()
{
  /* 10/04/1987 19:21:00 */
  struct ln_date date =
  {
    .years   = 1987,
    .months  = 4,
    .days    = 10,
    .hours   = 19,
    .minutes = 21,
    .seconds = 0.0
  };

  JD = ln_get_julian_day(&date);
}

void tearDown()
{
}

void test_ln_get_mean_sidereal_time(void)
{
  double mst = ln_get_mean_sidereal_time(JD);

  TEST_ASSERT_DOUBLE_WITHIN(1.0e-8, 8.58252488, mst);
}

void test_ln_get_apparent_sidereal_time(void)
{
  double ast = ln_get_apparent_sidereal_time(JD);

  TEST_ASSERT_DOUBLE_WITHIN(1.0e-8, 8.58245327, ast);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_mean_sidereal_time);
  RUN_TEST(test_ln_get_apparent_sidereal_time);

  return UNITY_END();
}
