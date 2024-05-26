//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#include "plainsystemclient.hpp"

#include <cstdio>
#include <filesystem>

#include "dbg.h"

CPlainSystemClient::CPlainSystemClient( int id, std::string absolute, const char* path ) {
	this->m_iId = id;
	this->m_szNativePath = path;
	this->m_szNativeAbsolutePath = std::move( absolute );
}
auto CPlainSystemClient::Open( int id, std::string absolute, const char* path ) -> std::shared_ptr<ISystemClient> {
	if (! std::filesystem::exists( absolute ) )
		return {};

	return std::make_shared<CPlainSystemClient>( id, std::move( absolute ), path );
}
auto CPlainSystemClient::GetNativePath() const -> const char* {
	return this->m_szNativePath;
}
auto CPlainSystemClient::GetNativeAbsolutePath() const -> const char* {
	return this->m_szNativeAbsolutePath.c_str();
}
auto CPlainSystemClient::GetIdentifier() const -> int {
	return this->m_iId;
}
auto CPlainSystemClient::Shutdown() -> void { }

// FS interaction
auto CPlainSystemClient::Flush( FileHandle_t file ) -> bool {
	AssertMsg( file, "Was given a `NULL` file handle!" );
	return fflush( reinterpret_cast<FILE*>( file ) ) == 0;
}
auto CPlainSystemClient::Walk( uint16_t nwname, const char* wname ) -> void {

}
auto CPlainSystemClient::Open( const char* path, const char* mode ) -> FileHandle_t {
	AssertMsg( path, "Was given a `NULL` file path!" );
	AssertMsg( mode, "Was given a `NULL` open mode!" );
	return reinterpret_cast<FileHandle_t>( fopen( path, mode ) );
}
auto CPlainSystemClient::Close( FileHandle_t file ) -> void {

}
auto CPlainSystemClient::Create( const char* name, dirmode_t perm, openmode_t mode ) -> FileHandle_t {
	return nullptr;
}
auto CPlainSystemClient::Read( FileHandle_t file, void* buffer, uint32_t count ) -> uint32_t {
	AssertMsg( file, "Was given a `NULL` file handle!" );
	AssertMsg( buffer, "Was given a `NULL` buffer ptr!" );
	return fread( buffer, 1, count, reinterpret_cast<FILE*>( file ) ) == 0;
}
auto CPlainSystemClient::Write( FileHandle_t file, void const* buffer, uint32_t count ) -> uint32_t {
	AssertMsg( file, "Was given a `NULL` file handle!" );
	AssertMsg( buffer, "Was given a `NULL` buffer ptr!" );
	return fwrite( buffer, 1, count, reinterpret_cast<FILE*>( file ) ) == 0;
}
auto CPlainSystemClient::Remove( FileHandle_t file ) -> void {
}
auto CPlainSystemClient::Stat( FileHandle_t file ) -> void {
}
