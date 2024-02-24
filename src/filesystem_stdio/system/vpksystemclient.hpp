//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#pragma once

#include "isystemclient.hpp"

class CVPKSystemClient : public ISystemClient {
public:
	auto GetNativePath() -> const char* override;
	auto Shutdown() -> const char* override;
private:
	const char* m_szNativePath;
};