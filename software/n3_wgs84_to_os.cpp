#include "n3_wgs84_to_os.h"

extern "C" {
#include "os_coord/os_coord.h"
#include "os_coord/os_coord_math.h"
#include "os_coord/os_coord_data.h"
#include "os_coord/os_coord_transform.h"
#include "os_coord/os_coord_ordinance_survey.h"
}

os_grid_ref_t
n3_wgs84_to_os( os_lat_lon_t       ll_wgs84
              , os_tm_projection_t projection
              , os_grid_t          national_grid
              )
{
	// Convert from the WGS-84 ellipsoid to 3D Cartesian coordinates
	os_cartesian_t c_wgs84 = os_lat_lon_to_cartesian(ll_wgs84, OS_EL_WGS84);
	
	// Perform the Helmert transform to (approximately) correct for differences in
	// datum
	os_cartesian_t c_airy30 = os_helmert_transform(c_wgs84, OS_HE_WGS84_TO_OSGB36);
	
	// Convert back to latitude, longitude and ellipsoidal height to give
	// coordinates on the Airy 1830 ellipsoid.
	os_lat_lon_t ll_airy30 = os_cartesian_to_lat_lon(c_airy30, OS_EL_AIRY_1830);
	
	// Project the coordinates to the National Grid's 2D projection
	os_eas_nor_t en_airy30 = os_lat_lon_to_tm_eas_nor(ll_airy30, projection);
	
	// Turn these Eastings and Northings into a grid references
	os_grid_ref_t grid_ref = os_eas_nor_to_grid_ref(en_airy30, national_grid);
	
	return grid_ref;
}


