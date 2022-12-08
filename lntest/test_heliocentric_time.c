/*
 * test_heliocentric_time.c
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

struct ln_equ_posn object;
struct ln_date date;

void setUp()
{
  object = (struct ln_equ_posn) {.ra = 270.0, .dec = 50.0};
  date   = (struct ln_date) {
    .years = 2000, .months  = 1, .days    = 1,
    .hours =    0, .minutes = 0, .seconds = 0.0
  };
}

void tearDown()
{
}

void test_ln_get_heliocentric_time_diff_at_20000101_00h60d(void)
{
  object = (struct ln_equ_posn) {.ra = 0.0, .dec = 60.0};
  double JD = ln_get_julian_day(&date);

  double diff = ln_get_heliocentric_time_diff(JD, &object);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-4, 15.0e-4, diff,
    "TD for 01/01, object on 0h +60"
  );
}

void test_ln_get_heliocentric_time_diff_at_20000101_18h50d(void)
{
  double JD = ln_get_julian_day(&date);

  double diff = ln_get_heliocentric_time_diff(JD, &object);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-4, -16.0e-4, diff,
    "TD for 01/01, object on 18h +50"
  );
}

void test_ln_get_heliocentric_time_diff_at_20000808_18h50d(void)
{
  date = (struct ln_date) {.months = 8, .days = 8};
  double JD = ln_get_julian_day(&date);

  double diff = ln_get_heliocentric_time_diff(JD, &object);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-4, 12.0e-4, diff,
    "TD for 08/08, object on 18h +50"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_heliocentric_time_diff_at_20000101_00h60d);
  RUN_TEST(test_ln_get_heliocentric_time_diff_at_20000101_18h50d);
  RUN_TEST(test_ln_get_heliocentric_time_diff_at_20000808_18h50d);

  return UNITY_END();
}
