#pragma once
#include "baseentity.h"
#include "point_camera.h"

enum HoundeyePointBehavior_t {
	HEYE_POINT_WATCH_TV = 0,
	HEYE_POINT_INSPECT_DEADGUY,
};

class CHoundeyeWaypoint : public CPointEntity {
	DECLARE_CLASS(CHoundeyeWaypoint, CPointEntity)
	DECLARE_DATADESC()
public:

	CHoundeyeWaypoint();

	HoundeyePointBehavior_t GetBehavior() { return m_nBehavior; }

	bool IsInUse();

private:

	HoundeyePointBehavior_t m_nBehavior;
	bool m_bDisabled;

};