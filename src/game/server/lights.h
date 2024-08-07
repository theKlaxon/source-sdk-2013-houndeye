//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CLight : public CPointEntity {
public:
	DECLARE_CLASS( CLight, CPointEntity );

	bool KeyValue( const char* szKeyName, const char* szValue );
	void Spawn();
	void FadeThink();
	void Use( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value );

	void TurnOn();
	void TurnOff();
	void Toggle();

	// Input handlers
	void InputSetPattern( inputdata_t& inputdata );
	void InputFadeToPattern( inputdata_t& inputdata );

	void InputToggle( inputdata_t& inputdata );
	void InputTurnOn( inputdata_t& inputdata );
	void InputTurnOff( inputdata_t& inputdata );

	DECLARE_DATADESC();

private:
	int m_iStyle;
	int m_iDefaultStyle;
	string_t m_iszPattern;
	char m_iCurrentFade;
	char m_iTargetFade;
};
