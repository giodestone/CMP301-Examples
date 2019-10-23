#pragma once

#include "DXF.h"

struct ExtraShaderParams
{
	XMFLOAT3 playerPos;
	XMFLOAT4 colorOfDot = XMFLOAT4(1.f, 0.f, 1.f, 1.f);
	float radiusOfDot = 250.f;
	bool drawPlayer = false;

	XMMATRIX worldAtTopDown;
	XMMATRIX viewAtTopDown;
	XMMATRIX projectionAtTopDown;

	XMMATRIX orthoViewMatrix;
	XMMATRIX orthoMatrix;
};