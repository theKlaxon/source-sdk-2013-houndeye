//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//
#pragma once
#include "interface.h"

//----------------------------------------------------------------------------------------

class CReplay;
struct CaptureScreenshotParams_t;

//----------------------------------------------------------------------------------------

class IReplayScreenshotManager : public IBaseInterface {
public:
	virtual void CaptureScreenshot( CaptureScreenshotParams_t& params ) = 0;
	virtual void GetUnpaddedScreenshotSize( int& nOutWidth, int& nOutHeight ) = 0;
	virtual void DeleteScreenshotsForReplay( CReplay* pReplay ) = 0;
};
