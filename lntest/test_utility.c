/*
 * test_utility.c
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

#define MAS_IN_DEG  (2.7777777777777777777777777777778e-7L)
#define MAS_IN_RAD  (4.8481368110953599358991410235795e-9L)

double JD;
struct ln_equ_posn object;

void setUp()
{
}

void tearDown()
{
}

static void test_rad_to_deg()
{
  TEST_ASSERT_EQUAL_DOUBLE(90.0, ln_rad_to_deg(M_PI_2));
}

static void test_deg_to_rad()
{
  TEST_ASSERT_EQUAL_DOUBLE(M_PI_4, ln_deg_to_rad(45.0));
}

static void test_hms_to_deg()
{
  struct ln_hms hms = {.hours = 23, .minutes = 1, .seconds = 0.01};

  TEST_ASSERT_EQUAL_DOUBLE(345.25004166666666666666666666667, ln_hms_to_deg(&hms));
}

static void test_hms_to_rad()
{
  struct ln_hms hms = {.hours = 23, .minutes = 1, .seconds = 0.01};

  TEST_ASSERT_EQUAL_DOUBLE(6.0257499697309445286330327630781, ln_hms_to_rad(&hms));
}

static void test_deg_to_hms()
{
  struct ln_hms hms;
  ln_deg_to_hms(360345.25004166666666666666666667, &hms);
  TEST_ASSERT_EQUAL_INT(23, hms.hours);
  TEST_ASSERT_EQUAL_INT(1, hms.minutes);
  TEST_ASSERT_DOUBLE_WITHIN(1e-6, 0.01, hms.seconds);
}

static void test_rad_to_hms()
{
  struct ln_hms hms;
  ln_rad_to_hms(6289.2110571493174214539197993221, &hms);
  TEST_ASSERT_EQUAL_INT(23, hms.hours);
  TEST_ASSERT_EQUAL_INT(1, hms.minutes);
  TEST_ASSERT_DOUBLE_WITHIN(1e-6, 0.01, hms.seconds);
}

static void test_dms_to_deg()
{
  struct ln_dms dms = {.degrees = 359, .minutes = 1, .seconds = 0.01};

  TEST_ASSERT_EQUAL_DOUBLE(359.01666944444444444444444444444, ln_dms_to_deg(&dms));
}

static void test_dms_to_rad()
{
  struct ln_dms dms = {.degrees = 359, .minutes = 1, .seconds = 0.01};

  TEST_ASSERT_EQUAL_DOUBLE(6.2660229513496770137058031663269, ln_dms_to_rad(&dms));
}

static void test_deg_to_dms()
{
  struct ln_dms dms;
  ln_deg_to_dms(31345.25004166666666666666666667, &dms);
  TEST_ASSERT_EQUAL_INT(0, dms.neg);
  TEST_ASSERT_EQUAL_INT(31345, dms.degrees);
  TEST_ASSERT_EQUAL_INT(15, dms.minutes);
  TEST_ASSERT_DOUBLE_WITHIN(1e-6, 0.15, dms.seconds);
}

static void test_rad_to_dms()
{
  struct ln_dms dms;
  ln_rad_to_dms(547.07781808797311337497717099461, &dms);
  TEST_ASSERT_EQUAL_INT(0, dms.neg);
  TEST_ASSERT_EQUAL_INT(31345, dms.degrees);
  TEST_ASSERT_EQUAL_INT(15, dms.minutes);
  TEST_ASSERT_DOUBLE_WITHIN(1e-6, 0.15, dms.seconds);
}

static void test_range_degrees()
{
  TEST_ASSERT_DOUBLE_WITHIN(1e-9,
    2.77777777777777777778e-6,
    ln_range_degrees(360000.00000277777777777777777778)
  );
}

static void test_range_radians()
{
  TEST_ASSERT_EQUAL_DOUBLE(
    0.5873026462311421393814996765736,
    ln_range_radians(-57295.779513524417940942308524575)
  );
}

static void test_range_radians2()
{
  TEST_ASSERT_EQUAL_DOUBLE(
    -5.6958826609484443375437870899854,
    ln_range_radians2(-57295.779513524417940942308524575)
  );
}

void test_deg_to_dms_several(void)
{
  struct ln_dms dms;

  ln_deg_to_dms(-1.23, &dms);
  TEST_ASSERT(dms.neg);
  TEST_ASSERT_EQUAL_INT( 1, dms.degrees);
  TEST_ASSERT_EQUAL_INT(13, dms.minutes);
  TEST_ASSERT_EQUAL_DOUBLE(48.0, dms.seconds);

  ln_deg_to_dms(1.23, &dms);
  TEST_ASSERT(!dms.neg);
  TEST_ASSERT_EQUAL_INT( 1, dms.degrees);
  TEST_ASSERT_EQUAL_INT(13, dms.minutes);
  TEST_ASSERT_EQUAL_DOUBLE(48.0, dms.seconds);

  ln_deg_to_dms(-0.5, &dms);
  TEST_ASSERT(dms.neg);
  TEST_ASSERT_EQUAL_INT( 0, dms.degrees);
  TEST_ASSERT_EQUAL_INT(30, dms.minutes);
  TEST_ASSERT_EQUAL_DOUBLE(0.0, dms.seconds);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_rad_to_deg);
  RUN_TEST(test_deg_to_rad);
  RUN_TEST(test_hms_to_deg);
  RUN_TEST(test_hms_to_rad);
  RUN_TEST(test_deg_to_hms);
  RUN_TEST(test_rad_to_hms);
  RUN_TEST(test_dms_to_deg);
  RUN_TEST(test_dms_to_rad);
  RUN_TEST(test_deg_to_dms);
  RUN_TEST(test_rad_to_dms);
  RUN_TEST(test_range_degrees);
  RUN_TEST(test_range_radians);
  RUN_TEST(test_range_radians2);
  RUN_TEST(test_deg_to_dms_several);
  return UNITY_END();
}
