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

#define ARCSEC_TENTH  (0.1 / 3600.0)

void setUp()
{
}

void tearDown()
{
}

void test_get_ecl_from_equ()
{
  struct ln_equ_posn object = { /* Pollux coordinates (apparent) */
    .ra  = 105.778176, /* 07h 44' 32.294" */
    .dec =  27.972272  /* 28° 03' 27.66" */
  }; //alt60.443811 az103.794356
  double JD = 2446896.22292;  /* 1987/4/10 19:21:0.0 UTC */

  struct ln_lnlat_posn ecl;
  ln_get_ecl_from_equ(&object, JD, &ecl);

  printf("L =\t% 3.12f\nB =\t% 3.12f\n", ecl.lng, ecl.lat);
  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH, 113.526097, ecl.lng);
  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH,   6.687989, ecl.lat);
}

void test_get_ecl_from_rect()
{
  TEST_IGNORE();
}

void test_get_equ2000_from_gal()
{
  TEST_IGNORE();
}

void test_get_equ_from_ecl()
{
  struct ln_lnlat_posn ecl = {.lng = 113.039166667, .lat = 6.68388888889};
  double JD = 2446896.22292;  /* 1987/4/10 19:21:0.0 UTC */

  struct ln_equ_posn equ;
  ln_get_equ_from_ecl(&ecl, JD, &equ);

  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH, 116.32894167, equ.ra);
  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH,  28.02618333, equ.dec);
}

void test_get_equ_from_gal()
{
  /* Gal pole */
  struct ln_gal_posn gal = {.l = 0.0, .b = 90.0};

  struct ln_equ_posn equ;
  ln_get_equ_from_gal(&gal, &equ);

  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH, 192.25, equ.ra);
  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH,  27.40, equ.dec);
}

void test_get_equ_from_hrz()
{
  struct ln_hrz_posn object_hrz = {.az = 65.015, .alt = 14.0167777778};
  struct ln_lnlat_posn observer = {.lng = 282.934444444, .lat = 38.9213888889};
  double JD = 2446896.22292;  /* 1987/4/10 19:21:0.0 UTC */

  struct ln_equ_posn object_equ;
  ln_get_equ_from_hrz(&object_hrz, &observer, JD, &object_equ);

  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH, 192.25, object_equ.ra);
  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH,  27.40, object_equ.dec);
}

void test_get_gal_from_equ()
{
  /* Gal pole */
  struct ln_equ_posn equ = {.ra = 192.25, .dec =  27.4};

  struct ln_gal_posn gal;
  ln_get_gal_from_equ(&equ, &gal);

  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH, 90.0, gal.b);
}

void test_get_gal_from_equ2000()
{
  // Swift triger 174738
  struct ln_equ_posn equ = {.ra = 125.2401, .dec =  31.9260};

  struct ln_gal_posn gal;
  ln_get_gal_from_equ2000(&equ, &gal);

  TEST_ASSERT_DOUBLE_WITHIN(0.005, 190.54, gal.l);
  TEST_ASSERT_DOUBLE_WITHIN(0.005,  31.92, gal.b);
}

void test_get_hrz_from_equ()
{
  struct ln_equ_posn object = { /* Pollux coordinates (apparent) */
    .ra  = 105.742299722,  /* 07h 44' 32.294" */
    .dec =  28.0608194444  /* 28° 03' 27.66" */

  };
  /* Observer site */
  struct ln_lnlat_posn observer = {
    .lng = 282.934444444,  /* 282° 56'  4.0" */
    .lat = 38.9213888889   /*  38° 55' 17.0" */
  };
  double JD = 2446896.22292;  /* 1987/4/10 19:21:0.0 UTC */

  struct ln_hrz_posn hrz;
  ln_get_hrz_from_equ(&object, &observer, JD, &hrz);

  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH, 15.12426274, hrz.alt);
  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH, 68.03429264, hrz.az);
}

void test_get_hrz_from_equ_sidereal_time()
{
  struct ln_equ_posn object = { /* Pollux coordinates (apparent) */
    .ra  = 105.742299722,  /* 07h 44' 32.294" */
    .dec =  28.0608194444  /* 28° 03' 27.66" */

  };

  /* Observer site */
  struct ln_lnlat_posn observer = {
    .lng = 282.934444444,  /* 282° 56'  4.0" */
    .lat =  38.9213888889  /*  38° 55' 17.0" */
  };
  /* Observer local sidereal time */
  double lst = 1.8725;     /* 01h 26m 21s */

  struct ln_hrz_posn hrz;
  ln_get_hrz_from_equ_sidereal_time(&object, &observer, lst, &hrz);

  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH, 15.12426274, hrz.alt);
  TEST_ASSERT_DOUBLE_WITHIN(ARCSEC_TENTH, 68.03429264, hrz.az);
}

void test_get_rect_from_helio()
{
  TEST_IGNORE();
}

void test_get_nutation()
{
  double JD = 2446896.22292;  /* 1987/4/10 19:21:0.0 UTC */

  struct ln_nutation nutation;
  ln_get_nutation(JD, &nutation);

  TEST_ASSERT_EQUAL_DOUBLE( 0.002629211838, nutation.obliquity);
  TEST_ASSERT_EQUAL_DOUBLE(23.4409460039, nutation.ecliptic);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_get_nutation);

  RUN_TEST(test_get_ecl_from_equ);
  RUN_TEST(test_get_ecl_from_rect);
  RUN_TEST(test_get_equ2000_from_gal);
  RUN_TEST(test_get_equ_from_ecl);
  RUN_TEST(test_get_equ_from_gal);
  RUN_TEST(test_get_equ_from_hrz);
  RUN_TEST(test_get_gal_from_equ);
  RUN_TEST(test_get_gal_from_equ2000);
  RUN_TEST(test_get_hrz_from_equ);
  RUN_TEST(test_get_hrz_from_equ_sidereal_time);
  RUN_TEST(test_get_rect_from_helio);

  return UNITY_END();
}
