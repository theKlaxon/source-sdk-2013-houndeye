//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "UtlCachedFileData.h"

#define MODELSOUNDSCACHE_VERSION 5

class CStudioHdr;

#pragma pack( 1 )
class CModelSoundsCache : public IBaseCacheInfo {
public:
	CUtlVector<unsigned short> sounds;

	CModelSoundsCache();
	CModelSoundsCache( const CModelSoundsCache& src );

	void PrecacheSoundList();

	virtual void Save( CUtlBuffer& buf );
	virtual void Restore( CUtlBuffer& buf );
	virtual void Rebuild( char const* filename );

	static void FindOrAddScriptSound( CUtlVector<unsigned short>& sounds, char const* soundname );
	static void BuildAnimationEventSoundList( CStudioHdr* hdr, CUtlVector<unsigned short>& sounds );

private:
	char const* GetSoundName( int index );
};
#pragma pack()
