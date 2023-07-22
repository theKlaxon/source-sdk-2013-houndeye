//
// Created by ENDERZOMBI102 on 22/07/2023.
//

#include <cstdio>
#include <cstdlib>

#include <windef.h>
#include <libloaderapi.h>

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )  {
	auto launcherHandle = LoadLibraryA( "%s\\bin\\launcherHandle.dll" );
	auto proc = GetProcAddress( launcherHandle, "LauncherMain" );

	code = GetModuleFileNameA( hInstance, thisPath, 260 );
	if (code == 0) {
		MessageBoxA((HWND)NULL,"Failed calling GetModuleFileName","Launcher Error",0);
		return 0;
	}
	FUN_004020e8((char *)libraryPath,0x1000,(byte *)"PATH=%s\\bin\\;%s");
	local_509 = 0;
	FUN_00401e8b(libraryPath);
	FUN_004020e8((char *)libraryPath,0x1000,(byte *)"%s\\bin\\launcher.dll");
	local_509 = 0;
	hModule = LoadLibraryExA((LPCSTR)libraryPath,(HANDLE)NULL,8);
	if (hModule == (HMODULE)NULL) {
		lpBuffer = &hInstance;
		dwLanguageId = LANG_USER_DEFAULT;
		Arguments = hModule;
		code = GetLastError();
		FormatMessageA(0x1300,(LPCVOID)0x0,code,dwLanguageId,(LPSTR)lpBuffer,(DWORD)hModule,(va_list *)Arguments);
		FUN_004020e8(local_508,0x400,(byte *)"Failed to load the launcher DLL:\n\n%s");
		local_109 = 0;
		MessageBoxA((HWND)NULL,local_508,"Launcher Error",0);
		LocalFree(hInstance);
		return 0;
	}
	pFVar1 = GetProcAddress(hModule,"LauncherMain");
	iVar2 = (*pFVar1)(hInstance,hPrevInstance,lpCmdLine,nShowCmd);
	return iVar2;
}
