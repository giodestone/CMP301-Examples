#pragma once
#include "DXF.h"
#include "ExtraShaderParams.h"

class DynamicTriTesselationShader :
	public BaseShader
{
	struct DynamicCamTesselBufferType
	{
		XMFLOAT3 cameraPos; // where the camera is
		float tessFactorModifier; // how much to tessellate
		float maxDistance; // max distance to tessellate at
		float maxTessFactor; // max tessellation factor
		XMFLOAT2 padding;
	};

public:

	DynamicTriTesselationShader(ID3D11Device* device, HWND hwnd);
	~DynamicTriTesselationShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ExtraShaderParams extraShaderParams);

private:
	void initShader(const wchar_t* vsFilename, const wchar_t* psFilename);
	void initShader(const wchar_t* vsFilename, const wchar_t* hsFilename, const wchar_t* dsFilename, const wchar_t* psFilename);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* tesselationBuffer;
};

