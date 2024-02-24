//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#include "vpksystemclient.hpp"

auto CVPKSystemClient::GetNativePath() -> const char* {
	return this->m_szNativePath;
}

auto CVPKSystemClient::Shutdown() -> const char* {
	return nullptr;
}
