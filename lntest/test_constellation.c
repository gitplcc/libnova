/*
 * test_constellation.c
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

void test_ln_get_constellation(void)
{
  const struct {
    double ra;
    double dec;
    const char *constel;
  } test_c[] = {
    { 127.1810,  14.2177, "Cnc" },
    { 139.0884,  11.5273, "Leo" },
    { 139.0884,  11.5273, "Leo" },
    { 150.7003,   8.3804, "Leo" },
    { 162.0989,   4.9088, "Leo" },
    { 173.3865,   1.2455, "Vir" },
    { 184.6787,  -2.4756, "Vir" },
    { 219.8070, -12.6071, "Lib" },
    { 232.3073, -15.1551, "Lib" },
    { 245.3311, -17.0311, "Oph" },
    { 258.8877, -18.0809, "Oph" },
    { 272.9237, -18.1687, "Sgr" },
    { 287.3266, -17.2004, "Sgr" },
    { 301.9481, -15.1508, "Cap" },
    { 316.6420, -12.0870, "Aqr" },
    { 331.3001,  -8.1780, "Aqr" },
    { 345.8704,  -3.6867, "Psc" },
    {   0.3515,   1.0575, "Psc" },
    {  14.7686,   5.6999, "Psc" },
    {  29.1425,   9.9069, "Psc" },
    {  43.4634,  13.4057, "Ari" },
    {  57.6785,  16.0084, "Tau" },
    {  71.6973,  17.6196, "Tau" },
    {  85.4141,  18.2277, "Tau" },
    {  98.7371,  17.8859, "Gem" },
    { 111.6113,  16.6902, "Gem" },
    { 124.0301,  14.7589, "Cnc" },
    { 136.0329,  12.2183, "Cnc" },
    { 147.6959,   9.1950, "Leo" },
    {   0.0000,   0.0000, NULL }
  };

  for (int i = 0; ; i++) {
    if (!test_c[i].constel)
      break;

    struct ln_equ_posn equ = {.ra = test_c[i].ra, .dec = test_c[i].dec};
    char msg[200];
    sprintf(msg, "Constellation at %.04f %+.04f", equ.ra, equ.dec);
    const char *constel = constel = ln_get_constellation(&equ);

    TEST_ASSERT_EQUAL_STRING_MESSAGE(test_c[i].constel, constel, msg);
  }
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_ln_get_constellation);

  return UNITY_END();
}
