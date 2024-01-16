//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: C callable material system interface for the utils.
//
// $Workfile:   $
// $Date:       $
// $NoKeywords: $
//=============================================================================//

#include "utilmatlib.h"
#include "cmdlib.h"
#include "materialsystem/imaterial.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/imaterialvar.h"
#include "tier0/dbg.h"
#include "materialsystem/materialsystem_config.h"
#include "mathlib/mathlib.h"


#if IsPosix()
	// TODO: Cry and fix this, matsys shouldn't need this, why does it???
	#define USE_SDL
	#include "appframework/ilaunchermgr.h"

	static CreateInterfaceFn g_fileSystemFactory;

	class FakeLauncherMgr : public ILauncherMgr {
	public:
		virtual bool Connect( CreateInterfaceFn factory ) { return true; }
		virtual void Disconnect() { }
		virtual void *QueryInterface( const char *pInterfaceName ) { return nullptr; }
		virtual InitReturnVal_t Init() { return static_cast<InitReturnVal_t>( 0 ); }
		virtual void Shutdown() { }
		virtual bool CreateGameWindow( const char *pTitle, bool bWindowed, int width, int height ) { return true; }
		virtual void IncWindowRefCount() { }
		virtual void DecWindowRefCount() { }
		virtual int GetEvents( CCocoaEvent *pEvents, int nMaxEventsToReturn, bool debugEvents = false ) { return 0; }
		#if IsLinux()
			virtual int PeekAndRemoveKeyboardEvents( bool *pbEsc, bool *pbReturn, bool *pbSpace, bool debugEvents = false ) { return 0; }
		#endif
		virtual void SetCursorPosition( int x, int y ) { }
		virtual void SetWindowFullScreen( bool bFullScreen, int nWidth, int nHeight ) { }
		virtual bool IsWindowFullScreen() { return true; }
		virtual void MoveWindow( int x, int y ) { }
		virtual void SizeWindow( int width, int tall ) { }
		virtual void PumpWindowsMessageLoop() { }
		virtual void DestroyGameWindow() { }
		virtual void SetApplicationIcon( const char *pchAppIconFile ) { }
		virtual void GetMouseDelta( int &x, int &y, bool bIgnoreNextMouseDelta = false ) { }
		virtual void GetNativeDisplayInfo( int nDisplay, uint &nWidth, uint &nHeight, uint &nRefreshHz ) { }
		virtual void RenderedSize( uint &width, uint &height, bool set ) { }
		virtual void DisplayedSize( uint &width, uint &height ) { }
		#if defined( DX_TO_GL_ABSTRACTION )
			virtual PseudoGLContextPtr	GetMainContext() { return nullptr; }
			virtual PseudoGLContextPtr GetGLContextForWindow( void* windowref ) { return nullptr; }
			virtual PseudoGLContextPtr CreateExtraContext() { return nullptr; }
			virtual void DeleteContext( PseudoGLContextPtr hContext ) { }
			virtual bool MakeContextCurrent( PseudoGLContextPtr hContext ) { return true; }
			virtual GLMDisplayDB *GetDisplayDB( void ) { return nullptr; }
			virtual void GetDesiredPixelFormatAttribsAndRendererInfo( uint **ptrOut, uint *countOut, GLMRendererInfoFields *rendInfoOut ) { }
			virtual void ShowPixels( CShowPixelsParams *params ) { }
		#endif
		virtual void GetStackCrawl( CStackCrawlParams *params ) { }
		virtual void WaitUntilUserInput( int msSleepTime ) { }
		virtual void *GetWindowRef() { return nullptr; }
		virtual void SetMouseVisible( bool bState ) { }
		virtual void SetMouseCursor( SDL_Cursor* hCursor ) { }
		virtual void SetForbidMouseGrab( bool bForbidMouseGrab ) { }
		virtual void OnFrameRendered() { }
		virtual void SetGammaRamp( const uint16 *pRed, const uint16 *pGreen, const uint16 *pBlue ) { }
		virtual double GetPrevGLSwapWindowTime() { return 0.0; }

	} g_FakeLauncherMgr;

	void* SDLMgrFactoryRedirector( const char* pName, int *pReturnCode ) {
		return !strcmp("SDLMgrInterface001", pName) ? &g_FakeLauncherMgr : g_fileSystemFactory( pName, pReturnCode );
	}
#endif

void LoadMaterialSystemInterface( CreateInterfaceFn fileSystemFactory ) {
	if ( g_pMaterialSystem ) {
		return;
	}

	#if IsPosix()
		g_fileSystemFactory = fileSystemFactory;
		fileSystemFactory = SDLMgrFactoryRedirector;
	#endif

	// materialsystem.dll should be in the path, it's in bin along with vbsp.
	const char* pDllName = "materialsystem" DLL_EXT_STRING;
	CSysModule* materialSystemDLLHInst = g_pFullFileSystem->LoadModule( pDllName );
	if ( !materialSystemDLLHInst ) {
		Error( "Can't load materialsystem" DLL_EXT_STRING "\n" );
	}

	CreateInterfaceFn clientFactory = Sys_GetFactory( materialSystemDLLHInst );
	if ( clientFactory ) {
		g_pMaterialSystem = static_cast<IMaterialSystem*>( clientFactory( MATERIAL_SYSTEM_INTERFACE_VERSION, nullptr ) );
		if ( !g_pMaterialSystem ) {
			Error( "Could not get the material system interface from materialsystem" DLL_EXT_STRING " (" __FILE__ ")" );
		}
	} else {
		Error( "Could not find factory interface in library materialsystem" DLL_EXT_STRING );
	}

	CreateInterfaceFn res;
	if ( !(res = g_pMaterialSystem->Init( "shaderapiempty" DLL_EXT_STRING, nullptr, fileSystemFactory )) ) {
		Error( "Could not start the empty shader %p (shaderapiempty" DLL_EXT_STRING ")!", res );
	}
}

void InitMaterialSystem( const char* materialBaseDirPath, CreateInterfaceFn fileSystemFactory ) {
	LoadMaterialSystemInterface( fileSystemFactory );
	MaterialSystem_Config_t config;
	g_pMaterialSystem->OverrideConfig( config, false );
}

void ShutdownMaterialSystem() {
	if ( g_pMaterialSystem ) {
		g_pMaterialSystem->Shutdown();
		g_pMaterialSystem = nullptr;
	}
}

MaterialSystemMaterial_t FindMaterial( const char* materialName, bool* pFound, bool bComplain ) {
	IMaterial* pMat = g_pMaterialSystem->FindMaterial( materialName, TEXTURE_GROUP_OTHER, bComplain );
	MaterialSystemMaterial_t matHandle = pMat;

	if ( pFound ) {
		*pFound = true;
		if ( IsErrorMaterial( pMat ) )
			*pFound = false;
	}

	return matHandle;
}

void GetMaterialDimensions( MaterialSystemMaterial_t materialHandle, int* width, int* height ) {
	PreviewImageRetVal_t retVal;
	ImageFormat dummyImageFormat;
	auto* material = static_cast<IMaterial*>( materialHandle );
	bool translucent;
	retVal = material->GetPreviewImageProperties( width, height, &dummyImageFormat, &translucent );
	if ( retVal != MATERIAL_PREVIEW_IMAGE_OK ) {
#if 0
		if (retVal == MATERIAL_PREVIEW_IMAGE_BAD ) 
		{
			Error( "problem getting preview image for %s", 
				g_pMaterialSystem->GetMaterialName( materialInfo[matID].materialHandle ) );
		}
#else
		*width = 128;
		*height = 128;
#endif
	}
}

void GetMaterialReflectivity( MaterialSystemMaterial_t materialHandle, float* reflectivityVect ) {
	auto* material = static_cast<IMaterial*>( materialHandle );
	const IMaterialVar* reflectivityVar;

	bool found;
	reflectivityVar = material->FindVar( "$reflectivity", &found, false );
	if ( !found ) {
		Vector tmp;
		material->GetReflectivity( tmp );
		VectorCopy( tmp.Base(), reflectivityVect );
	} else {
		reflectivityVar->GetVecValue( reflectivityVect, 3 );
	}
}

int GetMaterialShaderPropertyBool( MaterialSystemMaterial_t materialHandle, int propID ) {
	auto* material = static_cast<IMaterial*>( materialHandle );
	switch ( propID ) {
		case UTILMATLIB_NEEDS_BUMPED_LIGHTMAPS:
			return material->GetPropertyFlag( MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS );

		case UTILMATLIB_NEEDS_LIGHTMAP:
			return material->GetPropertyFlag( MATERIAL_PROPERTY_NEEDS_LIGHTMAP );

		default:
			Assert( 0 );
			return 0;
	}
}

int GetMaterialShaderPropertyInt( MaterialSystemMaterial_t materialHandle, int propID ) {
	auto* material = static_cast<IMaterial*>( materialHandle );
	switch ( propID ) {
		case UTILMATLIB_OPACITY:
			if ( material->IsTranslucent() )
				return UTILMATLIB_TRANSLUCENT;
			if ( material->IsAlphaTested() )
				return UTILMATLIB_ALPHATEST;
			return UTILMATLIB_OPAQUE;

		default:
			Assert( 0 );
			return 0;
	}
}

const char* GetMaterialVar( MaterialSystemMaterial_t materialHandle, const char* propertyName ) {
	auto* material = static_cast<IMaterial*>( materialHandle );
	bool found;
	IMaterialVar* var = material->FindVar( propertyName, &found, false );
	if ( found ) {
		return var->GetStringValue();
	}
	return nullptr;
}

const char* GetMaterialShaderName( MaterialSystemMaterial_t materialHandle ) {
	return static_cast<IMaterial*>( materialHandle )->GetShaderName();
}
