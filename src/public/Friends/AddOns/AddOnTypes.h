//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef ADDONTYPES_H
#define ADDONTYPES_H
#pragma once

#if !IsWindows()
  typedef unsigned long long SessionInt64;
#else
  typedef unsigned __int64 SessionInt64;
#endif

#endif // ADDONTYPES_H

