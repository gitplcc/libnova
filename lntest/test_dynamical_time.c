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

void setUp()
{
}

void tearDown()
{
}

void test_ln_get_dynamical_time_diff()
{
  double JD = 2448972.5;  /* 1992/12/16 00:00:00 UT */

  double TD = ln_get_dynamical_time_diff(JD);

  TEST_ASSERT_DOUBLE_WITHIN(0.1, 59.0, TD);
}

void test_ln_get_jde()
{
  double JD = 2451544.5;  /* 2000/01/01 00:00:00 UT */

  double JDE = ln_get_jde(JD);

  TEST_ASSERT_DOUBLE_WITHIN(1e-5, 2451544.50073877, JDE);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_dynamical_time_diff);
  RUN_TEST(test_ln_get_jde);

  return UNITY_END();
}
