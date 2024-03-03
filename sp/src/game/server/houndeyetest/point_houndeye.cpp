#include "cbase.h"
#include "point_houndeye.h"

LINK_ENTITY_TO_CLASS(houndeye_point, CHoundeyeWaypoint)

BEGIN_DATADESC(CHoundeyeWaypoint)
	
	DEFINE_KEYFIELD( m_nBehavior, FIELD_INTEGER, "Houndeye Task (0 = Watch TV, 1 = Inspect)"),
	DEFINE_KEYFIELD( m_bDisabled, FIELD_BOOLEAN, "Is this waypoint disabled?")

END_DATADESC()

CHoundeyeWaypoint::CHoundeyeWaypoint() {

}