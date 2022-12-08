/*
 * test_aberration.c
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
struct ln_equ_posn object;

void setUp()
{
  /* object position */
  struct lnh_equ_posn hobject = {
    .ra  = {.hours = 2, .minutes = 44, .seconds = 12.9747},
    .dec = {.neg   = 0, .degrees = 49, .minutes = 13, .seconds = 39.896}
  };
  ln_hequ_to_equ(&hobject, &object);

  /* date */
  struct ln_date date = {
    .years = 2028, .months  = 11, .days    = 13,
    .hours =    4, .minutes = 31, .seconds =  0
  };
  JD = ln_get_julian_day(&date);
}

void tearDown()
{
}

void test_ln_get_equ_aber(void)
{
  struct ln_equ_posn pos;
  ln_get_equ_aber(&object, JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 41.06238352, pos.ra, "RA aberration"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 49.22962359, pos.dec, "DEC aberration"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_equ_aber);

  return UNITY_END();
}
