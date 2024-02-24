//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#pragma once
#include "tier0/platform.h"

abstract_class ISystemClient {
public:
	virtual auto GetNativePath() -> const char* = 0;
	virtual auto Shutdown() -> const char* = 0;
};
