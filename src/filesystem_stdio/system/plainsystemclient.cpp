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
auto CPlainSystemClient::Open( const char* path, OpenMode mode ) -> FileDescriptor* {
	AssertFatalMsg( path, "Was given a `NULL` file path!" );
	AssertFatalMsg( mode, "Was given an empty open mode!" );

	#if IsLinux()
		int32_t mode2{0};
		// read/write combos
		if ( mode.read && !mode.write )
			mode2 |= O_RDONLY;
		if ( mode.write && !mode.read )
			mode2 |= O_WRONLY;
		if ( mode.read && mode.write )
			mode2 |= O_RDWR;

		// other modes
		if ( mode.truncate )
			mode2 |= O_TRUNC;
		if ( mode.close )
			mode2 |= O_CLOEXEC;
		if ( mode.append )
			mode2 |= O_APPEND;

		int file{ open( path, mode2 ) };

		// Check if we got a valid handle, TODO: Actual error handling
		if ( file == -1 )
			return nullptr;
	#elif IsWindows()
		#error "Not implemented yet"
	#endif

	auto desc{ FileDescriptor::Make() };
	desc->m_Handle = file;
	return desc;
}
auto CPlainSystemClient::Close( const FileDescriptor* desc ) -> void {

}
auto CPlainSystemClient::Create( const char* path, dirmode_t perm, OpenMode mode ) -> FileDescriptor* {
	return nullptr;
}
auto CPlainSystemClient::Read( const FileDescriptor* desc, void* buffer, uint32_t count ) -> int32_t {
	AssertFatalMsg( desc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( buffer, "Was given a `NULL` buffer ptr!" );

	return pread64( static_cast<int>( desc->m_Handle ), buffer, count, desc->m_Offset );
}
auto CPlainSystemClient::Write( const FileDescriptor* desc, void const* buffer, uint32_t count ) -> int32_t {
	AssertFatalMsg( desc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( buffer, "Was given a `NULL` buffer ptr!" );

	return pwrite64( static_cast<int>( desc->m_Handle ), buffer, count, desc->m_Offset );
}
auto CPlainSystemClient::Remove( const FileDescriptor* desc ) -> void {
}
auto CPlainSystemClient::Stat( const FileDescriptor* desc ) -> void {
}

