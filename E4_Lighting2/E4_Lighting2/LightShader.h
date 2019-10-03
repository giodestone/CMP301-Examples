#pragma once

#include "DXF.h"

#include "ExtraLightParams.h"

using namespace std;
using namespace DirectX;

class LightShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 specularDiffuse;

		XMFLOAT3 directionalLightDireciton;
		float padding;
		XMFLOAT4 directionalLightDiffuse;

		float attenuationConstant;
		float attenuationLinear;
		float attenuationExponential;
		float padding2;
	};

	struct MultiLightBufferType
	{
		XMFLOAT4 diffuse[2]; //just use float4 before you do anything stupid... if you can think of it taking up 4 as you expect then keep it there but otherwise dont even think of using anything other than float4 as you have to pad it afterwards and it needs to be consistently padded. Seriously. Dont.
		XMFLOAT4 position[2];
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPos;
		float padding;
	};

public:
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, std::vector<Light *> lights, ExtraLightParams& extraLightParams);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* multiLightBuffer;
	ID3D11Buffer* cameraBuffer;
};

