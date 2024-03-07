#pragma once
#include "point_houndeye.h"
#include "utlvector.h"

class CHoundeyePointManager {
public:

	CHoundeyePointManager();

	void RegisterPoint(CHoundeyePoint* pPoint);
	void UnRegisterPoint(CHoundeyePoint* pPoint);

	CHoundeyePoint* GetPointClosestToPos(Vector const vecPos);

	static CHoundeyePointManager& GetInstance();

private:

	CUtlVector<CHoundeyePoint*> m_pPoints;
	static CHoundeyePointManager* m_sInstance;
};