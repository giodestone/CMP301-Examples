#pragma once
#include "DXF.h"

struct ExtraShaderParams
{
	float time = 0.f;

	float amplitudeSin = 1.f;
	float amplitudeCos = 1.f;
	float speedSin = 1.f;
	float speedCos = 1.f;
	
	ID3D11ShaderResourceView* heightmap;
};