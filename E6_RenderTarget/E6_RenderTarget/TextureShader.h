#pragma once

#include "BaseShader.h"

#include "ExtraShaderParams.h"

using namespace std;
using namespace DirectX;

class TextureShader : public BaseShader
{
	struct DotBufferType
	{
		XMFLOAT4 dotColor;
		float radius;
		bool shouldDrawDot;
		XMFLOAT2 padding;
	};

	struct PlayerPosBufferType
	{
		XMFLOAT4 playerPos;

		XMMATRIX worldAtTopDown;
		XMMATRIX viewAtTopDown;
		XMMATRIX projectionAtTopDown;

		XMMATRIX orthoViewMatrix;
		XMMATRIX orthoMatrix;
	};

public:
	TextureShader(ID3D11Device* device, HWND hwnd);
	~TextureShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ExtraShaderParams& extraShaderParams);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;

	ID3D11Buffer* playerPosBuffer;
	ID3D11Buffer* dotBuffer;
};

