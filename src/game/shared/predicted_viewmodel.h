//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "baseplayer_shared.h"
#include "predictable_entity.h"
#include "shared_classnames.h"
#include "utlvector.h"

#if defined( CLIENT_DLL )
	#define CPredictedViewModel C_PredictedViewModel
#endif

class CPredictedViewModel : public CBaseViewModel {
	DECLARE_CLASS( CPredictedViewModel, CBaseViewModel );

public:
	DECLARE_NETWORKCLASS();

	CPredictedViewModel();
	virtual ~CPredictedViewModel();

	virtual void CalcViewModelLag( Vector& origin, QAngle& angles, QAngle& original_angles );

	#if defined( CLIENT_DLL )
		virtual bool ShouldPredict() {
			if ( GetOwner() && GetOwner() == C_BasePlayer::GetLocalPlayer() )
				return true;

			return BaseClass::ShouldPredict();
		}
	#endif

private:
	#if defined( CLIENT_DLL )
		// This is used to lag the angles.
		CInterpolatedVar<QAngle> m_LagAnglesHistory;
		QAngle m_vLagAngles;

		CPredictedViewModel( const CPredictedViewModel& );// not defined, not accessible
	#endif
};
