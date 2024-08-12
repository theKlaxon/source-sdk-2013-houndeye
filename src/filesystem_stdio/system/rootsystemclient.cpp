//
// Created by ENDERZOMBI102 on 10/08/2024.
//
#include "rootsystemclient.hpp"
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>


auto CRootSystemClient::GetNativePath() const -> const char* {
	#if IsPosix()
		return "/";
	#else
		return nullptr;
	#endif
}
auto CRootSystemClient::GetNativeAbsolutePath() const -> const char* {
	#if IsPosix()
		return "/";
	#else
		return nullptr;
	#endif
}
auto CRootSystemClient::GetIdentifier() const -> int32 {
	return 0;
}
auto CRootSystemClient::GetType() const -> const char* {
	return "root";
}
auto CRootSystemClient::Shutdown() -> void { }

// FS interaction
auto CRootSystemClient::Open( const char* pPath, OpenMode pMode ) -> FileDescriptor* {
	AssertFatalMsg( pPath, "Was given a `NULL` file path!" );
	AssertFatalMsg( pMode, "Was given an empty open mode!" );

	#if IsLinux()
		int32 mode2{ 0 };
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

		int file{ open( pPath, mode2 ) };

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
auto CRootSystemClient::Read( const FileDescriptor* pDesc, void* pBuffer, uint32 pCount ) -> int32 {
	AssertFatalMsg( pDesc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( pBuffer, "Was given a `NULL` buffer ptr!" );

	return pread64( static_cast<int>( pDesc->m_Handle ), pBuffer, pCount, static_cast<__off64_t>( pDesc->m_Offset ) );
}
auto CRootSystemClient::Write( const FileDescriptor* pDesc, const void* pBuffer, uint32 pCount ) -> int32 {
	AssertFatalMsg( pDesc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( pBuffer, "Was given a `NULL` buffer ptr!" );

	return pwrite64( static_cast<int>( pDesc->m_Handle ), pBuffer, pCount, static_cast<__off64_t>( pDesc->m_Offset ) );
}
auto CRootSystemClient::Flush( const FileDescriptor* pDesc ) -> bool {
	AssertFatalMsg( pDesc, "Was given a `NULL` file handle!" );

	#if IsWindows()
		AssertUnreachable();
	#endif

	return true;
}
auto CRootSystemClient::Close( const FileDescriptor* pDesc ) -> void { }

auto CRootSystemClient::Walk( const FileDescriptor* pDesc, const WalkEntry*& pEntry ) -> void {
	//	getdents64( static_cast<int>( pDesc->m_Handle ), , );
}
auto CRootSystemClient::Create( const char* pPath, FileType pType, OpenMode pMode ) -> FileDescriptor* { return {}; }
auto CRootSystemClient::Remove( const FileDescriptor* pDesc ) -> void { }
auto CRootSystemClient::Stat( const FileDescriptor* pDesc ) -> std::optional<StatData> {
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
