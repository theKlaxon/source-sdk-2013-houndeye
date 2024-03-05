#include "cbase.h"
#include "point_houndeye_manager.h"

CHoundeyePointManager::CHoundeyePointManager() {

}

void CHoundeyePointManager::RegisterPoint(CHoundeyePoint* pPoint) {
	m_pPoints.AddToTail(pPoint);
}

void CHoundeyePointManager::UnRegisterPoint(CHoundeyePoint* pPoint) {
	m_pPoints.FindAndFastRemove(pPoint);
}

CHoundeyePoint* CHoundeyePointManager::GetPointClosestToPos(Vector const vecPos) {

	CHoundeyePoint* prevClosePoint = nullptr;
	Vector prevCloseDist = vec3_origin;
	for (int i = 0; i < m_pPoints.Count(); i++) {

		Vector dist = m_pPoints[i]->GetAbsOrigin() - vecPos;

		if (prevClosePoint == nullptr || dist.Length() < prevCloseDist.Length()) {
			prevClosePoint = m_pPoints[i];
			prevCloseDist = dist;
		}
	}

	return prevClosePoint;
}

CHoundeyePointManager* CHoundeyePointManager::m_sInstance = NULL;
CHoundeyePointManager& CHoundeyePointManager::GetInstance() {

	if (!m_sInstance)
		m_sInstance = new CHoundeyePointManager();

	return *m_sInstance;
}