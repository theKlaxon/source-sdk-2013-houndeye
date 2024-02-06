//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once

#include "matsys_controls/mdlpanel.h"

//-----------------------------------------------------------------------------
// Resource file data used in posing the model inside of the model panel.
//-----------------------------------------------------------------------------
struct BMPResAnimData_t {
	const char* m_pszName;
	const char* m_pszSequence;
	const char* m_pszActivity;
	KeyValues* m_pPoseParameters;
	bool m_bDefault;

	BMPResAnimData_t() {
		m_pszName = nullptr;
		m_pszSequence = nullptr;
		m_pszActivity = nullptr;
		m_pPoseParameters = nullptr;
		m_bDefault = false;
	}

	~BMPResAnimData_t() {
		if ( m_pszName && m_pszName[ 0 ] ) {
			delete[] m_pszName;
			m_pszName = nullptr;
		}

		if ( m_pszSequence && m_pszSequence[ 0 ] ) {
			delete[] m_pszSequence;
			m_pszSequence = nullptr;
		}

		if ( m_pszActivity && m_pszActivity[ 0 ] ) {
			delete[] m_pszActivity;
			m_pszActivity = nullptr;
		}

		if ( m_pPoseParameters ) {
			m_pPoseParameters->deleteThis();
			m_pPoseParameters = nullptr;
		}
	}
};

struct BMPResAttachData_t {
	const char* m_pszModelName;
	int m_nSkin;

	BMPResAttachData_t() {
		m_pszModelName = nullptr;
		m_nSkin = 0;
	}

	~BMPResAttachData_t() {
		if ( m_pszModelName && m_pszModelName[ 0 ] ) {
			delete[] m_pszModelName;
			m_pszModelName = nullptr;
		}
	}
};

struct BMPResData_t {
	float m_flFOV;

	const char* m_pszModelName;
	const char* m_pszModelName_HWM;
	const char* m_pszVCD;
	QAngle m_angModelPoseRot;
	Vector m_vecOriginOffset;
	Vector m_vecFramedOriginOffset;
	Vector2D m_vecViewportOffset;
	int m_nSkin;
	bool m_bUseSpotlight;

	CUtlVector<BMPResAnimData_t> m_aAnimations;
	CUtlVector<BMPResAttachData_t> m_aAttachModels;

	BMPResData_t() {
		m_flFOV = 0.0f;

		m_pszModelName = nullptr;
		m_pszModelName_HWM = nullptr;
		m_pszVCD = nullptr;
		m_angModelPoseRot.Init();
		m_vecOriginOffset.Init();
		m_vecFramedOriginOffset.Init();
		m_vecViewportOffset.Init();
		m_nSkin = 0;
		m_bUseSpotlight = false;
	}

	~BMPResData_t() {
		if ( m_pszModelName && m_pszModelName[ 0 ] ) {
			delete[] m_pszModelName;
			m_pszModelName = nullptr;
		}

		if ( m_pszModelName_HWM && m_pszModelName_HWM[ 0 ] ) {
			delete[] m_pszModelName_HWM;
			m_pszModelName_HWM = nullptr;
		}

		if ( m_pszVCD && m_pszVCD[ 0 ] ) {
			delete[] m_pszVCD;
			m_pszVCD = nullptr;
		}

		m_aAnimations.Purge();
		m_aAttachModels.Purge();
	}
};

//-----------------------------------------------------------------------------
// Base Model Panel
//
//	...vgui::Panel					|--> vgui
//	   +->vgui::EditablePanel		|
//	      +->PotterWheelPanel		|--> matsys_controls
//           +->MDLPanel			|
//		        +->BaseModelPanel	|--> game_controls, client.dll
//
//-----------------------------------------------------------------------------
class CBaseModelPanel : public CMDLPanel {
	DECLARE_CLASS_SIMPLE( CBaseModelPanel, CMDLPanel );

public:
	// Constructor, Destructor.
	CBaseModelPanel( vgui::Panel* pParent, const char* pName );
	virtual ~CBaseModelPanel();

	// Overridden mdlpanel.h
	virtual void SetMDL( MDLHandle_t handle, void* pProxyData = nullptr ) override;
	virtual void SetMDL( const char* pMDLName, void* pProxyData = nullptr ) override;
	virtual void SetModelAnglesAndPosition( const QAngle& angRot, const Vector& vecPos ) override;

	// Overridden methods of vgui::Panel
	virtual void ApplySettings( KeyValues* inResourceData ) override;
	virtual void PerformLayout() override;

	// Animation.
	int FindDefaultAnim( void );
	int FindAnimByName( const char* pszName );
	void SetModelAnim( int iAnim );

	// Manipulation.
	virtual void OnKeyCodePressed( vgui::KeyCode code ) override;
	virtual void OnKeyCodeReleased( vgui::KeyCode code ) override;
	virtual void OnMousePressed( vgui::MouseCode code ) override;
	virtual void OnMouseReleased( vgui::MouseCode code ) override;
	virtual void OnCursorMoved( int x, int y ) override;
	virtual void OnMouseWheeled( int delta ) override;

	studiohdr_t* GetStudioHdr( void ) { return m_RootMDL.m_MDL.GetStudioHdr(); }
	void SetBody( unsigned int nBody ) { m_RootMDL.m_MDL.m_nBody = nBody; }

	void RotateYaw( float flDelta );
	void RotatePitch( float flDelta );

	Vector GetPlayerPos() const;
	QAngle GetPlayerAngles() const;

	void LookAtBounds( const Vector& vecBoundsMin, const Vector& vecBoundsMax );

	// Set to true if external code has set a specific camera position that shouldn't be clobbered by layout
	void SetForcedCameraPosition( bool bForcedCameraPosition ) { m_bForcedCameraPosition = bForcedCameraPosition; }

	int FindSequenceFromActivity( CStudioHdr* pStudioHdr, const char* pszActivity );

protected:
	// Resource file data.
	void ParseModelResInfo( KeyValues* inResourceData );
	void ParseModelAnimInfo( KeyValues* inResourceData );
	void ParseModelAttachInfo( KeyValues* inResourceData );

	void SetupModelDefaults( void );
	void SetupModelAnimDefaults( void );

public:
	BMPResData_t m_BMPResData;// Base model panel data set in the .res file.
	QAngle m_angPlayer;
	Vector m_vecPlayerPos;

protected:
	bool m_bForcePos;
	bool m_bMousePressed;
	bool m_bAllowRotation;
	bool m_bAllowPitch;
	bool m_bAllowFullManipulation;
	bool m_bApplyManipulators;
	bool m_bForcedCameraPosition;

	// VGUI script accessible variables.
	CPanelAnimationVar( bool, m_bStartFramed, "start_framed", "0" );
	CPanelAnimationVar( bool, m_bDisableManipulation, "disable_manipulation", "0" );
	CPanelAnimationVar( bool, m_bUseParticle, "use_particle", "0" );
	CPanelAnimationVar( float, m_flMaxPitch, "max_pitch", "90" );

	struct particle_data_t {
		~particle_data_t();

		void UpdateControlPoints( CStudioHdr* pStudioHdr, matrix3x4_t* pWorldMatrix, const CUtlVector<int>& vecAttachments, int iDefaultBone = 0, const Vector& vecParticleOffset = vec3_origin );

		bool m_bIsUpdateToDate;
		CParticleCollection* m_pParticleSystem;
	};
	CUtlVector<particle_data_t*> m_particleList;

	particle_data_t* CreateParticleData( const char* pszParticleName );
	bool SafeDeleteParticleData( particle_data_t** pData );

	virtual void PrePaint3D( IMatRenderContext* pRenderContext ) override;
	virtual void PostPaint3D( IMatRenderContext* pRenderContext ) override;
};
