//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#include "packsystemclient.hpp"
#include <utility>

CPackSystemClient::CPackSystemClient( int32 pId, const char* pPath, std::unique_ptr<vpkpp::PackFile>&& pPack ) {
	this->m_iId = pId;
	this->m_szNativePath = pPath;
	this->m_PackFile = std::move( pPack );
}
auto CPackSystemClient::Open( int32 pId, const std::string& pAbsolute, const char* pPath ) -> std::shared_ptr<ISystemClient> {
	auto pack{ vpkpp::PackFile::open( pAbsolute, {} ) };
	if (! pack ) {
		return {};
	}

	switch ( pack->getType() ) {
		case vpkpp::PackFileType::VPK:
		case vpkpp::PackFileType::BSP:
			return std::make_shared<CPackSystemClient>( pId, pPath, std::move( pack ) );
		default:
			return {};
	}
}
auto CPackSystemClient::GetNativePath() const -> const char* {
	return this->m_szNativePath;
}
auto CPackSystemClient::GetNativeAbsolutePath() const -> const char* {
	return this->m_PackFile->getFilepath().data();
}
auto CPackSystemClient::GetIdentifier() const -> int32 {
	return this->m_iId;
}
auto CPackSystemClient::GetType() const -> const char* {
	return "pack";
}
auto CPackSystemClient::Shutdown() -> void { }

// FS interaction
auto CPackSystemClient::Flush( const FileDescriptor* desc ) -> bool {
	AssertFatalMsg( false, "Not supported!!" );
	std::unreachable();
}
auto CPackSystemClient::Walk( const FileDescriptor* pDesc, const WalkEntry*& pEntry ) -> void {

}
auto CPackSystemClient::Open( const char* pPath, OpenMode pMode ) -> FileDescriptor* {
	AssertFatalMsg( pPath, "Was given a `NULL` file path!" );
	AssertFatalMsg( pMode, "Was given an empty open mode!" );

	return nullptr;
}
auto CPackSystemClient::Close( const FileDescriptor* desc ) -> void { }
auto CPackSystemClient::Create( const char* pPath, FileType pType, OpenMode pMode ) -> FileDescriptor* {
	AssertFatalMsg( false, "Not supported!!" );
	std::unreachable();
}
auto CPackSystemClient::Read( const FileDescriptor* desc, void* buffer, uint32 count ) -> int32 {
	AssertFatalMsg( desc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( buffer, "Was given a `NULL` buffer ptr!" );

	return pread64( static_cast<int>( desc->m_Handle ), buffer, count, desc->m_Offset );
}
auto CPackSystemClient::Write( const FileDescriptor* desc, void const* buffer, uint32 count ) -> int32 {
	AssertFatalMsg( false, "Not supported!!" );
	std::unreachable();
}
auto CPackSystemClient::Remove( const FileDescriptor* desc ) -> void {
	AssertFatalMsg( false, "Not supported!!" );
	std::unreachable();
}
auto CPackSystemClient::Stat( const FileDescriptor* pDesc ) -> std::optional<StatData> {
	return {};
}
