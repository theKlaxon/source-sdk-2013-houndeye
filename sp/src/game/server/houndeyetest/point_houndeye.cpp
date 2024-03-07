#include "cbase.h"
#include "point_houndeye.h"
#include "point_houndeye_manager.h"

LINK_ENTITY_TO_CLASS(houndeye_point, CHoundeyePoint)

BEGIN_DATADESC(CHoundeyePoint)
	
	DEFINE_KEYFIELD( m_nBehavior, FIELD_INTEGER, "HoundeyeTask"),
	DEFINE_KEYFIELD( m_bEnabled, FIELD_BOOLEAN, "HoundeyePointEnabled"),

	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable),

END_DATADESC()

void CHoundeyePoint::Spawn() {
	BaseClass::Spawn();

	CHoundeyePointManager::GetInstance().RegisterPoint(this);
}

void CHoundeyePoint::InputEnable(inputdata_t& inputdata) {

	m_bEnabled = true;
	m_outOnEnabled.FireOutput(this, this);
	CHoundeyePointManager::GetInstance().RegisterPoint(this);
}

void CHoundeyePoint::InputDisable(inputdata_t& inputdata) {

	m_bEnabled = false;
	m_outOnDisabled.FireOutput(this, this);
	CHoundeyePointManager::GetInstance().UnRegisterPoint(this);
}