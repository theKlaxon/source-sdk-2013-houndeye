//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "vphysics_interface.h"

class ISaveRestoreBlockHandler;
class IPhysicsObject;
class CPhysCollide;

//-----------------------------------------------------------------------------

ISaveRestoreBlockHandler* GetPhysSaveRestoreBlockHandler();
ISaveRestoreOps* GetPhysObjSaveRestoreOps( PhysInterfaceId_t );

//-------------------------------------

#define DEFINE_PHYSPTR( name ) \
	{ FIELD_CUSTOM, #name, { static_cast<int>( offsetof( classNameTypedef, name ) ), 0 }, 1, FTYPEDESC_SAVE, NULL, GetPhysObjSaveRestoreOps( GetPhysIID( &( ( (classNameTypedef*) 0 )->name ) ) ), NULL }

#define DEFINE_PHYSPTR_ARRAY( name ) \
	{ FIELD_CUSTOM, #name, { static_cast<int>( offsetof( classNameTypedef, name ) ), 0 }, ARRAYSIZE( ( (classNameTypedef*) 0 )->name ), FTYPEDESC_SAVE, NULL, GetPhysObjSaveRestoreOps( GetPhysIID( &( ( (classNameTypedef*) 0 )->name[ 0 ] ) ) ), NULL }

//-----------------------------------------------------------------------------

abstract_class IPhysSaveRestoreManager {
public:
	virtual void NoteBBox( const Vector& mins, const Vector& maxs, CPhysCollide* ) = 0;

	virtual void AssociateModel( IPhysicsObject*, int modelIndex ) = 0;
	virtual void AssociateModel( IPhysicsObject*, const CPhysCollide* pModel ) = 0;
	virtual void ForgetModel( IPhysicsObject* ) = 0;

	virtual void ForgetAllModels() = 0;
};

extern IPhysSaveRestoreManager* g_pPhysSaveRestoreManager;
