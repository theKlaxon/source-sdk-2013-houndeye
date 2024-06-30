//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#include "plainsystemclient.hpp"

#include <cstdio>
#include <fcntl.h>
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
auto CPlainSystemClient::Flush( const FileDescriptor* desc ) -> bool {
	AssertFatalMsg( desc, "Was given a `NULL` file handle!" );
	return fflush( reinterpret_cast<FILE*>( desc->m_Handle ) ) == 0;
}
auto CPlainSystemClient::Walk( uint16_t nwname, const char* wname ) -> void {

}
auto CPlainSystemClient::Open( const char* path, openmode::Type mode ) -> FileDescriptor* {
	AssertFatalMsg( path, "Was given a `NULL` file path!" );
	AssertFatalMsg( mode, "Was given an empty open mode!" );

	int32_t mode2{0};

	if ( (mode & openmode::Read) != 0 )
		mode2 |= O_RDONLY;
	if ( (mode & openmode::Write) != 0 )
		mode2 |= O_WRONLY;
	if ( (mode & openmode::ReadWrite) != 0 )
		mode2 |= O_RDWR;
	if ( (mode & openmode::Trunc) != 0 )
		mode2 |= O_TRUNC;
	if ( (mode & openmode::Close) != 0 )
		mode2 |= O_CLOEXEC;

	int file{ open( path, static_cast<int>( mode ) ) };
	auto desc{ FileDescriptor::Make() };
	desc->m_Handle = file;
	return desc;
}
auto CPlainSystemClient::Close( const FileDescriptor* desc ) -> void {

}
auto CPlainSystemClient::Create( const char* path, dirmode_t perm, openmode::Type mode ) -> FileDescriptor* {
	return nullptr;
}
auto CPlainSystemClient::Read( const FileDescriptor* desc, uint64_t offset, void* buffer, uint32_t count ) -> uint32_t {
	AssertFatalMsg( desc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( buffer, "Was given a `NULL` buffer ptr!" );
	pread64( reinterpret_cast<intptr_t>( desc ), buffer, count, offset );
	return fread( buffer, 1, count, reinterpret_cast<FILE*>( desc ) ) == 0;
}
auto CPlainSystemClient::Write( const FileDescriptor* desc, uint64_t offset, void const* buffer, uint32_t count ) -> uint32_t {
	AssertFatalMsg( desc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( buffer, "Was given a `NULL` buffer ptr!" );
	return fwrite( buffer, 1, count, reinterpret_cast<FILE*>( desc ) ) == 0;
}
auto CPlainSystemClient::Remove( const FileDescriptor* desc ) -> void {
}
auto CPlainSystemClient::Stat( const FileDescriptor* desc ) -> void {
}
