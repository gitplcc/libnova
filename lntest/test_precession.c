/*
 * test_precession.c
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
  JD = 2462088.69;

  /* object position */
  struct lnh_equ_posn hobject = {
    .ra  = {.hours = 2, .minutes = 44, .seconds = 11.986},
    .dec = {.neg = 0, .degrees = 49, .minutes = 13, .seconds = 42.48}
  };

  ln_hequ_to_equ(&hobject, &object);

  pm = (struct ln_equ_posn) {
    .ra = 0.03425 * (15.0 / 3600.0), .dec = -0.0895 / 3600.0
  };
}

void tearDown()
{
}

void test_ln_get_equ_pm(void)
{
  ln_get_equ_pm(&object, &pm, JD, &object);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 41.054063, object.ra, "RA proper motion on JD 2462088.69"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 49.227750, object.dec, "DEC proper motion on JD 2462088.69"
  );
}

void test_ln_get_equ_prec(void)
{
  struct ln_equ_posn pos;
  ln_get_equ_prec(&object, JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    3.0e-5, 41.547212, pos.ra, "RA precession on JD 2462088.69"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 49.348483, pos.dec, "DEC precession on JD 2462088.69"
  );
}

void test_ln_get_equ_prec2(void)
{
  struct ln_equ_posn pos;
  ln_get_equ_prec2(&object, JD2000, JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 41.547212, pos.ra, "RA precession 2 on JD 2462088.69"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, 49.348483, pos.dec, "DEC precession 2 on JD 2462088.69"
  );

  struct ln_equ_posn pos2;
  ln_get_equ_prec2(&pos, JD, JD2000, &pos2);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, object.ra, pos2.ra, "RA precession 2 on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-5, object.dec, pos2.dec, "DEC precession 2 on JD 2451545.0"
  );
}

void test_ln_get_equ_ra_out_0_360(void)
{
  // INTEGRAL GRB050922A coordinates lead to RA not in <0-360> range
  struct ln_equ_posn pos = {.ra = 271.2473, .dec = -32.0227};

  struct ln_date grb_date = {
    .years = 2005, .months  =  9, .days    = 22,
    .hours =   13, .minutes = 43, .seconds = 18.0
  };

  JD = ln_get_julian_day(&grb_date);

  struct ln_equ_posn pos2;
  ln_get_equ_prec2(&pos, JD, JD2000, &pos2);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    2.0e-4, 271.1541, pos2.ra, "RA precession 2 on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    2.0e-4, -32.0235, pos2.dec, "DEC precession 2 on JD 2451545.0"
  );
}

void test_ln_get_equ_prec_close_pole(void)
{
  // second test from AA, p. 128
  struct lnh_equ_posn hobject = {
    .ra  = {.hours = 2, .minutes = 31, .seconds = 48.704},
    .dec = {.neg = 0, .degrees = 89, .minutes = 15, .seconds = 50.72}
  };

  ln_hequ_to_equ (&hobject, &object);

  // proper motions
  pm = (struct ln_equ_posn) {
    .ra  = ((long double) 0.19877) * (15.0 / 3600.0),
    .dec = ((long double) -0.0152) / 3600.0
  };

  struct ln_equ_posn pos, pos2;
  ln_get_equ_pm(&object, &pm, B1900, &pos);
  ln_get_equ_prec2(&pos, JD2000, B1900, &pos2);

  // the position is so close to pole, that it depends a lot on how precise
  // functions we will use. So we get such big errors compared to Meeus.
  // I checked results agains SLAlib on-line calculator and SLAlib performs
  // even worse then we

  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    2.0e-3, 20.6412499980, pos2.ra, "RA precession 2 on B1900"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-4, 88.7739388888, pos2.dec, "DEC precession 2 on B1900"
  );

  ln_get_equ_pm(&object, &pm, JD2050, &pos);
  ln_get_equ_prec2(&pos, JD2000, JD2050, &pos2);

  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    3.0e-3, 57.0684583320, pos2.ra, "RA precession 2 on J2050"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1.0e-4, 89.4542722222, pos2.dec, "DEC precession 2 on J2050"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_equ_pm);
  RUN_TEST(test_ln_get_equ_prec);
  RUN_TEST(test_ln_get_equ_prec2);
  RUN_TEST(test_ln_get_equ_ra_out_0_360);
  RUN_TEST(test_ln_get_equ_prec_close_pole);

  return UNITY_END();
}
