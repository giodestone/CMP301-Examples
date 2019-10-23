#pragma once
#include "DXF.h"
#include "BaseShader.h"
#include "LightingDetails.h"

using namespace std;
using namespace DirectX;

class BoxBlurShader :
	public BaseShader
{
	struct MatrixBuffer
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};

	struct PixelShaderBufferType
	{
		XMFLOAT2 screenDimensions;
		XMFLOAT2 padding;
	};

public:
	BoxBlurShader(ID3D11Device* device, HWND hwnd);

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, Light* light, XMFLOAT2 screenDimensions);
private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* pixelShaderBuffer;
	ID3D11SamplerState* samplerState;
};

