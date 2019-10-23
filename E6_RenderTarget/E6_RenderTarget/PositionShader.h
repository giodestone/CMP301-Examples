#pragma once
#include "DXF.h"
#include "BaseShader.h"
#include "ExtraShaderParams.h"

class PositionShader :
	public BaseShader
{
	struct PlayerPosBufferType
	{
		XMFLOAT2 screenDimensions;
		XMFLOAT3 playerPosition;
		XMFLOAT3 padding;

		XMMATRIX worldAtTopDown;
		XMMATRIX viewAtTopDown;
		XMMATRIX projectionAtTopDown;
	};

public:

	PositionShader(ID3D11Device* device, HWND hwnd);

	void initShader(const wchar_t* vsFilename, const wchar_t* psFilename);

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, ExtraShaderParams& extraShaderParams);

private:
	ID3D11Buffer* playerPosBuffer;
	ID3D11SamplerState* sampler;
};

