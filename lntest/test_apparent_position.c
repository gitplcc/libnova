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

#define MARCSEC  (0.001 / 3600.0)

double JD;
struct ln_equ_posn object, pm;

void setUp()
{
  /* objects position */
  object = (struct ln_equ_posn) {.ra  = 41.04994167, .dec = 49.22846667};

  /* proper motion of object */
  pm =  (struct ln_equ_posn) {.ra  =  1.4270833333e-4, .dec = -2.4861111111e-5};

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
    MARCSEC, 41.5599646, pos.ra, "Apparent RA on JD 2462088.69"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    MARCSEC, 49.3520685, pos.dec, "Apparent RA on JD 2462088.69"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_apparent_posn);

  return UNITY_END();
}
