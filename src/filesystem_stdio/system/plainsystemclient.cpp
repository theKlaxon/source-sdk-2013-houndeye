//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#include "plainsystemclient.hpp"

#include <dirent.h>
#include <fcntl.h>
#include <filesystem>
#include <sys/stat.h>
#include <unistd.h>
#include <utility>

#include "dbg.h"

CPlainSystemClient::CPlainSystemClient( int id, std::string absolute, const char* path )
	: m_iId( id ), m_szNativePath( path ), m_szNativeAbsolutePath( std::move( absolute ) ) {}
auto CPlainSystemClient::Open( int id, const std::string& absolute, const char* path ) -> std::shared_ptr<ISystemClient> {
	if ( !std::filesystem::exists( absolute ) )
		return {};
	return std::make_shared<CPlainSystemClient>( id, absolute, path );
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
auto CPlainSystemClient::Shutdown() -> void {}

// FS interaction
auto CPlainSystemClient::Flush( const FileDescriptor* desc ) -> bool {
	AssertFatalMsg( desc, "Was given a `NULL` file handle!" );

	AssertUnreachable();
	return {};
}
auto CPlainSystemClient::Walk( const FileDescriptor* desc, const WalkEntry*& walkable ) -> void {

	getdents64( static_cast<int>( desc->m_Handle ), , );
}
auto CPlainSystemClient::Open( const char* path, OpenMode mode ) -> FileDescriptor* {
	AssertFatalMsg( path, "Was given a `NULL` file path!" );
	AssertFatalMsg( mode, "Was given an empty open mode!" );

#if IsLinux()
	int32_t mode2{ 0 };
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
auto CPlainSystemClient::Create( const char* path, FileType type, OpenMode mode ) -> FileDescriptor* {
	return nullptr;
}
auto CPlainSystemClient::Read( const FileDescriptor* desc, void* buffer, uint32_t count ) -> int32_t {
	AssertFatalMsg( desc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( buffer, "Was given a `NULL` buffer ptr!" );

	return pread64( static_cast<int>( desc->m_Handle ), buffer, count, static_cast<__off64_t>( desc->m_Offset ) );
}
auto CPlainSystemClient::Write( const FileDescriptor* desc, void const* buffer, uint32_t count ) -> int32_t {
	AssertFatalMsg( desc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( buffer, "Was given a `NULL` buffer ptr!" );

	return pwrite64( static_cast<int>( desc->m_Handle ), buffer, count, static_cast<__off64_t>( desc->m_Offset ) );
}
auto CPlainSystemClient::Remove( const FileDescriptor* desc ) -> void {
}

auto CPlainSystemClient::Stat( const FileDescriptor* desc ) -> std::optional<StatData> {
	// get stat data
	struct stat64 it {};
	if ( fstat64( static_cast<int>( desc->m_Handle ), &it ) != 0 )
		return {}; // error happened :/

	// find the type
	FileType fileType{ FileType::Unknown };
	if ( S_ISDIR( it.st_mode ) )
		fileType = FileType::Directory;
	else if ( S_ISREG( it.st_mode ) )
		fileType = FileType::Regular;
	else if ( S_ISSOCK( it.st_mode ) )
		fileType = FileType::Socket;

	// return value
	return { StatData{ fileType, it.st_atim.tv_nsec, it.st_mtim.tv_nsec, it.st_size } };
}

void x() {
	const StatData data{};
	const StatData* ptrToConst{ &data };
	const StatData*& ptrToPtrToConst{ ptrToConst };

	ptrToPtrToConst = ptrToConst;
	*ptrToPtrToConst = {};
}
