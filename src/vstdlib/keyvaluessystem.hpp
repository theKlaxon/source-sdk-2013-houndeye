//
// Created by ENDERZOMBI102 on 12/02/2024.
//
#pragma once

#include "vstdlib/IKeyValuesSystem.h"
#include <unordered_map>

class CKeyValuesSystem : public IKeyValuesSystem {
	friend IKeyValuesSystem* KeyValuesSystem();
public:
	void RegisterSizeofKeyValues( int size ) override;

	void* AllocKeyValuesMemory( int size ) override;
	void FreeKeyValuesMemory( void* pMem ) override;

	HKeySymbol GetSymbolForString( const char* name, bool bCreate = true ) override;
	const char* GetStringForSymbol( HKeySymbol symbol ) override;

	void AddKeyValuesToMemoryLeakList( void* pMem, HKeySymbol pName ) override;
	void RemoveKeyValuesFromMemoryLeakList( void* pMem ) override;

	void AddFileKeyValuesToCache( const KeyValues* _kv, const char* resourceName, const char* pathID ) override;
	bool LoadFileKeyValuesFromCache( KeyValues* _outKv, const char* resourceName, const char* pathID, IBaseFileSystem* filesystem ) const override;
	void InvalidateCache() override;
	void InvalidateCacheForFile( const char* resourceName, const char* pathID ) override;
public:
	~CKeyValuesSystem();
private:
	std::unordered_map<void*, HKeySymbol> m_LeakList{};
};
