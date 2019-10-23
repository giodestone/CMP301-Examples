#include "PositionShader.h"

PositionShader::PositionShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"texture_vs.cso", L"position_ps.cso");
}

void PositionShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

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

	//setup player pos buffer
	D3D11_BUFFER_DESC playerBufferDesc;

	playerBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	playerBufferDesc.ByteWidth = sizeof(PlayerPosBufferType);
	playerBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	playerBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	playerBufferDesc.MiscFlags = 0;
	playerBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&playerBufferDesc, NULL, &playerPosBuffer);

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);
}


void PositionShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, ExtraShaderParams& extraShaderParams)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(world);
	tview = XMMatrixTranspose(view);
	tproj = XMMatrixTranspose(projection);

	auto identityt = XMMatrixTranspose(XMMatrixIdentity());
	// Send matrix data
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = identityt;// worldMatrix;
	dataPtr->view = identityt;
	dataPtr->projection = identityt;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	//map player pos buffer
	result = deviceContext->Map(playerPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	PlayerPosBufferType* playerPosPtr = (PlayerPosBufferType*)mappedResource.pData;
	playerPosPtr->playerPosition = extraShaderParams.playerPos;
	playerPosPtr->screenDimensions = XMFLOAT2(1200.f, 600.f);
	playerPosPtr->worldAtTopDown = XMMatrixTranspose(world);
	playerPosPtr->viewAtTopDown = XMMatrixTranspose(view);
	playerPosPtr->projectionAtTopDown = XMMatrixTranspose(projection);
	deviceContext->Unmap(playerPosBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &playerPosBuffer);


	// Set shader texture and sampler resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}
