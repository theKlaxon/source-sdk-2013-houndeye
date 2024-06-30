//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#include "packsystemclient.hpp"


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
auto CPackSystemClient::Flush( FileHandle_t file ) -> bool {
	return false;
}
auto CPackSystemClient::Walk( uint16_t nwname, const char* wname ) -> void {
}
auto CPackSystemClient::Open( const char* path, openmode::Type mode ) -> FileHandle_t {
	{ this->m_PackFile->findEntry( path,  ) };
	return nullptr;
}
auto CPackSystemClient::Close( FileHandle_t file ) -> void {

}
auto CPackSystemClient::Create( const char* path, dirmode_t perm, openmode::Type mode ) -> FileHandle_t {
	return nullptr;
}
auto CPackSystemClient::Read( FileHandle_t file, uint64_t offset, void* buffer, uint32_t count ) -> uint32_t {
	return 0;
}
auto CPackSystemClient::Write( FileHandle_t file, uint64_t offset, const void* buffer, uint32_t count ) -> uint32_t {
	return 0;
}
auto CPackSystemClient::Remove( FileHandle_t file ) -> void {
}
auto CPackSystemClient::Stat( FileHandle_t file ) -> void {
}
