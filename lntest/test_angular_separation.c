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

struct ln_equ_posn posn1, posn2;

void setUp()
{
  /* alpha Bootes (Arcturus) */
  posn1 = (struct ln_equ_posn) {
    .ra = 213.9154, .dec =  19.1825
  };

  /* alpha Virgo (Spica) */
  posn2 = (struct ln_equ_posn) {
    .ra = 201.2983, .dec = -11.1614
  };
}

void tearDown()
{
}

void test_ln_get_angular_separation(void)
{
  double d = ln_get_angular_separation(&posn1, &posn2);

  TEST_ASSERT_DOUBLE_WITHIN(1.0e-5, 32.79302684, d);
}

void test_ln_get_rel_posn_angle(void)
{
  double d = ln_get_rel_posn_angle(&posn1, &posn2);

  TEST_ASSERT_DOUBLE_WITHIN(1.0e-5, 22.39042787, d);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_angular_separation);
  RUN_TEST(test_ln_get_rel_posn_angle);

  return UNITY_END();
}
