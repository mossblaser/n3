/**
 * Convert from GPS' WGS84 coordinates and e.h. to an ordinance survey version.
 */

#ifndef N3_UI_WGS84_TO_OS_H
#define N3_UI_WGS84_TO_OS_H

#include "os_coord/os_coord.h"

/**
 * Convert from WGS84 lat/lon/eh to a OS/Irish OS national grid representation.
 */
os_grid_ref_t
n3_wgs84_to_os( os_lat_lon_t       ll_wgs84
              , os_tm_projection_t projection
              , os_grid_t          national_grid
              );

#endif




