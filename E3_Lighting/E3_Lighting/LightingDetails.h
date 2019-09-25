#pragma once
#include "DXF.h"
struct LightingDetails
{
	XMFLOAT4 ambientColor;
	float* ambientColorF;
	XMFLOAT4 diffuseColor;
	float* diffuseColorF;

	XMFLOAT3 direction;
	float* directionF;

	XMFLOAT3 position;
	float* positionF;

	float pointLightRange = 5000.f;
	float attenuationConstant = 0.f;
	float attenuationLinear = 0.001f;
	float attenuationExponential = 0.f;
};