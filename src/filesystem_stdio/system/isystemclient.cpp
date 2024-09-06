//
// Created by ENDERZOMBI102 on 30/06/2024.
//
#include "isystemclient.hpp"
#include "tier1/mempool.h"
#include "packsystemclient.hpp"
#include "plainsystemclient.hpp"
#include <filesystem>
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static CMemoryPoolMT g_DescriptorArena{ sizeof( FileDescriptor ), 10, UTLMEMORYPOOL_GROW_SLOW, "FileSystem|DescriptorArena" };

auto FileDescriptor::Make() -> FileDescriptor* {
	auto desc{ static_cast<FileDescriptor*>( g_DescriptorArena.AllocZero( sizeof( FileDescriptor ) ) ) };
	return std::construct_at( desc );
}

auto FileDescriptor::Free( FileDescriptor* desc ) -> void {
	g_DescriptorArena.Free( desc );
}

auto FileDescriptor::CleanupArena() -> void {
	g_DescriptorArena.Clear();
}

auto CreateSystemClient( int pId, const char* pAbsolute, const char* pPath ) -> ISystemClient* {
	if ( std::filesystem::is_directory( pAbsolute ) ) {
		return new CPlainSystemClient( pId, pAbsolute, pPath );
	}

	if ( V_strcmp( V_GetFileExtension( pPath ), "vpk" ) == 0 || V_strcmp( V_GetFileExtension( pPath ), "bsp" ) == 0 ) {
		return new CPackSystemClient( pId, pAbsolute, pPath );
	}

	return {};
}
