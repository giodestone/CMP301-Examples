#pragma once
#include "DXF.h"

/* Contains information for the data that will be later passed into the 
	shader. Some of it needs to go into the light class first, if avaliable.
	*/
struct LightingDetails
{
	LightingDetails() :
		ambientColorF(nullptr), 
		diffuseColorF(nullptr), 
		positionF(nullptr)
	{
		// Initialising values for IMGui which needs points to floats
		ambientColorF = new float[4];
		diffuseColorF = new float[4];
		positionF = new float[3];
	}

	~LightingDetails()
	{
		delete[] ambientColorF;
		ambientColorF = nullptr;

		delete[] diffuseColorF;
		diffuseColorF = nullptr;

		delete[] positionF;
		positionF = nullptr;
	}

	/* Set the float pointers with the values from the matrices (for
	setup before ImGui uses them - to avoid garbled data)*/
	void UpdatePointersFromMatrices()
	{
		ambientColorF[0] = ambientColor.x;
		ambientColorF[1] = ambientColor.y;
		ambientColorF[2] = ambientColor.z;
		ambientColorF[3] = ambientColor.w;

		diffuseColorF[0] = diffuseColor.x;
		diffuseColorF[1] = diffuseColor.y;
		diffuseColorF[2] = diffuseColor.z;
		diffuseColorF[3] = diffuseColor.w;

		positionF[0] = position.x;
		positionF[1] = position.y;
		positionF[2] = position.z;
	}

	/* Update Matrices from the pointers which have been probably
	modified by ImGui (call every update()/frame()). */
	void UpdateMatricesFromPointers()
	{
		ambientColor.x = ambientColorF[0];
		ambientColor.y = ambientColorF[1];
		ambientColor.z = ambientColorF[2];
		ambientColor.w = ambientColorF[3];

		diffuseColor.x = diffuseColorF[0];
		diffuseColor.y = diffuseColorF[1];
		diffuseColor.z = diffuseColorF[2];
		diffuseColor.w = diffuseColorF[3];

		position.x = positionF[0];
		position.y = positionF[1];
		position.z = positionF[2];
	}

	XMFLOAT4 ambientColor;
	float* ambientColorF;
	XMFLOAT4 diffuseColor;
	float* diffuseColorF;

	XMFLOAT3 direction;
	//float* directionF;

	XMFLOAT3 position;
	float* positionF;

	float range = 5000.f;
	float attenuationConstant = 0.25f;
	float attenuationLinear = 0.001f;
	float attenuationExponential = 0.f;
};