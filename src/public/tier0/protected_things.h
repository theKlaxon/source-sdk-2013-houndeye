//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#pragma once


// This header tries to prevent people from using potentially dangerous functions
// (like the notorious non-null-terminating strncpy).
//
// This header should be included by ALL of our source code.

// Eventually, ALL of these should be protected, but one man can only accomplish so much in
// one day AND work on features too!
#if defined( PROTECTED_STRINGS_ENABLE ) && !defined( DISABLE_PROTECTED_STRINGS )
	#if defined( printf )
		#undef printf
	#endif
	#define printf				printf__HEY_YOU__USE_VSTDLIB
	
	#if defined( wprintf )
		#undef wprintf
	#endif
	#define wprintf				wprintf__HEY_YOU__USE_VSTDLIB
	
	#if defined( strcmp )
		#undef strcmp
	#endif
	#define strcmp				strcmp__HEY_YOU__USE_VSTDLIB
	
	#if defined( wcscmp )
		#undef wcscmp
	#endif
	#define wcscmp				wcscmp__HEY_YOU__USE_VSTDLIB
	
	#if defined( strncpy )
		#undef strncpy
	#endif
	#define strncpy				strncpy__HEY_YOU__USE_VSTDLIB
	
	#if defined( wcsncpy )
		#undef wcsncpy
	#endif
	#define wcsncpy				wcsncpy__HEY_YOU__USE_VSTDLIB
	
	#if defined( strlen )
		#undef strlen
	#endif
	#define strlen				strlen__HEY_YOU__USE_VSTDLIB
	
	#if defined( wcslen )
		#undef wcslen
	#endif
	#define wcslen				wcslen__HEY_YOU__USE_VSTDLIB
	
	#if defined( Q_strlen )
		#undef Q_strlen
	#endif
	#define Q_strlen			Q_strlen__HEY_YOU__USE_VSTDLIB
	
	#if defined( _snprintf )
		#undef _snprintf
	#endif
	#define _snprintf			snprintf__HEY_YOU__USE_VSTDLIB
	
	#if defined( _snwprintf )
		#undef _snwprintf
	#endif
	#define _snwprintf			snwprintf__HEY_YOU__USE_VSTDLIB
	
	#if defined( sprintf )
		#undef sprintf
	#endif
	#define sprintf				sprintf__HEY_YOU__USE_VSTDLIB

	#if defined( swprintf )
		#undef swprintf
	#endif
	#define swprintf			swprintf__HEY_YOU__USE_VSTDLIB

	#if defined( vsprintf )
		#undef vsprintf
	#endif
	#define vsprintf			vsprintf__HEY_YOU__USE_VSTDLIB

	#if defined( vswprintf )
		#undef vswprintf
	#endif
	#define vswprintf			vswprintf__HEY_YOU__USE_VSTDLIB

	#if defined( _vsnprintf )
		#undef _vsnprintf
	#endif
	#define _vsnprintf			vsnprintf__HEY_YOU__USE_VSTDLIB
	
	#if defined( _vsnwprintf )
		#undef _vsnwprintf
	#endif
	#define _vsnwprintf			vsnwprintf__HEY_YOU__USE_VSTDLIB
	
	#if defined( strcat )
		#undef strcat
	#endif
	#define strcat				strcat__HEY_YOU__USE_VSTDLIB

	#if defined( wcscat )
		#undef wcscat
	#endif
	#define wcscat				wcscat__HEY_YOU__USE_VSTDLIB

	#if defined( strncat )
		#undef strncat
	#endif
	#define strncat				strncat__HEY_YOU__USE_VSTDLIB

	#if defined( wcsncat )
		#undef wcsncat
	#endif
	#define wcsncat				wcsncat__HEY_YOU__USE_VSTDLIB
#endif

