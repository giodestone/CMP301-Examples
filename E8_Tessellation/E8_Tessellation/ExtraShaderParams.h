#pragma once

struct ExtraShaderParams
{
	float tessFactor;
	float tessFactorTop;
	float tessFactorBL;
	float tessFactorBR;

	float tessFactor2;
	float tessFactorEdgeTLQuad;
	float tessFactorEdgeTRQuad;
	float tessFactorEdgeBLQuad;
	float tessFactorEdgeBRQuad;
};