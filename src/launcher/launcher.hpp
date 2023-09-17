//
// Created by ENDERZOMBI102 on 11/09/2023.
//

#pragma once

#include "appframework/IAppSystemGroup.h"


class CLauncher : public CSteamAppSystemGroup {
public:
	// Methods of IApplication
	bool Create() override;
	bool PreInit() override;
	int Main() override;
	void PostShutdown() override;
	void Destroy() override;
private:
	// Sets up the search paths
	bool SetupSearchPaths();
};

