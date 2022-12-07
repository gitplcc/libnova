/*
 * test_airmass.c
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

void test_ln_get_airmass_at_zenit(void)
{
  double X = ln_get_airmass(90.0, 750.0);
  TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, X, "Airmass at Zenith");
}

void test_ln_get_airmass_at_10degrees(void)
{
  double X = ln_get_airmass(10.0, 750.0);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.1, 5.64, X, "Airmass at 10 degrees altitude"
  );
}

void test_ln_get_alt_from_airmass_at_1airmass(void)
{
  double X = ln_get_alt_from_airmass(1.0, 750.0);
  TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(90.0, X, "Altitude at airmass 1");
}

void test_ln_get_alt_from_airmass_roundloop(void)
{
  for (double x = -10.0; x < 90.0; x += 10.54546456)
  {
    char msg[200];
    sprintf(msg, "Altitude->Airmass->Altitude at %.2f degrees", x);
    double X = ln_get_alt_from_airmass(ln_get_airmass(x, 750.0), 750.0);
    TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(1.0e-9, x, X, msg);
  }
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_airmass_at_zenit);
  RUN_TEST(test_ln_get_airmass_at_10degrees);
  RUN_TEST(test_ln_get_alt_from_airmass_at_1airmass);
  RUN_TEST(test_ln_get_alt_from_airmass_roundloop);

  return UNITY_END();
}
