//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "interface.h"

#if !defined( WIN32_LEAN_AND_MEAN )
	#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

class IUnicodeWindows : public IBaseInterface {
public:
	virtual LRESULT DefWindowProcW(
		HWND hWnd,
		UINT Msg,
		WPARAM wParam,
		LPARAM lParam ) = 0;

	virtual HWND CreateWindowExW(
		DWORD dwExStyle,
		LPCWSTR lpClassName,
		LPCWSTR lpWindowName,
		DWORD dwStyle,
		int x,
		int y,
		int nWidth,
		int nHeight,
		HWND hWndParent,
		HMENU hMenu,
		HINSTANCE hInstance,
		LPVOID lpParam ) = 0;

	virtual ATOM RegisterClassW(
		CONST WNDCLASSW* lpWndClass ) = 0;

	virtual BOOL UnregisterClassW(
		LPCWSTR lpClassName,
		HINSTANCE hInstance ) = 0;
};

#define VENGINE_UNICODEINTERFACE_VERSION "VENGINEUNICODE001"
