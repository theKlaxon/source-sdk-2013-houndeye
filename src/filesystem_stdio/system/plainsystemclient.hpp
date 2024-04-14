//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#pragma once

#include "isystemclient.hpp"

class CPlainSystemClient : public ISystemClient {
public:
	explicit CPlainSystemClient( const char* string ) : m_szNativePath{ string } { };
	auto GetNativePath() -> const char* override;
	auto Shutdown() -> const char* override;
public: // FS interaction
	auto Flush( FlushCallback callback ) -> void override;
	auto Walk( uint16_t nwname, const char* wname, WalkCallback callback ) -> void override;
	auto Open( const char* path, const char* mode, OpenCallback callback ) -> void override;
	auto Create( const char* name, dirmode_t perm, openmode_t mode, CreateCallback callback ) -> void override;
	auto Read( uint64_t offset, uint32_t count, ReadCallback callback ) -> void override;
	auto Write( uint64_t offset, uint32_t count, void* data, WriteCallback callback ) -> void override;
	auto Remove( RemoveCallback callback ) -> void override;
	auto Stat( StatCallback callback ) -> void override;
private:
	const char* m_szNativePath;
};