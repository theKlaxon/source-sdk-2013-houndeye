//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once


class IHLTVServer;
class KeyValues;
class Vector;

#define INTERFACEVERSION_HLTVDIRECTOR "HLTVDirector001"

class IHLTVDirector {
public:
	virtual ~IHLTVDirector() { }

	virtual bool IsActive() = 0;// true if director is active

	virtual void SetHLTVServer( IHLTVServer* hltv ) = 0;// give the director the engine HLTV interface
	virtual IHLTVServer* GetHLTVServer() = 0;     // get current HLTV server interface

	virtual int GetDirectorTick() = 0;// get current broadcast tick from director
	virtual int GetPVSEntity() = 0;   // get current view entity (PVS), 0 if coords are used
	virtual Vector GetPVSOrigin() = 0;// get current PVS origin
	virtual float GetDelay() = 0;     // returns current delay in seconds

	virtual const char** GetModEvents() = 0;
};
