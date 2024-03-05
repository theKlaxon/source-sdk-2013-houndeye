#pragma once
#include "baseentity.h"
#include "point_camera.h"

enum HoundeyePointBehavior_t {
	HEYE_POINT_WATCH_TV = 0,
	HEYE_POINT_INSPECT_DEADGUY,
};

class CHoundeyePoint : public CPointEntity {
	DECLARE_CLASS(CHoundeyePoint, CPointEntity)
	DECLARE_DATADESC()
public:

	void Spawn();

	void InputEnable(inputdata_t& inputdata);
	void InputDisable(inputdata_t& inputdata);

	HoundeyePointBehavior_t GetBehavior() { return m_nBehavior; }

	friend class CHoundeyePointManager;
protected:
	void SetId(int nId) { m_nManagedId = nId; }
	int GetId() { return m_nManagedId; }

private:

	HoundeyePointBehavior_t m_nBehavior;
	COutputEvent m_outOnEnabled;
	COutputEvent m_outOnDisabled;

	bool m_bEnabled;
	int m_nManagedId;
};
