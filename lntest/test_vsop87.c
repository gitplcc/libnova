/*
 * test_vsop87.c
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

#define ARCSECTENTH (0.1 / 3600.0)

double JD;

void setUp()
{
  JD = 2451545.0; /* 2000-01-01 12:00:00 TDB */
}

void tearDown()
{
}

/* Sun */

void test_ln_get_solar_equ_coords(void)
{
  struct ln_equ_posn equ;
  ln_get_solar_equ_coords(JD, &equ);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH, 281.278554167, equ.ra,  "(Solar Position) RA on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH, -23.034641667, equ.dec, "(Solar Position) DEC on JD 2451545.0"
  );
}

/* Mercury */

void test_ln_get_mercury_helio_coords(void)
{
  struct ln_helio_posn pos;
  ln_get_mercury_helio_coords(JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH, 253.782952371, pos.L,  "(Mercury Position) L on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,  -3.022772983, pos.B, "(Mercury Position) B on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-6, 0.4664714751, pos.R, "(Venus Position) R (AU) on JD 2451545.0"
  );
}

void test_ln_get_mercury_equ_coords(void)
{
  TEST_IGNORE();
  struct ln_equ_posn equ;
  ln_get_mercury_equ_coords(JD, &equ);
  TEST_PRINTF("Mercury RA %f Dec %f", equ.ra, equ.dec);
}

void test_ln_get_mercury_earth_dist(void)
{
  double au = ln_get_mercury_earth_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 1.415451264, au, "Mercury -> Earth dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_mercury_solar_dist(void)
{
  double au = ln_get_mercury_solar_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 0.466468825, au, "Mercury -> Sun dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_mercury_disk(void)
{
  double ilum_dsk = ln_get_mercury_disk(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-3, 0.973, ilum_dsk, "Mercury -> illuminated disk on JD 2451545.0"
  );
}

void test_ln_get_mercury_magnitude(void)
{
  double mag = ln_get_mercury_magnitude(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, -0.7, mag, "Mercury -> magnitude on JD 2451545.0"
  );
}

void test_ln_get_mercury_phase(void)
{
  double phase = ln_get_mercury_phase(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 19., phase, "Mercury -> phase on JD 2451545.0"
  );
}

void test_ln_get_mercury_sdiam(void)
{
  double sdiam = ln_get_mercury_sdiam(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 2.35, sdiam, "Mercury -> sdiam on JD 2451545.0"
  );
}

/* Venus */

void test_ln_get_venus_helio_coords(void)
{
  struct ln_helio_posn pos;
  ln_get_venus_helio_coords(JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH, 182.602920759, pos.L,  "(Venus Position) L on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,   3.264615249, pos.B, "(Venus Position) B on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-6, 0.720212925, pos.R, "(Venus Position) R (AU) on JD 2451545.0"
  );
}

void test_ln_get_venus_equ_coords(void)
{
  TEST_IGNORE();
  struct ln_equ_posn equ;
  ln_get_venus_equ_coords(JD, &equ);
  TEST_PRINTF("Venus RA %f Dec %f", equ.ra, equ.dec);
}

void test_ln_get_venus_earth_dist(void)
{
  double au = ln_get_venus_earth_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 1.137563157, au, "Venus -> Earth dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_venus_solar_dist(void)
{
  double au = ln_get_venus_solar_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 0.720212307, au, "Venus -> Sun distance (AU) on JD 2451545.0"
  );
}

void test_ln_get_venus_disk(void)
{
  double ilum_dsk = ln_get_venus_disk(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-3, 0.760, ilum_dsk, "Venus -> illuminated disk on JD 2451545.0"
  );
}

void test_ln_get_venus_magnitude(void)
{
  double mag = ln_get_venus_magnitude(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, -4.1, mag, "Venus -> magnitude on JD 2451545.0"
  );
}

void test_ln_get_venus_phase(void)
{
  double phase = ln_get_venus_phase(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 59., phase, "Venus -> phase on JD 2451545.0"
  );
}

void test_ln_get_venus_sdiam(void)
{
  double sdiam = ln_get_venus_sdiam(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 7.4, sdiam, "Venus -> sdiam on JD 2451545.0"
  );
}

/* Earth */

void test_ln_get_earth_helio_coords(void)
{
  struct ln_helio_posn pos;
  ln_get_earth_helio_coords(JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH, 100.377843418, pos.L,  "(Earth Position) L on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,  -0.000227212, pos.B, "(Earth Position) B on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-6, 0.983327682, pos.R, "(Earth Position) R (AU) on JD 2451545.0"
  );
}

void test_ln_get_earth_solar_dist(void)
{
  double au = ln_get_earth_solar_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-6, 0.983327682, au, "(Earth -> Sun dist (AU) on JD 2451545.0"
  );
}

/* Mars */

void test_ln_get_mars_helio_coords(void)
{
  struct ln_helio_posn pos;
  ln_get_mars_helio_coords(JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH, 359.447306577, pos.L,  "(Mars Position) L on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,  -1.419673816, pos.B, "(Mars Position) B on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-6, 1.391207694, pos.R, "(Mars Position) R (AU) on JD 2451545.0"
  );
}

void test_ln_get_mars_equ_coords(void)
{
  TEST_IGNORE();
  struct ln_equ_posn equ;
  ln_get_mars_equ_coords(JD, &equ);
  TEST_PRINTF("Mars RA %f Dec %f", equ.ra, equ.dec);
}

void test_ln_get_mars_earth_dist(void)
{
  double au = ln_get_mars_earth_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 1.849675427, au, "Mars -> Earth dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_mars_solar_dist(void)
{
  double au = ln_get_mars_solar_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 1.391202531, au, "Mars -> Sun distance (AU) on JD 2451545.0"
  );
}

void test_ln_get_mars_disk(void)
{
  double ilum_dsk = ln_get_mars_disk(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-3, 0.927, ilum_dsk, "Mars -> illuminated disk on JD 2451545.0"
  );
}

void test_ln_get_mars_magnitude(void)
{
  double mag = ln_get_mars_magnitude(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 1.0, mag, "Mars -> magnitude on JD 2451545.0"
  );
}

void test_ln_get_mars_phase(void)
{
  double phase = ln_get_mars_phase(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 31., phase, "Mars -> phase on JD 2451545.0"
  );
}

void test_ln_get_mars_sdiam(void)
{
  double sdiam = ln_get_mars_sdiam(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 2.55, sdiam, "Mars -> sdiam on JD 2451545.0"
  );
}

/* Jupiter */

void test_ln_get_jupiter_helio_coords(void)
{
  struct ln_helio_posn pos;
  ln_get_jupiter_helio_coords(JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,  36.294665948, pos.L,  "(Jupiter Position) L on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,  -1.174569433, pos.B, "(Jupiter Position) B on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-6, 4.965381280, pos.R, "(Jupiter Position) R (AU) on JD 2451545.0"
  );
}

void test_ln_get_jupiter_equ_coords(void)
{
  TEST_IGNORE();
  struct ln_equ_posn equ;
  ln_get_jupiter_equ_coords(JD, &equ);
  TEST_PRINTF("Jupiter RA %f Dec %f", equ.ra, equ.dec);
}

void test_ln_get_jupiter_earth_dist(void)
{
  double au = ln_get_jupiter_earth_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 4.621180500, au, "Jupiter -> Earth dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_jupiter_solar_dist(void)
{
  double au = ln_get_jupiter_solar_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 4.965377614, au, "Jupiter -> Sun distance (AU) on JD 2451545.0"
  );
}

void test_ln_get_jupiter_disk(void)
{
  double ilum_dsk = ln_get_jupiter_disk(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-3, 0.991, ilum_dsk, "Jupiter -> illuminated disk on JD 2451545.0"
  );
}

void test_ln_get_jupiter_magnitude(void)
{
  double mag = ln_get_jupiter_magnitude(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, -2.5, mag, "Jupiter -> magnitude on JD 2451545.0"
  );
}

void test_ln_get_jupiter_phase(void)
{
  double phase = ln_get_jupiter_phase(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 11., phase, "Jupiter -> phase on JD 2451545.0"
  );
}

void test_ln_get_jupiter_pol_sdiam(void)
{
  double sdiam = ln_get_jupiter_pol_sdiam(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 21.3, sdiam, "Jupiter -> pol sdiam on JD 2451545.0"
  );
}

void test_ln_get_jupiter_equ_sdiam(void)
{
  double sdiam = ln_get_jupiter_equ_sdiam(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 21.3, sdiam, "Jupiter -> equ sdiam on JD 2451545.0"
  );
}

/* Saturn */

void test_ln_get_saturn_helio_coords(void)
{
  struct ln_helio_posn pos;
  ln_get_saturn_helio_coords(JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,  45.722254743, pos.L,  "(Saturn Position) L on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,  -2.303199517, pos.B, "(Saturn Position) B on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-6, 9.183848288, pos.R, "(Saturn Position) R (AU) on JD 2451545.0"
  );
}

void test_ln_get_saturn_equ_coords(void)
{
  TEST_IGNORE();
  struct ln_equ_posn equ;
  ln_get_saturn_equ_coords(JD, &equ);
  TEST_PRINTF("Saturn RA %f Dec %f", equ.ra, equ.dec);
}

void test_ln_get_saturn_earth_dist(void)
{
  double au = ln_get_saturn_earth_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 8.652807209, au, "Saturn -> Earth dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_saturn_solar_dist(void)
{
  double au = ln_get_saturn_solar_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 9.183858131, au, "Saturn -> Sun distance (AU) on JD 2451545.0"
  );
}

void test_ln_get_saturn_disk(void)
{
  double ilum_dsk = ln_get_saturn_disk(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-3, 0.998, ilum_dsk, "Saturn -> illuminated disk on JD 2451545.0"
  );
}

void test_ln_get_saturn_magnitude(void)
{
  double mag = ln_get_saturn_magnitude(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 0.1, mag, "Saturn -> magnitude on JD 2451545.0"
  );
}

void test_ln_get_saturn_phase(void)
{
  double phase = ln_get_saturn_phase(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 5., phase, "Saturn -> phase on JD 2451545.0"
  );
}

void test_ln_get_saturn_pol_sdiam(void)
{
  double sdiam = ln_get_saturn_pol_sdiam(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 9.65, sdiam, "Saturn -> pol sdiam on JD 2451545.0"
  );
}

void test_ln_get_saturn_equ_sdiam(void)
{
  double sdiam = ln_get_saturn_equ_sdiam(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 9.65, sdiam, "Saturn -> equ sdiam on JD 2451545.0"
  );
}

/* Uranus */

void test_ln_get_uranus_helio_coords(void)
{
  struct ln_helio_posn pos;
  ln_get_uranus_helio_coords(JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH, 316.418722908, pos.L,  "(Uranus Position) L on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,  -0.684844294, pos.B, "(Uranus Position) B on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-6, 19.924047895, pos.R, "(Uranus Position) R (AU) on JD 2451545.0"
  );
}

void test_ln_get_uranus_equ_coords(void)
{
  TEST_IGNORE();
  struct ln_equ_posn equ;
  ln_get_uranus_equ_coords(JD, &equ);
  TEST_PRINTF("Uranus RA %f Dec %f", equ.ra, equ.dec);
}

void test_ln_get_uranus_earth_dist(void)
{
  double au = ln_get_uranus_earth_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 20.727155177, au, "Uranus -> Earth dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_uranus_solar_dist(void)
{
  double au = ln_get_uranus_solar_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 19.924013639, au, "Uranus -> Sun dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_uranus_disk(void)
{
  double ilum_dsk = ln_get_uranus_disk(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-3, 1.000, ilum_dsk, "Uranus -> illuminated disk on JD 2451545.0"
  );
}

void test_ln_get_uranus_magnitude(void)
{
  double mag = ln_get_uranus_magnitude(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 5.9, mag, "Uranus -> magnitude on JD 2451545.0"
  );
}

void test_ln_get_uranus_phase(void)
{
  double phase = ln_get_uranus_phase(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 2., phase, "Uranus -> phase on JD 2451545.0"
  );
}

void test_ln_get_uranus_sdiam(void)
{
  double sdiam = ln_get_uranus_sdiam(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 1.65, sdiam, "Uranus -> sdiam on JD 2451545.0"
  );
}

/* Neptune */

void test_ln_get_neptune_helio_coords(void)
{
  struct ln_helio_posn pos;
  ln_get_neptune_helio_coords(JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH, 303.929067959, pos.L,  "(Neptune Position) L on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,   0.241998981, pos.B, "(Neptune Position) B on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-6, 30.120532933, pos.R, "(Neptune Position) R (AU) on JD 2451545.0"
  );
}

void test_ln_get_neptune_equ_coords(void)
{
  TEST_IGNORE();
  struct ln_equ_posn equ;
  ln_get_neptune_equ_coords(JD, &equ);
  TEST_PRINTF("Neptune RA %f Dec %f", equ.ra, equ.dec);
}

void test_ln_get_neptune_earth_dist(void)
{
  double au = ln_get_neptune_earth_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 31.024479681, au, "Neptune -> Earth dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_neptune_solar_dist(void)
{
  double au = ln_get_neptune_solar_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 30.120585961, au, "Neptune -> Sun dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_neptune_disk(void)
{
  double ilum_dsk = ln_get_neptune_disk(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-3, 1.000, ilum_dsk, "Neptune -> illuminated disk on JD 2451545.0"
  );
}

void test_ln_get_neptune_magnitude(void)
{
  double mag = ln_get_neptune_magnitude(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 8.0, mag, "Neptune -> magnitude on JD 2451545.0"
  );
}

void test_ln_get_neptune_phase(void)
{
  double phase = ln_get_neptune_phase(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 1., phase, "Neptune -> phase on JD 2451545.0"
  );
}

void test_ln_get_neptune_sdiam(void)
{
  double sdiam = ln_get_neptune_sdiam(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 1.2, sdiam, "Neptune -> sdiam on JD 2451545.0"
  );
}

/* Pluto */

void test_ln_get_pluto_helio_coords(void)
{
  struct ln_helio_posn pos;
  ln_get_pluto_helio_coords(JD, &pos);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH, 251.454722222, pos.L,  "(Pluto Position) L on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    ARCSECTENTH,  10.855277777, pos.B, "(Pluto Position) B on JD 2451545.0"
  );
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-6, 30.223193432, pos.R, "(Pluto Position) R (AU) on JD 2451545.0"
  );
}

void test_ln_get_pluto_equ_coords(void)
{
  TEST_IGNORE();
  struct ln_equ_posn equ;
  ln_get_pluto_equ_coords(JD, &equ);
  TEST_PRINTF("Pluto RA %f Dec %f", equ.ra, equ.dec);
}

void test_ln_get_pluto_earth_dist(void)
{
  double au = ln_get_pluto_earth_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 31.064337555, au, "Pluto -> Earth dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_pluto_solar_dist(void)
{
  double au = ln_get_pluto_solar_dist(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-5, 30.223193432, au, "Pluto -> Sun dist (AU) on JD 2451545.0"
  );
}

void test_ln_get_pluto_disk(void)
{
  double ilum_dsk = ln_get_pluto_disk(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-3, 1.000, ilum_dsk, "Pluto -> illuminated disk on JD 2451545.0"
  );
}

void test_ln_get_pluto_magnitude(void)
{
  double mag = ln_get_pluto_magnitude(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 13.9, mag, "Pluto -> magnitude on JD 2451545.0"
  );
}

void test_ln_get_pluto_phase(void)
{
  double phase = ln_get_pluto_phase(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 1., phase, "Pluto -> phase on JD 2451545.0"
  );
}

void test_ln_get_pluto_sdiam(void)
{
  double sdiam = ln_get_pluto_sdiam(JD);
  TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
    1e-1, 0.05, sdiam, "Pluto -> sdiam on JD 2451545.0"
  );
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  /* Sun */
  RUN_TEST(test_ln_get_solar_equ_coords);

  /* Mercury */
  RUN_TEST(test_ln_get_mercury_helio_coords);
  RUN_TEST(test_ln_get_mercury_equ_coords);
  RUN_TEST(test_ln_get_mercury_earth_dist);
  RUN_TEST(test_ln_get_mercury_solar_dist);
  RUN_TEST(test_ln_get_mercury_disk);
  RUN_TEST(test_ln_get_mercury_magnitude);
  RUN_TEST(test_ln_get_mercury_phase);
  RUN_TEST(test_ln_get_mercury_sdiam);

  /* Venus */
  RUN_TEST(test_ln_get_venus_helio_coords);
  RUN_TEST(test_ln_get_venus_equ_coords);
  RUN_TEST(test_ln_get_venus_earth_dist);
  RUN_TEST(test_ln_get_venus_solar_dist);
  RUN_TEST(test_ln_get_venus_disk);
  RUN_TEST(test_ln_get_venus_magnitude);
  RUN_TEST(test_ln_get_venus_phase);
  RUN_TEST(test_ln_get_venus_sdiam);

  /* Earth */
  RUN_TEST(test_ln_get_earth_helio_coords);
  RUN_TEST(test_ln_get_earth_solar_dist);

  /* Mars */
  RUN_TEST(test_ln_get_mars_helio_coords);
  RUN_TEST(test_ln_get_mars_equ_coords);
  RUN_TEST(test_ln_get_mars_earth_dist);
  RUN_TEST(test_ln_get_mars_solar_dist);
  RUN_TEST(test_ln_get_mars_disk);
  RUN_TEST(test_ln_get_mars_magnitude);
  RUN_TEST(test_ln_get_mars_phase);
  RUN_TEST(test_ln_get_mars_sdiam);

  /* Jupiter */
  RUN_TEST(test_ln_get_jupiter_helio_coords);
  RUN_TEST(test_ln_get_jupiter_equ_coords);
  RUN_TEST(test_ln_get_jupiter_earth_dist);
  RUN_TEST(test_ln_get_jupiter_solar_dist);
  RUN_TEST(test_ln_get_jupiter_disk);
  RUN_TEST(test_ln_get_jupiter_magnitude);
  RUN_TEST(test_ln_get_jupiter_phase);
  RUN_TEST(test_ln_get_jupiter_pol_sdiam);
  RUN_TEST(test_ln_get_jupiter_equ_sdiam);

  /* Saturn */
  RUN_TEST(test_ln_get_saturn_helio_coords);
  RUN_TEST(test_ln_get_saturn_equ_coords);
  RUN_TEST(test_ln_get_saturn_earth_dist);
  RUN_TEST(test_ln_get_saturn_solar_dist);
  RUN_TEST(test_ln_get_saturn_disk);
  RUN_TEST(test_ln_get_saturn_magnitude);
  RUN_TEST(test_ln_get_saturn_phase);
  RUN_TEST(test_ln_get_saturn_pol_sdiam);
  RUN_TEST(test_ln_get_saturn_equ_sdiam);

  /* Uranus */
  RUN_TEST(test_ln_get_uranus_helio_coords);
  RUN_TEST(test_ln_get_uranus_equ_coords);
  RUN_TEST(test_ln_get_uranus_earth_dist);
  RUN_TEST(test_ln_get_uranus_solar_dist);
  RUN_TEST(test_ln_get_uranus_disk);
  RUN_TEST(test_ln_get_uranus_magnitude);
  RUN_TEST(test_ln_get_uranus_phase);
  RUN_TEST(test_ln_get_uranus_sdiam);

  /* Neptune */
  RUN_TEST(test_ln_get_neptune_helio_coords);
  RUN_TEST(test_ln_get_neptune_equ_coords);
  RUN_TEST(test_ln_get_neptune_earth_dist);
  RUN_TEST(test_ln_get_neptune_solar_dist);
  RUN_TEST(test_ln_get_neptune_disk);
  RUN_TEST(test_ln_get_neptune_magnitude);
  RUN_TEST(test_ln_get_neptune_phase);
  RUN_TEST(test_ln_get_neptune_sdiam);

  /* Pluto */
  RUN_TEST(test_ln_get_pluto_helio_coords);
  RUN_TEST(test_ln_get_pluto_equ_coords);
  RUN_TEST(test_ln_get_pluto_earth_dist);
  RUN_TEST(test_ln_get_pluto_solar_dist);
  RUN_TEST(test_ln_get_pluto_disk);
  RUN_TEST(test_ln_get_pluto_magnitude);
  RUN_TEST(test_ln_get_pluto_phase);
  RUN_TEST(test_ln_get_pluto_sdiam);

  return UNITY_END();
}
