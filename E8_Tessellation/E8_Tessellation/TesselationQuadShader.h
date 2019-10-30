#pragma once
#include "DXF.h"

#include "ExtraShaderParams.h"

class TesselationQuadShader :
	public BaseShader
{
	struct TesselationQuadType
	{
		float tessFactorInside;
		float tessFactorInside2;

		float tessFactorEdgeTLQuad;
		float tessFactorEdgeTRQuad;
		float tessFactorEdgeBLQuad;
		float tessFactorEdgeBRQuad;

		XMFLOAT2 padding;
	};

public:
	TesselationQuadShader(ID3D11Device* device, HWND hwnd);
	~TesselationQuadShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ExtraShaderParams extraShaderParams);

private:
	void initShader(const wchar_t* vsFilename, const wchar_t* psFilename);
	void initShader(const wchar_t* vsFilename, const wchar_t* hsFilename, const wchar_t* dsFilename, const wchar_t* psFilename);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* tesselationBuffer;
};

