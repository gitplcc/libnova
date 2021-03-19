/*
 * test_ticket9.c
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

struct ln_lnlat_posn observer;
struct ln_zonedate zonedate;
double JD;

void setUp()
{
  observer.lng = -74.730571;
  observer.lat = +39.275787;

  zonedate.years   =   2009;
  zonedate.months  =      8;
  zonedate.days    =     10;
  zonedate.hours   =      0;
  zonedate.minutes =      0;
  zonedate.seconds =      0.0;
  zonedate.gmtoff  = -14400;

  JD = ln_get_julian_local_date(&zonedate);
}

void tearDown()
{
}

static void test_ticket9()
{
  struct ln_rst_time rst;
  int ret = ln_get_solar_rst(JD, &observer, &rst);

  struct ln_date date;
  ln_get_date(rst.set, &date);
  ln_date_to_zonedate(&date, &zonedate, zonedate.gmtoff);
  TEST_ASSERT_EQUAL(10, zonedate.days);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_ticket9);
  return UNITY_END();
}
