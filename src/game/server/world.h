//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: The worldspawn entity. This spawns first when each level begins.
//
// $NoKeywords: $
//=============================================================================//
#pragma once


class CWorld : public CBaseEntity {
public:
	DECLARE_CLASS( CWorld, CBaseEntity );

	CWorld();
	~CWorld();

	DECLARE_SERVERCLASS();

	virtual int RequiredEdictIndex() { return 0; }// the world always needs to be in slot 0

	static void RegisterSharedActivities();
	static void RegisterSharedEvents();
	virtual void Spawn();
	virtual void Precache();
	virtual bool KeyValue( const char* szKeyName, const char* szValue );
	virtual void DecalTrace( trace_t* pTrace, char const* decalName );
	virtual void VPhysicsCollision( int index, gamevcollisionevent_t* pEvent ) {}
	virtual void VPhysicsFriction( IPhysicsObject* pObject, float energy, int surfaceProps, int surfacePropsHit ) {}

	inline void GetWorldBounds( Vector& vecMins, Vector& vecMaxs ) {
		VectorCopy( m_WorldMins, vecMins );
		VectorCopy( m_WorldMaxs, vecMaxs );
	}

	inline float GetWaveHeight() const {
		return (float) m_flWaveHeight;
	}

	bool GetDisplayTitle() const;
	bool GetStartDark() const;

	void SetDisplayTitle( bool display );
	void SetStartDark( bool startdark );

	bool IsColdWorld();

private:
	DECLARE_DATADESC();

	string_t m_iszChapterTitle;

	CNetworkVar( float, m_flWaveHeight );
	CNetworkVector( m_WorldMins );
	CNetworkVector( m_WorldMaxs );
	CNetworkVar( float, m_flMaxOccludeeArea );
	CNetworkVar( float, m_flMinOccluderArea );
	CNetworkVar( float, m_flMinPropScreenSpaceWidth );
	CNetworkVar( float, m_flMaxPropScreenSpaceWidth );
	CNetworkVar( string_t, m_iszDetailSpriteMaterial );

	// start flags
	CNetworkVar( bool, m_bStartDark );
	CNetworkVar( bool, m_bColdWorld );
	bool m_bDisplayTitle;
};


CWorld* GetWorldEntity();
extern const char* GetDefaultLightstyleString( int styleIndex );
