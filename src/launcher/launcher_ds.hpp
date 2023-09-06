//
// Created by ENDERZOMBI102 on 22/07/2023.
//

#pragma once

#include "appframework/IAppSystemGroup.h"


class CLauncherDS : public CSteamAppSystemGroup {
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
