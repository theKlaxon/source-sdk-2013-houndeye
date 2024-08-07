//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//
#pragma once
#include "interface.h"

class CBaseRecordingSessionBlock;

class IRecordingSession : public IBaseInterface {
public:
	virtual void AddBlock( CBaseRecordingSessionBlock* pBlock ) = 0;
};
