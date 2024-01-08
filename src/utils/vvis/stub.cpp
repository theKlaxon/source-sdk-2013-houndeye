//
// Created by ENDERZOMBI102 on 1/7/24.
//
// Description:
//     This file is here to aid in building while on linux, where we don't have
//     all the necessary functions and libraries available.
//

void SetupToolsMinidumpHandler( void (*)( unsigned long, void* ) ) {}
void VMPI_ExceptionFilter( unsigned long, void* ) {}
void SetupDefaultToolsMinidumpHandler() {}
bool g_bMPIMaster{ false };
bool g_bUseMPI{ false };
bool g_bVMPIEarlyExit{ false };
void RunMPIPortalFlow() {}
void RunMPIBasePortalVis() {}
void VVIS_SetupMPI( int&, char**& ) {}
void EnableFullMinidumps( bool ) {}
unsigned char* LZMA_Compress( unsigned char*, unsigned int, unsigned int* ) { return nullptr; }
bool LZMA_Uncompress( unsigned char* pInput, unsigned char** ppOutput, unsigned int* pOutputSize ) { return false; }
bool LZMA_IsCompressed( unsigned char* pInput ) { return false; }
unsigned int LZMA_GetActualSize( unsigned char* pInput ) { return 0; }
