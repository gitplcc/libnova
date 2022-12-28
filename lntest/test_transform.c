/*
 * test_transform.c
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

void setUp()
{
}

void tearDown()
{
}

void test_get_ecl_from_equ(void)
{
  /* Pollux equatorial (2000) */
  struct ln_equ_posn object = {
    .ra  = 116.32894167, /* 07h 45' 18.946" */
    .dec =  28.02618333  /* 28° 01' 34.26" */
  };
  double JD = 2446896.30625;  /* 1987/4/10 19:21:0.0 UTC */

  struct ln_lnlat_posn ecl;
  ln_get_ecl_from_equ(&object, JD, &ecl);

  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, 113.21555278, ecl.lng);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC,   6.68264899, ecl.lat);
}

void test_get_ecl_from_rect(void)
{
  TEST_IGNORE();
}

void test_get_equ2000_from_gal(void)
{
  TEST_IGNORE();
}

void test_get_equ_from_ecl(void)
{
  struct ln_lnlat_posn ecl = {.lng =  113.21555278, .lat = 6.68264899};
  double JD = 2446896.30625;  /* 1987/4/10 19:21:0.0 UTC */

  struct ln_equ_posn equ;
  ln_get_equ_from_ecl(&ecl, JD, &equ);

  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, 116.32894167, equ.ra);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC,  28.02618333, equ.dec);
}

void test_get_equ_from_gal(void)
{
  /* Gal pole */
  struct ln_gal_posn gal = {.l = 0.0, .b = 90.0};

  struct ln_equ_posn equ;
  ln_get_equ_from_gal(&gal, &equ);

  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, 192.25, equ.ra);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC,  27.40, equ.dec);
}

void test_get_equ_from_hrz(void)
{
  /* Object position */
  struct ln_equ_posn object = {
    .ra  = 347.3193375,  /* 23h 09' 16.641" */
    .dec =  -6.71989166  /* -6° 43' 11.61" */

  };
  /* Observer site */
  struct ln_lnlat_posn observer = {
    .lng = 282.934444444,  /* 282° 56'  4.0" */
    .lat = 38.9213888889   /*  38° 55' 17.0" */
  };

  double JD = 2446896.30625;  /* Observer date: 1987/4/10 19:21:0.0 UTC */

  struct ln_hrz_posn hrz;
  ln_get_hrz_from_equ_sidereal_time(
    &object,
    &observer,
    ln_get_apparent_sidereal_time(JD),
    &hrz
  );

  struct ln_equ_posn object_equ;
  ln_get_equ_from_hrz(&hrz, &observer, JD, &object_equ);

  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, object.ra, object_equ.ra);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, object.dec, object_equ.dec);
}

void test_get_gal_from_equ(void)
{
  /* Gal pole */
  struct ln_equ_posn equ = {.ra = 192.25, .dec =  27.4};

  struct ln_gal_posn gal;
  ln_get_gal_from_equ(&equ, &gal);

  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, 90.0, gal.b);
}

void test_get_gal_from_equ2000(void)
{
  // Swift triger 174738
  struct ln_equ_posn equ = {.ra = 125.2401, .dec =  31.9260};

  struct ln_gal_posn gal;
  ln_get_gal_from_equ2000(&equ, &gal);

  TEST_ASSERT_DOUBLE_WITHIN(0.005, 190.54, gal.l);
  TEST_ASSERT_DOUBLE_WITHIN(0.005,  31.92, gal.b);
}

void test_get_hrz_from_equ(void)
{
  /* Object position */
  struct ln_equ_posn object = {
    .ra  = 347.3193375,  /* 23h 09' 16.641" */
    .dec =  -6.71989166  /* -6° 43' 11.61" */

  };
  /* Observer site */
  struct ln_lnlat_posn observer = {
    .lng = 282.934444444,  /* 282° 56'  4.0" */
    .lat = 38.9213888889   /*  38° 55' 17.0" */
  };
  double JD = 2446896.30625;  /* 1987/4/10 19:21:0.0 UTC */

  struct ln_hrz_posn hrz;
  ln_get_hrz_from_equ(&object, &observer, JD, &hrz);

  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, 15.12426274, hrz.alt);
  TEST_ASSERT_DOUBLE_WITHIN(MARCSEC, 68.03429264, hrz.az);
}

void test_get_rect_from_helio(void)
{
  TEST_IGNORE();
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_get_ecl_from_equ);
  RUN_TEST(test_get_ecl_from_rect);
  RUN_TEST(test_get_equ2000_from_gal);
  RUN_TEST(test_get_equ_from_ecl);
  RUN_TEST(test_get_equ_from_gal);
  RUN_TEST(test_get_equ_from_hrz);
  RUN_TEST(test_get_gal_from_equ);
  RUN_TEST(test_get_gal_from_equ2000);
  RUN_TEST(test_get_hrz_from_equ);
  RUN_TEST(test_get_rect_from_helio);

  return UNITY_END();
}
