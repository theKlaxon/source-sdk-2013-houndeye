//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "clientmode_shared.h"
#include "sdkviewport.h"

class ClientModeSDKNormal : public ClientModeShared {
	DECLARE_CLASS( ClientModeSDKNormal, ClientModeShared );

private:
	// IClientMode overrides.
public:
	ClientModeSDKNormal();
	virtual ~ClientModeSDKNormal();

	virtual void InitViewport();

	virtual float GetViewModelFOV( void );

	int GetDeathMessageStartHeight( void );

	virtual void PostRenderVGui();


private:
	//	void	UpdateSpectatorMode( void );
};


extern IClientMode* GetClientModeNormal();
extern ClientModeSDKNormal* GetClientModeSDKNormal();
