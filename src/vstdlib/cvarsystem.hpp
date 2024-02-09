//
// Created by ENDERZOMBI102 on 08/02/2024.
//
#pragma once

#include "vstdlib/cvar.h"

class CCVarSystem : public ICvar {
	// Allocate a unique DLL identifier
	CVarDLLIdentifier_t AllocateDLLIdentifier() override;

	// Register, unregister commands
	void RegisterConCommand( ConCommandBase * pCommandBase ) override;
	void UnregisterConCommand( ConCommandBase * pCommandBase ) override;
	void UnregisterConCommands( CVarDLLIdentifier_t id ) override;

	// If there is a +<varname> <value> on the command line, this returns the value.
	// Otherwise, it returns NULL.
	const char* GetCommandLineValue( const char* pVariableName ) override;

	// Try to find the cvar pointer by name
	ConCommandBase* FindCommandBase( const char* name ) override;
	const ConCommandBase* FindCommandBase( const char* name ) const override;
	ConVar* FindVar( const char* var_name ) override;
	const ConVar* FindVar( const char* var_name ) const override;
	ConCommand* FindCommand( const char* name ) override;
	const ConCommand* FindCommand( const char* name ) const override;

	// Get first ConCommandBase to allow iteration
	ConCommandBase* GetCommands() override;
	const ConCommandBase* GetCommands() const override;

	// Install a global change callback (to be called when any convar changes)
	void InstallGlobalChangeCallback( FnChangeCallback_t callback ) override;
	void RemoveGlobalChangeCallback( FnChangeCallback_t callback ) override;
	void CallGlobalChangeCallbacks( ConVar * var, const char* pOldString, float flOldValue ) override;

	// Install a console printer
	void InstallConsoleDisplayFunc( IConsoleDisplayFunc * pDisplayFunc ) override;
	void RemoveConsoleDisplayFunc( IConsoleDisplayFunc * pDisplayFunc ) override;
	void ConsoleColorPrintf( const Color& clr, PRINTF_FORMAT_STRING const char* pFormat, ... ) const override FMTFUNCTION( 3, 4 );
	void ConsolePrintf( PRINTF_FORMAT_STRING const char* pFormat, ... ) const override FMTFUNCTION( 2, 3 );
	void ConsoleDPrintf( PRINTF_FORMAT_STRING const char* pFormat, ... ) const override FMTFUNCTION( 2, 3 );

	// Reverts cvars which contain a specific flag
	void RevertFlaggedConVars( int nFlag ) override;

	// Method allowing the engine ICvarQuery interface to take over
	// A little hacky, owing to the fact the engine is loaded
	// well after ICVar, so we can't use the standard connect pattern
	void InstallCVarQuery( ICvarQuery * pQuery ) override;

	bool IsMaterialThreadSetAllowed() const override;
	void QueueMaterialThreadSetValue( ConVar * pConVar, const char* pValue ) override;
	void QueueMaterialThreadSetValue( ConVar * pConVar, int nValue ) override;
	void QueueMaterialThreadSetValue( ConVar * pConVar, float flValue ) override;
	bool HasQueuedMaterialThreadConVarSets() const override;
	int ProcessQueuedMaterialThreadConVarSets() override;
protected:
	// internals for  ICVarIterator
	class CCVarSystemIterator : public ICVarIteratorInternal {
	public:
		// warning: delete called on 'ICvar::ICVarIteratorInternal' that is abstract but has non-virtual destructor [-Wdelete-non-virtual-dtor]
		~CCVarSystemIterator() override = default;
		void SetFirst() override;
		void Next() override;
		bool IsValid() override;
		ConCommandBase* Get() override;
	};

	virtual ICVarIteratorInternal* FactoryInternalIterator() = 0;
	friend class Iterator;
private:
	int m_IdCounter{ 0 };
};
