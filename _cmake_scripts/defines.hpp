//
// Created by ENDERZOMBI102 on 27/07/2024.
//
#pragma once

#if IsWindows == 1
	#undef IsWindows
	#define IsWindows() true
#else
	#undef IsWindows
	#define IsWindows() false
#endif

#if IsPosix == 1
	#undef IsPosix
	#define IsPosix() true
#else
	#undef IsPosix
	#define IsPosix() false
#endif

#if IsLinux == 1
	#undef IsLinux
	#define IsLinux() true
#else
	#undef IsLinux
	#define IsLinux() false
#endif

#if IsRetail == 1
	#undef IsRetail
	#define IsRetail() true
#else
	#undef IsRetail
	#define IsRetail() false
#endif

#if IsDebug == 1
	#undef IsDebug
	#define IsDebug() true
#else
	#undef IsDebug
	#define IsDebug() false
#endif

#if IsRelease == 1
	#undef IsRelease
	#define IsRelease() true
#else
	#undef IsRelease
	#define IsRelease() false
#endif