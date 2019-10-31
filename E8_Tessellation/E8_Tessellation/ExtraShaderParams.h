#pragma once
#include "DXF.h"
struct ExtraShaderParams
{
	//for quad and tri shader
	float tessFactor;
	//for tri shader
	float tessFactorTop;
	float tessFactorBL;
	float tessFactorBR;

	//for quad shader
	float tessFactor2;
	float tessFactorEdgeTLQuad;
	float tessFactorEdgeTRQuad;
	float tessFactorEdgeBLQuad;
	float tessFactorEdgeBRQuad;

	bool shouldWave = false;
	float totalTime = 0.f;

	//for dynamic shader
	XMFLOAT3 cameraPos;
	float tessFactorModifier;
	float maxDistance;
	float maxTessFactor;
};