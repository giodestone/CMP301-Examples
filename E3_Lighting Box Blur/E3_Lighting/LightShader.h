#pragma once

#include "DXF.h"
#include "LightingDetails.h"

using namespace std;
using namespace DirectX;

class LightShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float padding;

		XMFLOAT4 ambientColour;
		
		//for the point light
		XMFLOAT3 lightPosition;
		float padding2;
		float padding21;
		float attenuationConstant;
		float attenuationLinear;
		float attenuationExponential;

		XMFLOAT4 specularColor;
		float specularPower;
		XMFLOAT3 padding3;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPos;
		float padding;
	};

public:
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, LightingDetails& lightingDetails);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* cameraBuffer; //for the passing camera pos into vertex shader
};

