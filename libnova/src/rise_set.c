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

Copyright 2002-2003 Liam Girdwood

*/

#include "libnova.h"
#include <math.h>

/*! \fn double get_object_rst (double JD, struct ln_lnlat_posn * observer, struct ln_equ_posn * object, struct ln_rst_time * rst);
* \param JD Julian day
* \param observer Observers position
* \param object Object position
* \param rst Pointer to store Rise, Set and Transit time in JD
* \return 0 for success, else 1 for circumpolar.
*
* Calculate the time the rise, set and transit (crosses the local meridian at upper culmination)
* time of the object for the given Julian day.
*
* Note: this functions returns 1 if the object is circumpolar, that is it remains the whole
* day either above or below the horizon.
*/
int
get_object_rst (double JD, struct ln_lnlat_posn *observer,
		struct ln_equ_posn *object, struct ln_rst_time *rst)
{
  int jd;
  double T, O, JD_UT, H0, H1;
  double mt, mr, ms;
  double h = -0.5667;		/* standard altitude of stars */

  /* dynamical time diff */
  T = get_dynamical_time_diff (JD);

  /* convert local sidereal time into degrees
     for 0h of UT on day JD */
  jd = (int) JD;
  if (JD - jd > 0.5)
    JD_UT = jd + 0.5 + (T / (24 * 60 * 60));
  else
    JD_UT = jd - 0.5 + (T / (24 * 60 * 60));
  O = get_apparent_sidereal_time (JD_UT);
  O *= 15.0;

  /* equ 15.1 */
  H0 =
    (sin (deg_to_rad (h)) -
     sin (deg_to_rad (observer->lat)) * sin (deg_to_rad (object->dec)));
  H1 = (cos (deg_to_rad (observer->lat)) * cos (deg_to_rad (object->dec)));

  H1 = H0 / H1;

  /* check if body is circumpolar */
  if (H1 > 1.0)
    return (1);
  if (H1 < -1.0)
    return (-1);

  H0 = acos (H1);
  H0 = rad_to_deg (H0);

  /* equ 15.2 */
  mt = (object->ra + observer->lng - O) / 360.0;
  mr = mt - H0 / 360.0;
  ms = mt + H0 / 360.0;

  /* put in correct range */
  if (mt > 1.0)
    mt--;
  else if (mt < 0)
    mt++;
  if (mr > 1.0)
    mr--;
  else if (mr < 0)
    mr++;
  if (ms > 1.0)
    ms--;
  else if (ms < 0)
    ms++;

  rst->rise = JD_UT + mr;
  rst->transit = JD_UT + mt;
  rst->set = JD_UT + ms;

  /* not circumpolar */
  return (0);
}

int
get_object_next_rst (double JD, struct ln_lnlat_posn *observer,
		     struct ln_equ_posn *object, struct ln_rst_time *rst)
{
  struct ln_rst_time rst_p0;
  double t_JD = JD;
  int ret;
  ret = get_object_rst (JD, observer, object, &rst_p0);
#define get_next_rst(val) \
	while (1) \
	{ \
		if (rst_p0.val < JD) \
			t_JD = t_JD + 1; \
		else if (rst_p0.val > JD + 1) \
			t_JD = t_JD - 1; \
		else \
			break; \
		get_object_rst (t_JD, observer, object, &rst_p0); \
	} \
	rst->val = rst_p0.val;
  get_next_rst (rise);
  get_next_rst (set);
  get_next_rst (transit);
#undef get_next_rst
  return ret;
}

/*! \fn double get_body_rst_horizont (double JD_UT, struct ln_lnlat_posn *
* observer, struct ln_equ_posn * obj1, struct ln_equ_posn * obj2, struct
* ln_equ_posn obj3, double horizont, struct ln_rst_time * rst); 
* \param JD Julian day 
* \param observer Observers position 
* \param horizont Sun horizont use *_HORIZONT constants from libnova.h 
* \param rst Pointer to store Rise, Set and Transit time in JD 
* \return 0 for success, else 1 for circumpolar.
*
* Calculate the time the rise, set and transit (crosses the local meridian at
* upper culmination) time of the body for the given Julian day and given
* horizont.
*
* Note: this functions returns 1 if the body is circumpolar, that is it remains
* the whole day either above or below the horizon.
*/
int
get_body_rst_horizont (double JD, struct ln_lnlat_posn *observer,
		       void (*get_equ_body_coords) (double,
						    struct ln_equ_posn *),
		       double horizont, struct ln_rst_time *rst)
{
  int jd;
  double T, O, JD_UT, H0, H1;
  double Hat, Har, Has, altr, alts;
  double mt, mr, ms, mst, msr, mss, nt, nr, ns;
  struct ln_equ_posn sol1, sol2, sol3, post, posr, poss;
  double dmt, dmr, dms;

  /* dynamical time diff */
  T = get_dynamical_time_diff (JD);

  /* convert local sidereal time into degrees
     for 0h of UT on day JD */
  jd = (int) JD;
  if (JD - jd > 0.5)
    JD_UT = jd + 0.5 + (T / 86400.0);
  else
    JD_UT = jd - 0.5 + (T / 86400.0);
  O = get_apparent_sidereal_time (JD_UT);
  O *= 15.0;

  /* get body coords for JD_UT -1, JD_UT and JD_UT + 1 */
  get_equ_body_coords (JD_UT - 1.0, &sol1);
  get_equ_body_coords (JD_UT, &sol2);
  get_equ_body_coords (JD_UT + 1.0, &sol3);

  /* equ 15.1 */
  H0 =
    (sin (deg_to_rad (horizont)) -
     sin (deg_to_rad (observer->lat)) * sin (deg_to_rad (sol2.dec)));
  H1 = (cos (deg_to_rad (observer->lat)) * cos (deg_to_rad (sol2.dec)));

  H1 = H0 / H1;

  /* check if body is circumpolar */
  if (H1 > 1.0)
    return (1);
  if (H1 < -1.0)
    return (-1);

  H0 = acos (H1);
  H0 = rad_to_deg (H0);

  /* equ 15.2 */
  mt = (sol2.ra + observer->lng - O) / 360.0;
  mr = mt - H0 / 360.0;
  ms = mt + H0 / 360.0;

  /* put in correct range */
  if (mt > 1.0)
    mt--;
  else if (mt < 0)
    mt++;
  if (mr > 1.0)
    mr--;
  else if (mr < 0)
    mr++;
  if (ms > 1.0)
    ms--;
  else if (ms < 0)
    ms++;

  /* find sidereal time at Greenwich, in degrees, for each m */
  mst = O + 360.985647 * mt;
  msr = O + 360.985647 * mr;
  mss = O + 360.985647 * ms;

  /* calc n */
  nt = mt + T / 86400.0;
  nr = mr + T / 86400.0;
  ns = ms + T / 86400.0;

  /* interpolate ra and dec for each m, except for transit dec (dec2) */
  posr.ra = interpolate3 (nr, sol1.ra, sol2.ra, sol3.ra);
  posr.dec = interpolate3 (nr, sol1.dec, sol2.dec, sol3.dec);
  post.ra = interpolate3 (nt, sol1.ra, sol2.ra, sol3.ra);
  poss.ra = interpolate3 (ns, sol1.ra, sol2.ra, sol3.ra);
  poss.dec = interpolate3 (ns, sol1.dec, sol2.dec, sol3.dec);

  /* find local hour angle */
  Hat = mst - observer->lng - post.ra;
  Har = msr - observer->lng - posr.ra;
  Has = mss - observer->lng - poss.ra;

  /* find altitude for rise and set */
  altr = sin (deg_to_rad (observer->lat)) * sin (deg_to_rad (posr.dec)) +
    cos (deg_to_rad (observer->lat)) * cos (deg_to_rad (posr.dec)) *
    cos (deg_to_rad (Har));
  alts =
    sin (deg_to_rad (observer->lat)) * sin (deg_to_rad (poss.dec)) +
    cos (deg_to_rad (observer->lat)) * cos (deg_to_rad (poss.dec)) *
    cos (deg_to_rad (Has));

  /* must be in degrees */
  altr = rad_to_deg (altr);
  alts = rad_to_deg (alts);

  /* corrections for m */
  range_degrees (Hat);
  if (Hat > 180.0)
    Hat -= 360;

  dmt = -(Hat / 360.0);
  dmr =
    (altr -
     horizont) / (360 * cos (deg_to_rad (posr.dec)) *
		  cos (deg_to_rad (observer->lat)) * sin (deg_to_rad (Har)));
  dms =
    (alts -
     horizont) / (360 * cos (deg_to_rad (poss.dec)) *
		  cos (deg_to_rad (observer->lat)) * sin (deg_to_rad (Has)));

  /* add corrections and change to JD */
  mt += dmt;
  mr += dmr;
  ms += dms;
  rst->rise = JD_UT + mr;
  rst->transit = JD_UT + mt;
  rst->set = JD_UT + ms;

  /* not circumpolar */
  return (0);
}
