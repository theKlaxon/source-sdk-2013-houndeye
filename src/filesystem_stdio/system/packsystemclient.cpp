//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#include "packsystemclient.hpp"
#include <utility>

CPackSystemClient::CPackSystemClient( int id, const char* path, std::unique_ptr<vpkedit::PackFile>&& pack ) {
	this->m_iId = id;
	this->m_szNativePath = path;
	this->m_PackFile = std::move( pack );
}
auto CPackSystemClient::Open( int id, const std::string& absolute, const char* path ) -> std::shared_ptr<ISystemClient> {
	auto pack{ vpkedit::PackFile::open( absolute, {} ) };
	if (! pack )
		return {};

	switch ( pack->getType() ) {
		case vpkedit::PackFileType::VPK:
		case vpkedit::PackFileType::BSP:
			return std::make_shared<CPackSystemClient>( id, path, std::move( pack ) );
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
auto CPackSystemClient::GetIdentifier() const -> int {
	return this->m_iId;
}
auto CPackSystemClient::Shutdown() -> void { }

// FS interaction
auto CPackSystemClient::Flush( const FileDescriptor* desc ) -> bool {
	AssertFatalMsg( false, "Not supported!!" );
	std::unreachable();
}
auto CPackSystemClient::Walk( uint16_t nwname, const char* wname ) -> void {

}
auto CPackSystemClient::Open( const char* path, OpenMode mode ) -> FileDescriptor* {
	AssertFatalMsg( path, "Was given a `NULL` file path!" );
	AssertFatalMsg( mode, "Was given an empty open mode!" );

	return nullptr;
}
auto CPackSystemClient::Close( const FileDescriptor* desc ) -> void { }
auto CPackSystemClient::Create( const char* path, dirmode_t perm, OpenMode mode ) -> FileDescriptor* {
	AssertFatalMsg( false, "Not supported!!" );
	std::unreachable();
}
auto CPackSystemClient::Read( const FileDescriptor* desc, void* buffer, uint32_t count ) -> int32_t {
	AssertFatalMsg( desc, "Was given a `NULL` file handle!" );
	AssertFatalMsg( buffer, "Was given a `NULL` buffer ptr!" );

	return pread64( static_cast<int>( desc->m_Handle ), buffer, count, desc->m_Offset );
}
auto CPackSystemClient::Write( const FileDescriptor* desc, void const* buffer, uint32_t count ) -> int32_t {
	AssertFatalMsg( false, "Not supported!!" );
	std::unreachable();
}
auto CPackSystemClient::Remove( const FileDescriptor* desc ) -> void {
	AssertFatalMsg( false, "Not supported!!" );
	std::unreachable();
}
auto CPackSystemClient::Stat( const FileDescriptor* desc ) -> StatData {
	return {};
}