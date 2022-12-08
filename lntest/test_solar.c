/*
 * test_solar.c
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

void setUp()
{
}

void tearDown()
{
}

void test_ln_get_solar_geom_coords(void)
{
  struct ln_helio_posn pos;
  ln_get_solar_geom_coords(2448908.5, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 200.00810889, pos.L,
    "Longitude (deg) on JD 2448908.5"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 1.8690e-4, pos.B,
    "Latitude (deg) on JD 2448908.5"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-8, 0.99760852, pos.R,
    "Radius vector (AU) on JD 2448908.5"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_solar_geom_coords);

  return UNITY_END();
}
