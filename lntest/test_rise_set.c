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

const double jd_per_minute = 1.0 / (24.0 * 60.0);
const double jd_per_second = 1.0 / (24.0 * 60.0 * 60.0);

double JD;
struct ln_equ_posn object;
struct ln_lnlat_posn observer;

void setUp()
{
  JD = 2453752.5;  /* 2006/01/17 00:00:00 TU */
  object =  /* Arcturus apparent degrees */
    (struct ln_equ_posn) {.ra  = 210.2655825, .dec =  19.1470667};
  observer =  /* 51N 15E */
    (struct ln_lnlat_posn) {.lng = 15.0, .lat = 51.0};
}

void tearDown()
{
}

void test_ln_get_object_rst_arcturus_51N15E(void)
{
  struct ln_rst_time rst;
  int ret = ln_get_object_rst(JD, &observer, &object, &rst);

  TEST_ASSERT_EQUAL_INT(0, ret);
  TEST_ASSERT_DOUBLE_WITHIN(0.011, 2453753.40375, rst.rise);    /* 21:41:24 TU */
  TEST_ASSERT_DOUBLE_WITHIN(0.011, 2453752.72911, rst.transit); /* 05:29:55 TU */
  TEST_ASSERT_DOUBLE_WITHIN(0.011, 2453753.05174, rst.set);     /* 13:14:30 TU */
}

void test_ln_get_object_rst_arcturus_51S15E(void)
{
  observer.lat = -51.0;

  struct ln_rst_time rst;
  int ret = ln_get_object_rst(JD, &observer, &object, &rst);

  TEST_ASSERT_EQUAL_INT(0, ret);
  TEST_ASSERT_DOUBLE_WITHIN(0.011, 2453752.54722, rst.rise);    /* 01:08:00 TU */
  TEST_ASSERT_DOUBLE_WITHIN(0.011, 2453752.72911, rst.transit); /* 05:29:55 TU */
  TEST_ASSERT_DOUBLE_WITHIN(0.011, 2453752.91100, rst.set);     /* 09:51:50 TU */
}

void test_ln_get_object_rst_several(void)
{
  struct ln_rst_time rst;

  /* For observer at N37º */
  observer.lat = 37;

  /* Object at dec -54 never rise */
  object.dec = -54;
  TEST_ASSERT_EQUAL_INT(-1, ln_get_object_rst(JD, &observer, &object, &rst));

  /* Object at dec -52 rise */
  object.dec = -52;
  TEST_ASSERT_EQUAL_INT(0, ln_get_object_rst(JD, &observer, &object, &rst));

  /* Object at dec 54 is always above the horizon */
  object.dec = 54;
  TEST_ASSERT_EQUAL_INT(1, ln_get_object_rst(JD, &observer, &object, &rst));

  /* Object at dec 52 rise */
  object.dec = 52;
  TEST_ASSERT_EQUAL_INT(0, ln_get_object_rst(JD, &observer, &object, &rst));

  /* For observer at S37º */
  observer.lat = -37;

  /* Object at dec 54 never rise */
  object.dec = 54;
  TEST_ASSERT_EQUAL_INT(-1, ln_get_object_rst(JD, &observer, &object, &rst));

  /* Object at dec 52 rise */
  object.dec = 52;
  TEST_ASSERT_EQUAL_INT(0, ln_get_object_rst(JD, &observer, &object, &rst));

  /* Object at dec -54 is always above the horizon */
  object.dec = -54;
  TEST_ASSERT_EQUAL_INT(1, ln_get_object_rst(JD, &observer, &object, &rst));

  /* Object at dec -52 rise */
  object.dec = -52;
  TEST_ASSERT_EQUAL_INT(0, ln_get_object_rst(JD, &observer, &object, &rst));
}

void test_ln_get_object_next_rst(void)
{
  struct ln_rst_time rst;
  int ret = ln_get_object_rst(JD, &observer, &object, &rst);
  TEST_ASSERT_EQUAL_INT_MESSAGE(
    0, ret, "Arcturus sometimes rise at 15 E, 51 N"
  );

  double JD_next = rst.transit - 0.001;
  ret = ln_get_object_next_rst(JD_next, &observer, &object, &rst);
  TEST_ASSERT_EQUAL_INT_MESSAGE(
    0, ret, "Arcturus sometimes rise at 15 E, 51 N"
  );
  TEST_ASSERT_MESSAGE(
    (JD_next < rst.transit), "Arcturus next date is less then transit time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.transit < rst.set), "Arcturus next transit time is less then set time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.set < rst.rise), "Arcturus next set time is less then rise time"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453753.40278, rst.rise,
    "Arcturus next rise on 2006/01/17 at (15 E,51 N)"
  ); /* 21:40 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453752.72847, rst.transit,
    "Arcturus next transit on 2006/01/17 at (15 E,51 N)"
  ); /* 05:29 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453753.05139, rst.set,
    "Arcturus next set on 2006/01/17 at (15 E,51 N)"
  ); /* 13:14 TU */

  JD_next = rst.set - 0.001;
  ret = ln_get_object_next_rst(JD_next, &observer, &object, &rst);
  TEST_ASSERT_EQUAL_INT_MESSAGE(
    0, ret, "Arcturus sometimes rise at 15 E, 51 N"
  );
  TEST_ASSERT_MESSAGE(
    (JD_next < rst.set), "Arcturus next date is less then set time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.set < rst.rise), "Arcturus next set time is less then rise time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.rise < rst.transit),
    "Arcturus next rise time is less then transit time"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453753.40278, rst.rise,
    "Arcturus next rise on 2006/01/17 at (15 E,51 N)"
  ); /* 21:40 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453753.72569, rst.transit,
    "Arcturus next transit on 2006/01/17 at (15 E,51 N)"
  ); /* 05:25 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453753.05139, rst.set,
    "Arcturus next set on 2006/01/17 at (15 E,51 N)"
  );    /* 13:14 TU */

  JD_next = rst.rise - 0.001;
  ret = ln_get_object_next_rst(JD_next, &observer, &object, &rst);
  TEST_ASSERT_EQUAL_INT_MESSAGE(
    0, ret, "Arcturus sometimes rise at 15 E, 51 N"
  );
  TEST_ASSERT_MESSAGE(
    (JD_next < rst.rise), "Arcturus next date is less then rise time");
  TEST_ASSERT_MESSAGE(
    (rst.rise < rst.transit), "Arcturus next rise time is less then transit time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.transit < rst.set), "Arcturus next set time is less then transit time"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453753.40278, rst.rise,
    "Arcturus next rise on 2006/01/17 at (15 E,51 N)"
  ); /* 21:40 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453753.72569, rst.transit,
    "Arcturus next transit on 2006/01/17 at (15 E,51 N)"
  ); /* 05:25 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453754.04861, rst.set,
    "Arcturus next set on 2006/01/17 at (15 E,51 N)"
  );    /* 13:10 TU */

  JD_next = rst.rise + 0.001;
  ret = ln_get_object_next_rst(JD_next, &observer, &object, &rst);
  TEST_ASSERT_EQUAL_INT_MESSAGE(
    0, ret, "Arcturus sometimes rise at 15 E, 51 N"
  );
  TEST_ASSERT_MESSAGE(
    (JD_next < rst.transit), "Arcturus next date is less then transit time");
  TEST_ASSERT_MESSAGE(
    (rst.transit < rst.set), "Arcturus next transit time is less then set time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.set < rst.rise), "Arcturus next set time is less then rise time"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453754.40069, rst.rise,
    "Arcturus next rise on 2006/01/17 at (15 E,51 N)"
  ); /* 21:37 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453753.72569, rst.transit,
    "Arcturus next transit on 2006/01/17 at (15 E,51 N)"
  ); /* 05:25 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453754.04861, rst.set,
    "Arcturus next set on 2006/01/17 at (15 E,51 N)"
  ); /* 13:10 TU */
}

void test_ln_get_object_rst_horizon(void)
{
  observer.lat = -51.0;

  struct ln_rst_time rst;
  int ret = ln_get_object_rst_horizon(JD, &observer, &object, -20.0, &rst);

  /* Arcturus sometimes rise above -20 deg at 15 E, 51 S */
  TEST_ASSERT_EQUAL_INT(0, ret);
  TEST_ASSERT_DOUBLE_WITHIN(0.011, 2453753.45139, rst.rise);    /* 22:50 TU */
  TEST_ASSERT_DOUBLE_WITHIN(0.011, 2453752.72222, rst.transit); /* 05:20 TU */
  TEST_ASSERT_DOUBLE_WITHIN(0.011, 2453753.00278, rst.set);     /* 12:04 TU */
}

void test_ln_get_object_next_rst_horizon(void)
{
  struct ln_rst_time rst;
  int ret = ln_get_object_rst_horizon(JD, &observer, &object, 20, &rst);

  TEST_ASSERT_EQUAL_INT_MESSAGE(
    0, ret, "Arcturus sometimes rise above 20 deg at 15 E, 51 N"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453753.50417, rst.rise,
    "Arcturus rise above 20 deg hour on 2006/01/17 at (15 E,51 N)"
  ); /* 00:06 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453752.72847, rst.transit,
    "Arcturus transit hour on 2006/01/17 at (15 E,51 N)"
  ); /* 05:29 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.011, 2453752.95278, rst.set,
    "Arcturus set bellow 20 deg hour on 2006/01/17 at (15 E,51 N)"
  ); /* 10:52 TU */

  double JD_next = rst.rise - 0.002;
  ret = ln_get_object_next_rst_horizon(JD_next, &observer, &object, 20, &rst);

  TEST_ASSERT_EQUAL_INT_MESSAGE(
    0, ret, "Arcturus sometimes rise above 20 deg at 15 E, 51 N"
  );
  TEST_ASSERT_MESSAGE(
    (JD_next < rst.rise), "Arcturus next date is less than rise time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.rise < rst.transit), "Arcturus next rise time is less than transit time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.transit < rst.set), "Arcturus next transit time is less than set time"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453753.50417, rst.rise,
    "Arcturus next rise above 20 deg hour on 2006/01/17 at (15 E,51 N)"
  ); /* 00:06 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453753.72847, rst.transit,
    "Arcturus next transit hour on 2006/01/17 at (15 E,51 N)"
  ); /* 05:29 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453753.95278, rst.set,
    "Arcturus next set below 20 deg hour on 2006/01/17 at (15 E,51 N)"
  ); /* 10:52 TU */
  JD_next = rst.transit - 0.001;
  ret = ln_get_object_next_rst_horizon(JD_next, &observer, &object, 20, &rst);

  TEST_ASSERT_EQUAL_INT_MESSAGE(
    0, ret, "Arcturus sometimes rise above 20 deg at 15 E, 51 N"
  );
  TEST_ASSERT_MESSAGE(
    (JD_next < rst.transit), "Arcturus next date is less then transit time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.transit < rst.set), "Arcturus next transit time is less then set time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.set < rst.rise), "Arcturus next set time is less then rise time"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453754.50139, rst.rise,
    "Arcturus next rise above 20 deg on 2006/01/18 at (15 E,51 N)"
  ); /* 00:02 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453753.72847, rst.transit,
    "Arcturus next transit on 2006/01/17 at (15 E,51 N)"
  ); /* 05:29 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453753.95278, rst.set,
    "Arcturus next set below 20 deg on 2006/01/17 at (15 E,51 N)"
  ); /* 10:52 TU */

  JD_next = rst.set - 0.001;
  ret = ln_get_object_next_rst_horizon(JD_next, &observer, &object, 20, &rst);
  TEST_ASSERT_EQUAL_INT_MESSAGE(
    0, ret, "Arcturus sometimes rise above 20 deg at 15 E, 51 N"
  );
  TEST_ASSERT_MESSAGE(
    (JD_next < rst.set), "Arcturus next date is less then set time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.set < rst.rise), "Arcturus next set time is less then rise time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.rise < rst.transit), "Arcturus next rise time is less then transit time"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453754.50139, rst.rise,
    "Arcturus next rise above 20 deg on 2006/01/18 at (15 E,51 N)"
  ); /* 00:02 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453754.72569, rst.transit,
    "Arcturus next transit on 2006/01/18 at (15 E,51 N)"
  ); /* 05:25 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453753.95278, rst.set,
    "Arcturus next set below 20 deg on 2006/01/17 at (15 E,51 N)"
  ); /* 10:52 TU */

  JD_next = rst.set + 0.001;
  ret = ln_get_object_next_rst_horizon(JD_next, &observer, &object, 20, &rst);
  TEST_ASSERT_EQUAL_INT_MESSAGE(
    0, ret, "Arcturus sometimes rise above 20 deg at 15 E, 51 N"
  );
  TEST_ASSERT_MESSAGE(
    (JD_next < rst.rise), "Arcturus next date is less then rise time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.rise < rst.transit), "Arcturus next rise time is less then transit time"
  );
  TEST_ASSERT_MESSAGE(
    (rst.transit < rst.set), "Arcturus next transit time is less then set time"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453754.50139, rst.rise,
    "Arcturus next rise above 20 deg hour on 2006/01/18 at (15 E,51 N)"
  ); /* 00:02 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453754.72569, rst.transit,
    "Arcturus next transit hour on 2006/01/18 at (15 E,51 N)"
  ); /* 05:25 TU */
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    0.013, 2453754.95000, rst.set,
    "Arcturus next set bellow 20 deg hour on 2006/01/18 at (15 E,51 N)"
  ); /* 10:48 TU */
}

void test_ln_get_solar_rst(void)
{
  struct ln_rst_time rst;
  int ret = ln_get_solar_rst(JD, &observer, &rst);

  /* Sun sometimes rise on 2006/01/17 at 15 E, 51 S */
  TEST_ASSERT_EQUAL_INT(0, ret);
  TEST_ASSERT_DOUBLE_WITHIN(jd_per_minute, 2453752.78824, rst.rise);    /* 06:55:04 TU */
  TEST_ASSERT_DOUBLE_WITHIN(jd_per_minute, 2453752.96534, rst.transit); /* 11:10:05 TU */
  TEST_ASSERT_DOUBLE_WITHIN(jd_per_minute, 2453753.14275, rst.set);     /* 15:25:34 TU */
}

void test_ln_get_venus_rst(void)
{
  double JD = 2447240.5; /* 1988/03/20 00:00:00 */
  struct ln_lnlat_posn observer = {.lng = -71.0833, .lat = 42.3333}; /*  Boston */

  struct ln_rst_time rst;
  int ret = ln_get_venus_rst(JD, &observer, &rst);

  TEST_ASSERT_EQUAL_INT(0, ret);
  TEST_ASSERT_DOUBLE_WITHIN(jd_per_minute, 2447241.01764, rst.rise);    /* 12:25:24 TU */
  TEST_ASSERT_DOUBLE_WITHIN(jd_per_minute, 2447241.31981, rst.transit); /* 19:40:31 TU */
  TEST_ASSERT_DOUBLE_WITHIN(jd_per_minute, 2447240.62135, rst.set);     /* 02:54:45 TU */
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_object_rst_arcturus_51N15E);
  RUN_TEST(test_ln_get_object_rst_arcturus_51S15E);
  RUN_TEST(test_ln_get_object_rst_several);
  RUN_TEST(test_ln_get_object_next_rst);
  RUN_TEST(test_ln_get_object_rst_horizon);
  RUN_TEST(test_ln_get_object_next_rst_horizon);
  RUN_TEST(test_ln_get_solar_rst);
  RUN_TEST(test_ln_get_venus_rst);

  return UNITY_END();
}
