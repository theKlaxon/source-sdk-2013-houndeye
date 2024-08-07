//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "stringregistry.h"
#include "tier1/utlstring.h"
class CEnvSoundscape;

struct clusterSoundscapeList_t {
	unsigned short soundscapeCount;
	unsigned short firstSoundscape;
};


class CSoundscapeSystem : public CAutoGameSystemPerFrame {
public:
	CSoundscapeSystem( char const* name )
		: CAutoGameSystemPerFrame( name ) { }

	// game system
	virtual bool Init();
	virtual void Shutdown();
	virtual void FrameUpdatePostEntityThink();
	virtual void LevelInitPreEntity();
	virtual void LevelInitPostEntity();

	virtual void AddSoundscapeFile( const char* filename );
	int GetSoundscapeIndex( const char* pName );
	bool IsValidIndex( int index );

	void FlushSoundscapes();
	void AddSoundscapeEntity( CEnvSoundscape* pSoundscape );
	void RemoveSoundscapeEntity( CEnvSoundscape* pSoundscape );
	void PrintDebugInfo();

	void AddSoundscapeSounds( KeyValues* pSoundscape, int soundscapeIndex );
	void PrecacheSounds( int soundscapeIndex );

private:
	CStringRegistry m_soundscapes;
	int m_soundscapeCount;
	CUtlVector<CEnvSoundscape*> m_soundscapeEntities;
	CUtlVector<clusterSoundscapeList_t> m_soundscapesInCluster;
	CUtlVector<unsigned short> m_soundscapeIndexList;
	int m_activeIndex;
	CUtlVector<CUtlVector<CUtlString>> m_soundscapeSounds;
};

extern CSoundscapeSystem g_SoundscapeSystem;
