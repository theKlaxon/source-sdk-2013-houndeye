//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#include "plainsystemclient.hpp"

auto CPlainSystemClient::GetNativePath() -> const char* {
	return this->m_szNativePath;
}

auto CPlainSystemClient::Shutdown() -> const char* {
	return nullptr;
}
