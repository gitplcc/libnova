/*
 * test_apparent_position.c
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
struct ln_equ_posn object, pm;

void setUp()
{
  /* objects position */
  struct lnh_equ_posn hobject = {
    .ra  = {.hours = 2, .minutes = 44, .seconds = 12.9747},
    .dec = {.neg = 0, .degrees = 49, .minutes = 13, .seconds = 39.896}
  };
  ln_hequ_to_equ(&hobject, &object);

  /* proper motion of object */
  struct lnh_equ_posn hpm = {
    .ra  = {.hours = 0, .minutes = 0, .seconds = 0.03425},
    .dec = {.neg = 1, .degrees = 0, .minutes = 0, .seconds = 0.0895}
  };
  ln_hequ_to_equ(&hpm, &pm);

  JD = 2462088.69;
}

void tearDown()
{
}

void test_ln_get_apparent_posn(void)
{
  struct ln_equ_posn pos;
  ln_get_apparent_posn(&object, &pm, JD, &pos);

  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 41.56406641, pos.ra, "Apparent RA on JD 2462088.69"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 49.35135029, pos.dec, "Apparent RA on JD 2462088.69"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_apparent_posn);

  return UNITY_END();
}
