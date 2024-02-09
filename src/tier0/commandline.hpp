//
// Created by ENDERZOMBI102 on 09/02/2024.
//
#pragma once

#include "tier0/icommandline.h"
#include <string>
#include <vector>

class CCommandLine : public ICommandLine {
public:
	void CreateCmdLine( const char* pCommandLine ) override;
	void CreateCmdLine( int argc, char** argv ) override;
	[[nodiscard]] const char* GetCmdLine() const override;

	// Check whether a particular parameter exists
	const char* CheckParm( const char* psz, const char** ppszValue = nullptr ) const override;
	void RemoveParm( const char* parm ) override;
	void AppendParm( const char* pszParm, const char* pszValues ) override;

	// Returns the argument after the one specified, or the default if not found
	const char* ParmValue( const char* psz, const char* pDefaultVal = nullptr ) const override;
	int ParmValue( const char* psz, int nDefaultVal ) const override;
	float ParmValue( const char* psz, float flDefaultVal ) const override;

	// Gets at particular parameters
	[[nodiscard]] int ParmCount() const override;
	int FindParm( const char* psz ) const override;// Returns 0 if not found.
	[[nodiscard]] const char* GetParm( int nIndex ) const override;

	// copies the string passwed
	void SetParm( int nIndex, char const* pNewParm ) override;

	const char* ParmValueByIndex( int nIndex, const char* pDefaultVal = nullptr ) const override;
private:
	void Reset();
private:
	// Compromise space for speed
	std::string m_sCmdLine{};
	std::vector<std::string> m_Params{};
};

