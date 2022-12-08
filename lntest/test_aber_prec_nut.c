/*
 * test_aber_prec_nut.c
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
struct lnh_equ_posn hmean_position;

void setUp()
{
  struct ln_date date = {
    .years = 2028, .months  = 11, .days    = 13,
    .hours =    4, .minutes = 33, .seconds = 36
  };

  JD = ln_get_julian_day(&date);

  hmean_position = (struct lnh_equ_posn) {
    .ra  = {.hours = 2, .minutes = 44, .seconds = 12.9747},
    .dec = {.neg = 0, .degrees = 49, .minutes = 13, .seconds = 39.896}
  };
}

void tearDown()
{
}

void test_aber_prec_nut(void)
{
  struct ln_equ_posn mean_position;
  ln_hequ_to_equ(&hmean_position, &mean_position);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 41.0540613, mean_position.ra, "Theta Persei mean RA"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 49.2277489, mean_position.dec, "Theta Persei mean DEC"
  );

  struct ln_equ_posn aberated;
  ln_get_equ_aber(&mean_position, JD, &aberated);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-4, 41.0623836, aberated.ra,
    "Theta Persei position on 13th November 2028 aberration RA"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 49.2296238, aberated.dec,
    "Theta Persei position on 13th November 2028 aberration DEC"
  );

  struct ln_equ_posn precessed;
  ln_get_equ_prec(&aberated, JD, &precessed);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-4, 41.5555635, precessed.ra,
    "Theta Persei position on 13th November 2028 aberration + precession RA"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 49.3503415, precessed.dec,
    "Theta Persei position on 13th November 2028 aberration + precession DEC"
  );

  struct ln_equ_posn nutated;
  ln_get_equ_nut(&precessed, JD, &nutated);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-4, 41.5599646, nutated.ra,
    "Theta Persei position on 13th November 2028 aberration + precession + nutation RA"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 49.3520685, nutated.dec,
    "Theta Persei position on 13th November 2028 aberration + precession + nutation DEC"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_aber_prec_nut);

  return UNITY_END();
}
