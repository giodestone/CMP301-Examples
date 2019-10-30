// Light shader.h
// Basic single light shader setup
#pragma once

#include "DXF.h"



#include "ExtraShaderParams.h"

using namespace std;
using namespace DirectX;


class TessellationShader : public BaseShader
{
	struct TesselationBufferType
	{
		int tesselationFactorInside;
		int tesselationFactorTopEdge;
		int tesselationFactorBottomLeft;
		int tesselationFactorBottomRight;
	};

public:

	TessellationShader(ID3D11Device* device, HWND hwnd);
	~TessellationShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ExtraShaderParams extraShaderParams);

private:
	void initShader(const wchar_t* vsFilename, const wchar_t* psFilename);
	void initShader(const wchar_t* vsFilename, const wchar_t* hsFilename, const wchar_t* dsFilename, const wchar_t* psFilename);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* tesselationBuffer;
};
