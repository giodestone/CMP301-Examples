#include "ManipulationShader.h"

ManipulationShader::ManipulationShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"manipulation_vs.cso", L"manipulation_ps.cso");
}


ManipulationShader::~ManipulationShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the light constant buffer.
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void ManipulationShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC otherDataBufferDesc;
	D3D11_SAMPLER_DESC heightMapSamplerDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Other data buffer desc
	otherDataBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	otherDataBufferDesc.ByteWidth = sizeof(OtherDataBufferType);
	otherDataBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	otherDataBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	otherDataBufferDesc.MiscFlags = 0;
	otherDataBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&otherDataBufferDesc, NULL, &otherDataBuffer);

	// Height map sampler
	heightMapSamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	heightMapSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	heightMapSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	heightMapSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	heightMapSamplerDesc.MipLODBias = 0.0f;
	heightMapSamplerDesc.MaxAnisotropy = 1;
	heightMapSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	heightMapSamplerDesc.MinLOD = 0;
	heightMapSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&heightMapSamplerDesc, &heightMapSamplerState);



	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

}


void ManipulationShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light* light, ExtraShaderParams& extraShaderParams)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	
	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);


	// Other data buffer desc for the vertex shader
	OtherDataBufferType* otherDataBufferTypePtr = nullptr;

	result = deviceContext->Map(otherDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	otherDataBufferTypePtr = (OtherDataBufferType*)mappedResource.pData;
	otherDataBufferTypePtr->time = extraShaderParams.time;
	otherDataBufferTypePtr->amplitudeSin = extraShaderParams.amplitudeSin;
	otherDataBufferTypePtr->amplitudeCos = extraShaderParams.amplitudeCos;
	otherDataBufferTypePtr->speedSin = extraShaderParams.speedSin;
	otherDataBufferTypePtr->speedCos = extraShaderParams.speedCos;
	deviceContext->Unmap(otherDataBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &otherDataBuffer);

	//send heightmap and sampler to the vertex shader
	deviceContext->VSSetShaderResources(0, 1, &extraShaderParams.heightmap);
	deviceContext->VSSetSamplers(0, 1, &heightMapSamplerState);


	//Additional
	// Send light data to pixel shader
	LightBufferType* lightPtr;
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	lightPtr->diffuse = light->getDiffuseColour();
	lightPtr->direction = light->getDirection();
	lightPtr->padding = 0.0f;
	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &extraShaderParams.heightmap);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}
