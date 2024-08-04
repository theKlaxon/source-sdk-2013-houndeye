//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "isaverestore.h"

#if IsWindows()
	#pragma warning( push )
	#include <yvals.h>// warnings get enabled in yvals.h
	#pragma warning( disable : 4663 )
	#pragma warning( disable : 4530 )
	#pragma warning( disable : 4245 )
	#pragma warning( disable : 4018 )
	#pragma warning( disable : 4511 )
#endif

#include <string>

#if IsWindows()
	#pragma warning( pop )
#endif

class CStdStringSaveRestoreOps : public CDefSaveRestoreOps {
public:
	enum {
		MAX_SAVE_LEN = 4096,
	};

	// save data type interface
	virtual void Save( const SaveRestoreFieldInfo_t& fieldInfo, ISave* pSave ) {
		auto* pString = static_cast<std::string*>( fieldInfo.pField );
		Assert( pString->length() < MAX_SAVE_LEN - 1 );
		if ( pString->length() < MAX_SAVE_LEN - 1 ) {
			pSave->WriteString( pString->c_str() );
		} else {
			pSave->WriteString( "<<invalid>>" );
		}
	}

	virtual void Restore( const SaveRestoreFieldInfo_t& fieldInfo, IRestore* pRestore ) {
		auto* pString = static_cast<std::string*>( fieldInfo.pField );
		char szString[ MAX_SAVE_LEN ];
		pRestore->ReadString( szString, sizeof( szString ), 0 );
		szString[ MAX_SAVE_LEN - 1 ] = 0;
		pString->assign( szString );
	}

	virtual void MakeEmpty( const SaveRestoreFieldInfo_t& fieldInfo ) {
		auto* pString = static_cast<std::string*>( fieldInfo.pField );
		pString->erase();
	}

	virtual bool IsEmpty( const SaveRestoreFieldInfo_t& fieldInfo ) {
		auto* pString = static_cast<std::string*>( fieldInfo.pField );
		return pString->empty();
	}
};

//-------------------------------------

inline ISaveRestoreOps* GetStdStringDataOps() {
	static CStdStringSaveRestoreOps ops;
	return &ops;
}

//-------------------------------------

#define DEFINE_STDSTRING( name ) \
	{ FIELD_CUSTOM, #name, { static_cast<int>( offsetof( classNameTypedef, name ) ), 0 }, 1, FTYPEDESC_SAVE, NULL, GetStdStringDataOps(), NULL }
