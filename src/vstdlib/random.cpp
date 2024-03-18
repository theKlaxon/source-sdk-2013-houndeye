#include "vstdlib/random.h"

CUniformRandomStream g_uniformRandomStream;

void RandomSeed (int iSeed ) {
    g_uniformRandomStream.SetSeed( iSeed );
}

float RandomFloat( float flMinVal, float flMaxVal ) {
    g_uniformRandomStream.RandomFloat( flMinVal, flMaxVal );
}

int RandomInt( int iMinVal, int iMaxVal ) {
    g_uniformRandomStream.RandomInt( iMinVal, iMaxVal );
}

CUniformRandomStream::