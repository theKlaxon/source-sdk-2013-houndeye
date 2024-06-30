//
// Created by ENDERZOMBI102 on 30/06/2024.
//
#include "isystemclient.hpp"


static CMemoryPoolMT g_DescriptorArena{ sizeof( FileDescriptor ), 10, UTLMEMORYPOOL_GROW_SLOW, "FileSystem|DescriptorArena" };

auto FileDescriptor::Make() -> FileDescriptor* {
	return static_cast<FileDescriptor*>( g_DescriptorArena.AllocZero( sizeof( FileDescriptor ) ) );
}

auto FileDescriptor::Free( FileDescriptor* desc ) -> void {
	return g_DescriptorArena.Free( desc );
}

auto FileDescriptor::CleanupArena() -> void {
	return g_DescriptorArena.Clear();
}
