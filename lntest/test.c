/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

Copyright 2000 Liam Girdwood
Copyright 2008-2009 Petr Kubanek*/

#include "config.h"

#include <libnova/libnova.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if !HAVE_NANOSLEEP && !HAVE_USLEEP && HAVE_SLEEP
#include <Windows.h>
#endif

void test_milisleep(unsigned long ms)
{
#if HAVE_NANOSLEEP
    struct timespec delay;
    if (ms < 1000) {
        delay.tv_sec = 0;
        delay.tv_nsec = 1000000 * ms;
    }
    else {
        ldiv_t rslt = ldiv(ms, 1000);
        delay.tv_sec = rslt.quot;
        delay.tv_nsec = 1000000 * rslt.rem;
    }
    nanosleep(&delay, NULL);
#elif HAVE_USLEEP
    usleep(1000 * ms);
#elif HAVE_SLEEP
    Sleep(ms);
#else
#error Unsupported compiler.
#endif
}

/*
 * Define DATE or SYS_DATE for testing VSOP87 theory with other JD
 */
//#define DATE
//#define SYS_DATE

// holds number of tests
static int test_number = 0;

#if defined(_POSIX_TIMERS)
#if defined(_POSIX_CPUTIME)
#define TEST_CLOCKID (CLOCK_PROCESS_CPUTIME_ID)
#else
#define TEST_CLOCKID (CLOCK_REALTIME)
#endif
static struct timespec start, end;

static void start_timer(void)
{
    clock_gettime(TEST_CLOCKID, &start);
}

static void end_timer(void)
{
    double msecs;

    clock_gettime(TEST_CLOCKID, &end);
    msecs = 1.0e3 * (end.tv_sec - start.tv_sec)
            + (end.tv_nsec - start.tv_nsec) / 1.0e6;
    fprintf(stdout, "   Time %3.1f msecs\n", msecs);
}
#else
static clock_t start, end;

static void start_timer(void)
{
    start = clock();
}

static void end_timer(void)
{
    double msecs;

    end = clock();
    msecs = (1000.0 * (end - start)) / CLOCKS_PER_SEC;

    fprintf(stdout, "   Time %3.1f msecs\n", msecs);
}
#endif

static double compare_results(double calc, double expect, double tolerance)
{
        if (calc - expect > tolerance || calc - expect < (tolerance * -1.0))
                return calc - expect;
        else
                return 0;
}

static int test_result(char *test, double calc, double expect, double tolerance)
{
        double diff;

        fprintf(stdout, "TEST %s....", test);

        test_number++;

        diff = compare_results(calc, expect, tolerance);
        if (diff) {
                fprintf(stdout, "[FAILED]\n");
                fprintf(stdout,
                        "       Expected %8.8f but calculated %8.8f %0.12f error.\n\n",
                        expect, calc, diff);
                return 1;
        } else {
                fprintf(stdout, "[PASSED]\n");
                fprintf(stdout, "       Expected and calculated %8.8f.\n\n", calc);
                return 0;
        }
}

static int test_str_result(char *test, const char *calc, const char *expect)
{
        int diff;

        fprintf(stdout, "TEST %s....", test);

        test_number++;

        diff = strcmp(calc, expect);
        if (diff) {
                fprintf(stdout, "[FAILED]\n");
                fprintf(stdout,
                        "       Expected %s but calculated %s.\n\n",
                        expect, calc);
                return 1;
        } else {
                fprintf(stdout, "[PASSED]\n");
                fprintf(stdout, "       Expected and calculated %s.\n\n", calc);
                return 0;
        }
}

static int vsop87_test(void)
{
        struct ln_helio_posn pos;
        struct lnh_equ_posn hequ;
        struct ln_equ_posn equ;
        double JD = 2448976.5;
        double au;
        int failed = 0;

#ifdef DATE
        struct ln_date date;
        date.years = 2003;
        date.months = 1;
        date.days = 29;
        date.hours = 0;
        date.minutes = 0;
        date.seconds = 0;

        JD = ln_get_julian_day(&date);
#endif
#ifdef SYS_TIME
        JD = ln_get_julian_from_sys();
#endif

        ln_get_solar_equ_coords(JD, &equ);
        failed += test_result("(Solar Position) RA on JD 2448976.5  ",
                equ.ra, 268.32146893, 0.00000001);
        failed += test_result("(Solar Position) DEC on JD 2448976.5  ",
                equ.dec, -23.43026873, 0.00000001);

        ln_get_mercury_helio_coords(JD, &pos);
        printf("Mercury L %f B %f R %f\n", pos.L, pos.B, pos.R);
        ln_get_mercury_equ_coords(JD, &equ);
        ln_equ_to_hequ (&equ, &hequ);
        printf("Mercury RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
        au = ln_get_mercury_earth_dist(JD);
        fprintf(stdout, "mercury -> Earth dist (AU) %f\n",au);
        au = ln_get_mercury_solar_dist(JD);
        fprintf(stdout, "mercury -> Sun dist (AU) %f\n",au);
        au = ln_get_mercury_disk(JD);
        fprintf(stdout, "mercury -> illuminated disk %f\n",au);
        au = ln_get_mercury_magnitude(JD);
        fprintf(stdout, "mercury -> magnitude %f\n",au);
        au = ln_get_mercury_phase(JD);
        fprintf(stdout, "mercury -> phase %f\n",au);
        au = ln_get_mercury_sdiam(JD);
        fprintf(stdout, "mercury -> sdiam %f\n",au);

        ln_get_venus_helio_coords(JD, &pos);
        printf("Venus L %f B %f R %f\n", pos.L, pos.B, pos.R);
        ln_get_venus_equ_coords(JD, &equ);
        ln_equ_to_hequ (&equ, &hequ);
        printf("Venus RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
        au = ln_get_venus_earth_dist(JD);
        fprintf(stdout, "venus -> Earth dist (AU) %f\n",au);
        au = ln_get_venus_solar_dist(JD);
        fprintf(stdout, "venus -> Sun dist (AU) %f\n",au);
        au = ln_get_venus_disk(JD);
        fprintf(stdout, "venus -> illuminated disk %f\n",au);
        au = ln_get_venus_magnitude(JD);
        fprintf(stdout, "venus -> magnitude %f\n",au);
        au = ln_get_venus_phase(JD);
        fprintf(stdout, "venus -> phase %f\n",au);
        au = ln_get_venus_sdiam(JD);
        fprintf(stdout, "venus -> sdiam %f\n",au);

        ln_get_earth_helio_coords(JD, &pos);
        printf("Earth L %f B %f R %f\n", pos.L, pos.B, pos.R);
        au = ln_get_earth_solar_dist(JD);
        fprintf(stdout, "earth -> Sun dist (AU) %f\n",au);

        ln_get_mars_helio_coords(JD, &pos);
        printf("Mars L %f B %f R %f\n", pos.L, pos.B, pos.R);
        ln_get_mars_equ_coords(JD, &equ);
        ln_equ_to_hequ (&equ, &hequ);
        printf("Mars RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
        au = ln_get_mars_earth_dist(JD);
        fprintf(stdout, "mars -> Earth dist (AU) %f\n",au);
        au = ln_get_mars_solar_dist(JD);
        fprintf(stdout, "mars -> Sun dist (AU) %f\n",au);
        au = ln_get_mars_disk(JD);
        fprintf(stdout, "mars -> illuminated disk %f\n",au);
        au = ln_get_mars_magnitude(JD);
        fprintf(stdout, "mars -> magnitude %f\n",au);
        au = ln_get_mars_phase(JD);
        fprintf(stdout, "mars -> phase %f\n",au);
        au = ln_get_mars_sdiam(JD);
        fprintf(stdout, "mars -> sdiam %f\n",au);

        ln_get_jupiter_helio_coords(JD, &pos);
        printf("Jupiter L %f B %f R %f\n", pos.L, pos.B, pos.R);
        ln_get_jupiter_equ_coords(JD, &equ);
        ln_equ_to_hequ (&equ, &hequ);
        printf("Jupiter RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
        au = ln_get_jupiter_earth_dist(JD);
        fprintf(stdout, "jupiter -> Earth dist (AU) %f\n",au);
        au = ln_get_jupiter_solar_dist(JD);
        fprintf(stdout, "jupiter -> Sun dist (AU) %f\n",au);
        au = ln_get_jupiter_disk(JD);
        fprintf(stdout, "jupiter -> illuminated disk %f\n",au);
        au = ln_get_jupiter_magnitude(JD);
        fprintf(stdout, "jupiter -> magnitude %f\n",au);
        au = ln_get_jupiter_phase(JD);
        fprintf(stdout, "jupiter -> phase %f\n",au);
        au = ln_get_jupiter_pol_sdiam(JD);
        fprintf(stdout, "jupiter -> polar sdiam %f\n",au);
        au = ln_get_jupiter_equ_sdiam(JD);
        fprintf(stdout, "jupiter -> equ sdiam %f\n",au);

        ln_get_saturn_helio_coords(JD, &pos);
        printf("Saturn L %f B %f R %f\n", pos.L, pos.B, pos.R);
        ln_get_saturn_equ_coords(JD, &equ);
        ln_equ_to_hequ (&equ, &hequ);
        printf("Saturn RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
        au = ln_get_saturn_earth_dist(JD);
        fprintf(stdout, "saturn -> Earth dist (AU) %f\n",au);
        au = ln_get_saturn_solar_dist(JD);
        fprintf(stdout, "saturn -> Sun dist (AU) %f\n",au);
        au = ln_get_saturn_disk(JD);
        fprintf(stdout, "saturn -> illuminated disk %f\n",au);
        au = ln_get_saturn_magnitude(JD);
        fprintf(stdout, "saturn -> magnitude %f\n",au);
        au = ln_get_saturn_phase(JD);
        fprintf(stdout, "saturn -> phase %f\n",au);
        au = ln_get_saturn_pol_sdiam(JD);
        fprintf(stdout, "saturn -> polar sdiam %f\n",au);
        au = ln_get_saturn_equ_sdiam(JD);
        fprintf(stdout, "saturn -> equ sdiam %f\n",au);

        ln_get_uranus_helio_coords(JD, &pos);
        printf("Uranus L %f B %f R %f\n", pos.L, pos.B, pos.R);
        ln_get_uranus_equ_coords(JD, &equ);
        ln_equ_to_hequ (&equ, &hequ);
        printf("Uranus RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
        au = ln_get_uranus_earth_dist(JD);
        fprintf(stdout, "uranus -> Earth dist (AU) %f\n",au);
        au = ln_get_uranus_solar_dist(JD);
        fprintf(stdout, "uranus -> Sun dist (AU) %f\n",au);
        au = ln_get_uranus_disk(JD);
        fprintf(stdout, "uranus -> illuminated disk %f\n",au);
        au = ln_get_uranus_magnitude(JD);
        fprintf(stdout, "uranus -> magnitude %f\n",au);
        au = ln_get_uranus_phase(JD);
        fprintf(stdout, "uranus -> phase %f\n",au);
        au = ln_get_uranus_sdiam(JD);
        fprintf(stdout, "uranus -> sdiam %f\n",au);

        ln_get_neptune_helio_coords(JD, &pos);
        printf("Neptune L %f B %f R %f\n", pos.L, pos.B, pos.R);
        ln_get_neptune_equ_coords(JD, &equ);
        ln_equ_to_hequ (&equ, &hequ);
        printf("Neptune RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
        au = ln_get_neptune_earth_dist(JD);
        fprintf(stdout, "neptune -> Earth dist (AU) %f\n",au);
        au = ln_get_neptune_solar_dist(JD);
        fprintf(stdout, "neptune -> Sun dist (AU) %f\n",au);
        au = ln_get_neptune_disk(JD);
        fprintf(stdout, "neptune -> illuminated disk %f\n",au);
        au = ln_get_neptune_magnitude(JD);
        fprintf(stdout, "neptune -> magnitude %f\n",au);
        au = ln_get_neptune_phase(JD);
        fprintf(stdout, "neptune -> phase %f\n",au);
        au = ln_get_neptune_sdiam(JD);
        fprintf(stdout, "neptune -> sdiam %f\n",au);

        ln_get_pluto_helio_coords(JD, &pos);
        printf("Pluto L %f B %f R %f\n", pos.L, pos.B, pos.R);
        ln_get_pluto_equ_coords(JD, &equ);
        ln_equ_to_hequ (&equ, &hequ);
        printf("Pluto RA %d:%d:%f Dec %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);
        au = ln_get_pluto_earth_dist(JD);
        fprintf(stdout, "pluto -> Earth dist (AU) %f\n",au);
        au = ln_get_pluto_solar_dist(JD);
        fprintf(stdout, "pluto -> Sun dist (AU) %f\n",au);
        au = ln_get_pluto_disk(JD);
        fprintf(stdout, "pluto -> illuminated disk %f\n",au);
        au = ln_get_pluto_magnitude(JD);
        fprintf(stdout, "pluto -> magnitude %f\n",au);
        au = ln_get_pluto_phase(JD);
        fprintf(stdout, "pluto -> phase %f\n",au);
        au = ln_get_pluto_sdiam(JD);
        fprintf(stdout, "pluto -> sdiam %f\n",au);

        return failed;
}

static int ell_rst_test(void)
{
        struct ln_lnlat_posn observer;
        struct ln_ell_orbit orbit;
        struct ln_date date;
        struct ln_rst_time rst;
        struct ln_equ_posn pos;
        double JD;
        int ret;
        int failed = 0;

        /* Comment C/1996 B2 (Hyakutake) somewhere at Japan */

        observer.lng = 135.0;
        observer.lat = 35.0;

        date.years = 1996;
        date.months = 5;
        date.days = 1;

        date.hours = 0;
        date.minutes = 0;
        date.seconds = 0.0;

        orbit.JD = ln_get_julian_day(&date);
        orbit.JD += 0.39481;
        orbit.a = 1014.2022026431;
        orbit.e = 0.9997730;
        orbit.i = 124.92379;
        orbit.omega = 188.04546;
        orbit.w = 130.17654;
        orbit.n = 0.0;

        date.years = 1996;
        date.months = 3;
        date.days = 24;

        date.hours = date.minutes = 0;
        date.seconds = 0.0;

        JD = ln_get_julian_day(&date);

        ln_get_ell_body_equ_coords(JD, &orbit, &pos);
        failed += test_result("(RA) for Hyakutake 1996/03/28 00:00",
                pos.ra, 220.8554, 0.001);
        failed += test_result("(Dec) for Hyakutake 1996/03/28 00:00",
                pos.dec, 36.5341, 0.001);

        date.days = 28;

        date.hours = 10;
        date.minutes = 42;

        JD = ln_get_julian_day(&date);

        ln_get_ell_body_equ_coords(JD, &orbit, &pos);
        failed += test_result("(RA) for Hyakutake 1996/03/28 10:42",
                pos.ra, 56.2140, 0.001);
        failed += test_result("(Dec) for Hyakutake 1996/03/28 10:42",
                pos.dec, 74.4302, 0.001);

        date.days = 23;

        date.hours = 17;
        date.minutes = 38;
        date.seconds = 0.0;

        JD = ln_get_julian_day (&date);

        ln_get_ell_body_equ_coords(JD, &orbit, &pos);
        failed += test_result("(RA) for Hyakutake 1996/03/23 17:38",
                pos.ra,  221.2153, 0.001);
        failed += test_result("(Dec) for Hyakutake 1996/03/23 17:38",
                pos.dec, 32.4803, 0.001);

        JD = ln_get_julian_day(&date);

        ret = ln_get_ell_body_rst(JD, &observer, &orbit, &rst);
        failed += test_result("Hyakutake sometime rise on 1996/03/23 at 135 E, 35 N",
                ret, 0, 0);

        if (!ret) {
                ln_get_date(rst.rise, &date);
                failed += test_result("Hyakutake rise hour on 1996/03/23 at 135 E, 35 N",
                        date.hours, 9, 0);
                failed += test_result("Hyakutake rise minute on 1996/03/23 at 135 E, 35 N",
                        date.minutes, 31, 0);

                ln_get_date(rst.transit, &date);
                failed += test_result("Hyakutake transit hour on 1996/03/23 at 135 E, 35 N",
                        date.hours, 17, 0);
                failed += test_result("Hyakutake transit minute on 1996/03/23 at 135 E, 35 N",
                        date.minutes, 27, 0);

                ln_get_date(rst.set, &date);
                failed += test_result("Hyakutake set hour on 1996/03/23 at 135 E, 35 N",
                        date.hours, 1, 0);
                failed += test_result("Hyakutake set minute on 1996/03/23 at 135 E, 35 N",
                        date.minutes, 49, 0);
        }

        ret = ln_get_ell_body_next_rst(JD, &observer, &orbit, &rst);
        failed += test_result ("Hyakutake sometime rise on 1996/03/23 at 135 E, 35 N", ret, 0, 0);

        if (!ret) {
                ln_get_date(rst.rise, &date);
                failed += test_result("Hyakutake next rise hour on 1996/03/23 at 135 E, 35 N",
                        date.hours, 9, 0);
                failed += test_result("Hyakutake next rise minute on 1996/03/23 at 135 E, 35 N",
                        date.minutes, 31, 0);

                ln_get_date(rst.transit, &date);
                failed += test_result("Hyakutake next transit hour on 1996/03/24 at 135 E, 35 N",
                        date.hours, 17, 0);
                failed += test_result("Hyakutake next transit minute on 1996/03/24 at 135 E, 35 N",
                        date.minutes, 4, 0);

                ln_get_date(rst.set, &date);
                failed += test_result ("Hyakutake next set hour on 1996/03/23 at 135 E, 35 N",
                        date.hours, 1, 0);
                failed += test_result ("Hyakutake next set minute on 1996/03/23 at 135 E, 35 N",
                        date.minutes, 49, 0);
        }

        return failed;
}

static int hyp_future_rst_test(void)
{
        struct ln_lnlat_posn observer;
        struct ln_hyp_orbit orbit;
        struct ln_date date;
        struct ln_rst_time rst;
        double JD;
        int ret;
        int failed = 0;

        observer.lng = 15.0;
        observer.lat = 50.0;

        /* C/2006 P1 (McNaught) */

        orbit.q = 0.170742005109787;
        orbit.e = 1.00001895427704;
        orbit.i = 77.8348999023438;
        orbit.w = 155.977096557617;
        orbit.omega = 267.414398193359;
        orbit.JD = 2454113.251;

        date.years = 2007;
        date.months = 1;
        date.days = 17;

        date.hours = 12;
        date.minutes = 0;
        date.seconds = 0.0;

        JD = ln_get_julian_day(&date);

        ret = ln_get_hyp_body_next_rst_horizon(JD, &observer, &orbit, 0, &rst);
        failed += test_result("McNaught rise on 2997/01/18 at 15 E, 50 N",
                ret, 0, 0);

        if (!ret) {
                ln_get_date(rst.rise, &date);
                failed += test_result("McNaught rise hour on 2007/01/18 at 15 E, 50 N",
                        date.hours, 9, 0);
                failed += test_result("McNaught rise minute on 2007/01/18 at 15 E, 50 N",
                        date.minutes, 6, 0);

                ln_get_date(rst.transit, &date);
                failed += test_result("McNaught transit hour on 2007/01/18 at 15 E, 50 N",
                        date.hours, 11, 0);
                failed += test_result("McNaught transit minute on 2007/01/18 at 15 E, 50 N",
                        date.minutes, 38, 0);

                ln_get_date(rst.set, &date);
                failed += test_result("McNaught set hour on 2007/01/17 at 15 E, 50 N",
                        date.hours, 14, 0);
                failed += test_result("McNaught set minute on 2007/01/17 at 15 E, 50 N",
                        date.minutes, 37, 0);
        }

        ret = ln_get_hyp_body_next_rst_horizon(JD, &observer, &orbit, 15, &rst);
        failed += test_result("McNaught does not rise above 15 degrees on"
                "2007/01/17 at 15 E, 50 N", ret, -1, 0);

        return failed;
}

static int body_future_rst_test(void)
{
        struct ln_lnlat_posn observer;
        struct ln_date date;
        struct ln_rst_time rst;
        double JD;
        int ret;
        int failed = 0;

        observer.lng = 0;
        observer.lat = 85;

        date.years = 2006;
        date.months = 1;
        date.days = 1;

        date.hours = date.minutes = 0;
        date.seconds = 0.0;

        JD = ln_get_julian_day(&date);

        ret = ln_get_body_next_rst_horizon_future(JD, &observer,
                ln_get_solar_equ_coords, LN_SOLAR_STANDART_HORIZON, 300, &rst);

        failed += test_result("Sun is above horizon sometimes at 0, 85 N",
                ret, 0, 0);

        if (!ret) {
                ln_get_date(rst.rise, &date);
                failed += test_result("Solar next rise years at 0, 85 N",
                        date.years, 2006, 0);
                failed += test_result("Solar next rise months at 0, 85 N",
                        date.months, 3, 0);
                failed += test_result("Solar next rise days at 0, 85 N",
                        date.days, 7, 0);
                failed += test_result("Solar next rise hour at 0, 85 N",
                        date.hours, 10, 0);
                failed += test_result("Solar next rise minute at 0, 85 N",
                        date.minutes, 19, 0);

                ln_get_date(rst.transit, &date);
                failed += test_result("Solar next transit years at 0, 85 N",
                        date.years, 2006, 0);
                failed += test_result("Solar next transit months at 0, 85 N",
                        date.months, 3, 0);
                failed += test_result("Solar next transit days at 0, 85 N",
                        date.days, 7, 0);
                failed += test_result("Solar next transit hour at 0 E, 85 N",
                        date.hours, 12, 0);
                failed += test_result("Solar next transit minute at 0 E, 85 N",
                        date.minutes, 10, 0);

                ln_get_date(rst.set, &date);
                failed += test_result("Solar next set years at 0, 85 N",
                        date.years, 2006, 0);
                failed += test_result("Solar next set months at 0, 85 N",
                        date.months, 3, 0);
                failed += test_result("Solar next set days at 0, 85 N",
                        date.days, 7, 0);
                failed += test_result("Solar next set hour at 0 E, 85 N",
                        date.hours, 14, 0);
                failed += test_result("Solar next set minute at 0, 85 N",
                        date.minutes, 8, 0);
        }

        ret = ln_get_body_next_rst_horizon_future(JD, &observer, ln_get_solar_equ_coords, 0, 300, &rst);
        failed += test_result ("Sun is above 0 horizon sometimes at 0, 85 N", ret, 0, 0);

        if (!ret) {
                ln_get_date(rst.rise, &date);
                failed += test_result("Solar next rise years at 0, 85 N with 0 horizon",
                        date.years, 2006, 0);
                failed += test_result("Solar next rise months at 0, 85 N with 0 horizon",
                        date.months, 3, 0);
                failed += test_result("Solar next rise days at 0, 85 N with 0 horizon",
                        date.days, 9, 0);
                failed += test_result("Solar next rise hour at 0, 85 N with 0 horizon",
                        date.hours, 10, 0);
                failed += test_result("Solar next rise minute at 0, 85 N with 0 horizon",
                        date.minutes, 23, 0);

                ln_get_date(rst.transit, &date);
                failed += test_result("Solar next transit years at 0, 85 N with 0 horizon",
                        date.years, 2006, 0);
                failed += test_result("Solar next transit months at 0, 85 N with 0 horizon",
                        date.months, 3, 0);
                failed += test_result("Solar next transit days at 0, 85 N with 0 horizon",
                        date.days, 9, 0);
                failed += test_result("Solar next transit hour at 0 E, 85 N with 0 horizon",
                        date.hours, 12, 0);
                failed += test_result("Solar next transit minute at 0 E, 85 N with 0 horizon",
                        date.minutes, 10, 0);

                ln_get_date(rst.set, &date);
                failed += test_result("Solar next set years at 0, 85 N with 0 horizon",
                        date.years, 2006, 0);
                failed += test_result("Solar next set months at 0, 85 N with 0 horizon",
                        date.months, 3, 0);
                failed += test_result("Solar next set days at 0, 85 N with 0 horizon",
                        date.days, 9, 0);
                failed += test_result("Solar next set hour at 0 E, 85 N with 0 horizon",
                        date.hours, 14, 0);
                failed += test_result("Solar next set minute at 0, 85 N with 0 horizon",
                        date.minutes, 2, 0);
        }

        return failed;
}

int main(int argc, const char *argv[])
{
        int failed = 0;

        start_timer();

        failed += vsop87_test();
        failed += ell_rst_test ();
        failed += hyp_future_rst_test ();
        failed += body_future_rst_test ();

        end_timer();
        fprintf(stdout, "Test completed: %d tests, %d errors.\n",
                test_number, failed);

        return failed;
}
