#pragma once

struct ExtraLightParams
{
	float directionalDirection[3];
	float directionalDiffuse[4];

	float attConst = 1.f;
	float attLin = 0.1f;
	float attExp = 0.0001f;
};