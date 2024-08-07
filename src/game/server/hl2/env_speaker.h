//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once

// ===================================================================================
//
// Speaker class. Used for announcements per level, for door lock/unlock spoken voice.
//

class CSpeaker : public CPointEntity {
public:
	DECLARE_CLASS( CSpeaker, CPointEntity );

	void Spawn();
	void Precache();

	DECLARE_DATADESC();

	virtual int ObjectCaps() { return ( BaseClass::ObjectCaps() & ~FCAP_ACROSS_TRANSITION ); }

	virtual IResponseSystem* GetResponseSystem() { return m_pInstancedResponseSystem; }

	virtual int Save( ISave& save );
	virtual int Restore( IRestore& restore );

protected:
	void SpeakerThink();

	void InputToggle( inputdata_t& inputdata );

	float m_delayMin;
	float m_delayMax;

	string_t m_iszRuleScriptFile;
	string_t m_iszConcept;
	IResponseSystem* m_pInstancedResponseSystem;

public:
	void InputTurnOff( inputdata_t& inputdata );
	void InputTurnOn( inputdata_t& inputdata );
};
