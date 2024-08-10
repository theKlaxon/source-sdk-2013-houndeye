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

#include "strtools.h"
#include "dbg.h"

CPlainSystemClient::CPlainSystemClient( int pId, std::string pAbsolute, const char* pPath )
	: m_iId( pId ), m_szNativePath( pPath ), m_szNativeAbsolutePath( std::move( pAbsolute ) ) { }
auto CPlainSystemClient::Open( int pId, const std::string& pAbsolute, const char* pPath ) -> std::shared_ptr<ISystemClient> {
	if (! std::filesystem::is_directory( pAbsolute ) ) {
		return { };
	}
	return std::make_shared<CPlainSystemClient>( pId, pAbsolute, pPath );
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
auto CPlainSystemClient::Open( const char* pPath, OpenMode pMode ) -> FileDescriptor* {
	AssertFatalMsg( pPath, "Was given a `NULL` file path!" );
	AssertFatalMsg( pMode, "Was given an empty open mode!" );

	// create full path
	char buffer[1024];
	V_ComposeFileName( m_szNativeAbsolutePath.c_str(), pPath, buffer, 1024 );

	#if IsLinux()
		int32_t mode2{ 0 };
		// read/write combos
		if ( pMode.read && !pMode.write ) {
			mode2 |= O_RDONLY;
		}
		if ( pMode.write && !pMode.read ) {
			mode2 |= O_WRONLY;
		}
		if ( pMode.read && pMode.write ) {
			mode2 |= O_RDWR;
		}

		// other modes
		if ( pMode.truncate ) {
			mode2 |= O_TRUNC;
		}
		if ( pMode.close ) {
			mode2 |= O_CLOEXEC;
		}
		if ( pMode.append ) {
			mode2 |= O_APPEND;
		}

		int file{ open( buffer, mode2 ) };

		// Check if we got a valid handle, TODO: Actual error handling
		if ( file == -1 ) {
			return nullptr;
		}
	#elif IsWindows()
		#error "Not implemented yet"
	#endif

	auto desc{ FileDescriptor::Make() };
	desc->m_Handle = file;
	return desc;
}
auto CPlainSystemClient::Read( const FileDescriptor* pDesc, void* pBuffer, uint32_t pCount ) -> int32_t {
	AssertFatalMsg( pDesc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( pBuffer, "Was given a `NULL` buffer ptr!" );

	return pread64( static_cast<int>( pDesc->m_Handle ), pBuffer, pCount, static_cast<__off64_t>( pDesc->m_Offset ) );
}
auto CPlainSystemClient::Write( const FileDescriptor* pDesc, const void* pBuffer, uint32_t pCount ) -> int32_t {
	AssertFatalMsg( pDesc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( pBuffer, "Was given a `NULL` buffer ptr!" );

	return pwrite64( static_cast<int>( pDesc->m_Handle ), pBuffer, pCount, static_cast<__off64_t>( pDesc->m_Offset ) );
}
auto CPlainSystemClient::Flush( const FileDescriptor* pDesc ) -> bool {
	AssertFatalMsg( pDesc, "Was given a `NULL` file handle!" );

	AssertUnreachable();
	return {};
}
auto CPlainSystemClient::Close( const FileDescriptor* pDesc ) -> void { }

auto CPlainSystemClient::Walk( const FileDescriptor* pDesc, const WalkEntry*& pEntry ) -> void {
//	getdents64( static_cast<int>( pDesc->m_Handle ), , );
}
auto CPlainSystemClient::Create( const char* pPath, FileType pType, OpenMode pMode ) -> FileDescriptor* { return {}; }
auto CPlainSystemClient::Remove( const FileDescriptor* pDesc ) -> void { }
auto CPlainSystemClient::Stat( const FileDescriptor* pDesc ) -> std::optional<StatData> {
	// get stat data
	struct stat64 it {};
	if ( fstat64( static_cast<int>( pDesc->m_Handle ), &it ) != 0 ) {
		return {};// error happened :/
	}

		// find the type
	FileType fileType{ FileType::Unknown };
	if ( S_ISDIR( it.st_mode ) ) {
		fileType = FileType::Directory;
	} else if ( S_ISREG( it.st_mode ) ) {
		fileType = FileType::Regular;
	} else if ( S_ISSOCK( it.st_mode ) ) {
		fileType = FileType::Socket;
	}

	// return value
	return { StatData{ fileType, it.st_atim.tv_nsec, it.st_mtim.tv_nsec, it.st_size } };
}
