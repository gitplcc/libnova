/*
 * test_julian_day.c
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

#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <time.h>

static char *tz_old;

void setUp()
{
  const char *tz_current = getenv("TZ");
  if (tz_current)
  {
    size_t buf_size = strlen(tz_current) + 1;
    tz_old = malloc(buf_size);
    strncpy(tz_old, tz_current, buf_size);
  }
  else
    tz_old = NULL;

  setenv("TZ", "CET", 1);
}

void tearDown()
{
  if (!tz_old)
    return;

  setenv("TZ", tz_old, 1);
  free(tz_old);
}

void test_ln_date_to_zonedate()
{
  struct ln_date date = {
    .years   = 1954,
    .months  =    6,
    .days    =   30,
    .hours   =    0,
    .minutes =    0,
    .seconds =    0
  };
  struct ln_zonedate zonedate;
  ln_date_to_zonedate(&date, &zonedate, 7200);

  TEST_ASSERT_EQUAL(1954, zonedate.years);
  TEST_ASSERT_EQUAL(   6, zonedate.months);
  TEST_ASSERT_EQUAL(  30, zonedate.days);
  TEST_ASSERT_EQUAL(   2, zonedate.hours);
  TEST_ASSERT_EQUAL(   0, zonedate.minutes);
  TEST_ASSERT_DOUBLE_WITHIN(1e-4, 0.0, zonedate.seconds);
  TEST_ASSERT_EQUAL(7200, zonedate.gmtoff);
}

void test_ln_get_date()
{
  double JD = 2434923.5;
  struct ln_date date;
  ln_get_date(JD, &date);

  TEST_ASSERT_EQUAL(1954, date.years);
  TEST_ASSERT_EQUAL(   6, date.months);
  TEST_ASSERT_EQUAL(  30, date.days);
  TEST_ASSERT_EQUAL(   0, date.hours);
  TEST_ASSERT_EQUAL(   0, date.minutes);
  TEST_ASSERT_EQUAL_DOUBLE(0.0, date.seconds);
}

void test_ln_get_date_from_mpc()
{
  struct ln_date date;
  int ret = ln_get_date_from_mpc(&date, "K01AM");

  TEST_ASSERT_EQUAL(0, ret);
  TEST_ASSERT_EQUAL(2001, date.years);
  TEST_ASSERT_EQUAL(  10, date.months);
  TEST_ASSERT_EQUAL(  22, date.days);
  TEST_ASSERT_EQUAL(   0, date.hours);
  TEST_ASSERT_EQUAL(   0, date.minutes);
  TEST_ASSERT_EQUAL_DOUBLE(0.0, date.seconds);
}

void test_ln_get_date_from_sys()
{
  time_t now_time;
  time(&now_time);

  struct ln_date now_date;
  ln_get_date_from_sys(&now_date);

  struct tm *utcdate = gmtime(&now_time);
  TEST_ASSERT_EQUAL(utcdate->tm_year + 1900, now_date.years);
  TEST_ASSERT_EQUAL(utcdate->tm_mon + 1, now_date.months);
  TEST_ASSERT_EQUAL(utcdate->tm_mday, now_date.days);
  TEST_ASSERT_EQUAL(utcdate->tm_hour, now_date.hours);
  TEST_ASSERT_EQUAL(utcdate->tm_min, now_date.minutes);
  TEST_ASSERT_DOUBLE_WITHIN(1.0, utcdate->tm_sec, now_date.seconds);
}

void test_ln_get_date_from_timet()
{
  time_t now_time;
  time(&now_time);

  struct ln_date now_date;
  ln_get_date_from_timet (&now_time, &now_date);

  struct tm *utcdate = gmtime(&now_time);
  TEST_ASSERT_EQUAL(utcdate->tm_year + 1900, now_date.years);
  TEST_ASSERT_EQUAL(utcdate->tm_mon + 1, now_date.months);
  TEST_ASSERT_EQUAL(utcdate->tm_mday, now_date.days);
  TEST_ASSERT_EQUAL(utcdate->tm_hour, now_date.hours);
  TEST_ASSERT_EQUAL(utcdate->tm_min, now_date.minutes);
  TEST_ASSERT_EQUAL_DOUBLE(utcdate->tm_sec, now_date.seconds);
}

void test_ln_get_date_from_tm()
{
  time_t now_time;
  time(&now_time);
  struct tm *utcdate = gmtime(&now_time);

  struct ln_date now_date;
  ln_get_date_from_tm(utcdate, &now_date);

  TEST_ASSERT_EQUAL(utcdate->tm_year + 1900, now_date.years);
  TEST_ASSERT_EQUAL(utcdate->tm_mon + 1, now_date.months);
  TEST_ASSERT_EQUAL(utcdate->tm_mday, now_date.days);
  TEST_ASSERT_EQUAL(utcdate->tm_hour, now_date.hours);
  TEST_ASSERT_EQUAL(utcdate->tm_min, now_date.minutes);
  TEST_ASSERT_EQUAL_DOUBLE(utcdate->tm_sec, now_date.seconds);
}

void test_ln_get_day_of_week()
{
  /* Get julian day for 30/06/1954 00:00:00 */
  struct ln_date date = {
    .years   = 1954,
    .months  =    6,
    .days    =   30,
    .hours   =    0,
    .minutes =    0,
    .seconds =    0
  };
  int wday = ln_get_day_of_week(&date);

  TEST_ASSERT_EQUAL_INT(3, wday);
}

void test_ln_get_julian_day()
{
  /* Get julian day for 04/10/1957 19:00:00 */
  struct ln_date date = {
    .years   = 1957,
    .months  =   10,
    .days    =    4,
    .hours   =   19,
    .minutes =    0,
    .seconds =    0
  };
  double JD = ln_get_julian_day(&date);

  TEST_ASSERT_EQUAL_DOUBLE(2436116.291667, JD);

  /* Get julian day for 30/06/1954 00:00:00 */
  date.years   = 1954;
  date.months  =    6;
  date.days    =   30;
  date.hours   =    0;
  date.minutes =    0;
  date.seconds =    0;
  JD = ln_get_julian_day(&date);

  TEST_ASSERT_EQUAL_DOUBLE(2434923.5, JD);

  /* Get julian day for 27/01/333 12:00:00 */
  date.years = 333;
  date.months = 1;
  date.days = 27;
  date.hours = 12;
  date.minutes =    0;
  date.seconds =    0;
  JD = ln_get_julian_day(&date);

  TEST_ASSERT_EQUAL_DOUBLE(1842713.0, JD);

  /* Get julian day for 30/06/1954 00:00:00 */
  date.years = 1954;
  date.months = 6;
  date.days = 30;
  date.hours = 0;
  date.minutes =    0;
  date.seconds =    0;
  JD = ln_get_julian_day(&date);

  TEST_ASSERT_EQUAL_DOUBLE(2434923.5, JD);
}

void test_ln_get_julian_from_mpc()
{
  double JD = ln_get_julian_from_mpc("J969U");

  TEST_ASSERT_EQUAL_DOUBLE(2450356.5, JD);
}

void test_ln_get_julian_from_sys()
{
  double JD1 = ln_get_julian_from_sys();

  thrd_sleep(&(struct timespec){.tv_sec = 1, .tv_nsec = 500000000}, NULL);

  double JD2 = ln_get_julian_from_sys();

  TEST_ASSERT_DOUBLE_WITHIN(0.1 / 86400.0, 1.5 /86400.0, JD2 - JD1);
}

void test_ln_get_julian_from_timet()
{
  time_t utc_time = mktime(
    &(struct tm) {
      .tm_year = 87, .tm_mon =  3, .tm_mday = 10,
      .tm_hour =  1, .tm_min =  0, .tm_sec  =  0,
      .tm_isdst = 0
    }
  );

  double JD = ln_get_julian_from_timet(&utc_time);

  TEST_ASSERT_DOUBLE_WITHIN(0.001 / 86400.0, 2446895.5, JD);
}

void test_ln_get_julian_local_date()
{
  double JD = ln_get_julian_local_date(
    &(struct ln_zonedate) {
      .years  = 1987, .months  =  4, .days    = 10,
      .hours  =    1, .minutes =  0, .seconds =  0,
      .gmtoff = 3600
    }
  );

  TEST_ASSERT_DOUBLE_WITHIN(0.001 / 86400.0, 2446895.5, JD);
}

void test_ln_get_local_date()
{
  struct ln_zonedate localdate;
  ln_get_local_date(2459410.5, &localdate);

  TEST_ASSERT_EQUAL(2021, localdate.years);
  TEST_ASSERT_EQUAL(   7, localdate.months);
  TEST_ASSERT_EQUAL(  15, localdate.days);
  TEST_ASSERT_EQUAL(   2, localdate.hours);
  TEST_ASSERT_EQUAL(   0, localdate.minutes);
  TEST_ASSERT_DOUBLE_WITHIN(1e-4, 0.0, localdate.seconds);
  TEST_ASSERT_EQUAL(7200, localdate.gmtoff);
}

void test_ln_get_timet_from_julian()
{
  double JD = 2446895.5;  /* 1987/4/10 00:00:00.0 UT */

  time_t timepoint;
  ln_get_timet_from_julian(JD, &timepoint);

  struct tm *utcdate = gmtime(&timepoint);
  TEST_ASSERT_EQUAL(87, utcdate->tm_year);
  TEST_ASSERT_EQUAL( 3, utcdate->tm_mon);
  TEST_ASSERT_EQUAL(10, utcdate->tm_mday);
  TEST_ASSERT_EQUAL( 0, utcdate->tm_hour);
  TEST_ASSERT_EQUAL( 0, utcdate->tm_min);
  TEST_ASSERT_EQUAL( 0, utcdate->tm_sec);
}

void test_ln_zonedate_to_date()
{
  struct ln_zonedate zonedate = {
    .years   = 1954,
    .months  =    6,
    .days    =   30,
    .hours   =    2,
    .minutes =    0,
    .seconds =    0,
    .gmtoff  = 7200
  };

  struct ln_date date;
  ln_zonedate_to_date(&zonedate, &date);

  TEST_ASSERT_EQUAL(1954, date.years);
  TEST_ASSERT_EQUAL(   6, date.months);
  TEST_ASSERT_EQUAL(  30, date.days);
  TEST_ASSERT_EQUAL(   0, date.hours);
  TEST_ASSERT_EQUAL(   0, date.minutes);
  TEST_ASSERT_EQUAL_DOUBLE(0.0, date.seconds);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_julian_day);
  RUN_TEST(test_ln_get_date);
  RUN_TEST(test_ln_get_date_from_timet);
  RUN_TEST(test_ln_get_date_from_tm);
  RUN_TEST(test_ln_get_local_date);
  RUN_TEST(test_ln_get_day_of_week);
  RUN_TEST(test_ln_get_julian_from_sys);
  RUN_TEST(test_ln_get_date_from_sys);
  RUN_TEST(test_ln_get_julian_from_timet);
  RUN_TEST(test_ln_get_timet_from_julian);
  RUN_TEST(test_ln_get_julian_local_date);
  RUN_TEST(test_ln_get_date_from_mpc);
  RUN_TEST(test_ln_get_julian_from_mpc);
  RUN_TEST(test_ln_date_to_zonedate);
  RUN_TEST(test_ln_zonedate_to_date);

  return UNITY_END();
}
