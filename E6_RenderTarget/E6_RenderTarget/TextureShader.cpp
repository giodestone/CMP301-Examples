#include "TextureShader.h"



TextureShader::TextureShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"texture_vs.cso", L"texture_ps.cso");
}


TextureShader::~TextureShader()
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

	//Release base shader components
	BaseShader::~BaseShader();
}


void TextureShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
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

	// player dot buffer
	D3D11_BUFFER_DESC dotBufferDesc;
	dotBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	dotBufferDesc.ByteWidth = sizeof(DotBufferType);
	dotBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dotBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dotBufferDesc.MiscFlags = 0;
	dotBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&dotBufferDesc, NULL, &dotBuffer);

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

}


void TextureShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, ExtraShaderParams& extraShaderParams)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Send matrix data
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	//send player pos to vertex shader
	result = deviceContext->Map(playerPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	PlayerPosBufferType* playerPosBufferPtr = (PlayerPosBufferType*)mappedResource.pData;

	playerPosBufferPtr->playerPos = XMFLOAT4(extraShaderParams.playerPos.x, extraShaderParams.playerPos.y, extraShaderParams.playerPos.z, 1.f);
	playerPosBufferPtr->projectionAtTopDown = extraShaderParams.projectionAtTopDown;
	playerPosBufferPtr->viewAtTopDown = extraShaderParams.viewAtTopDown;
	playerPosBufferPtr->worldAtTopDown = extraShaderParams.worldAtTopDown;

	deviceContext->Unmap(playerPosBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &playerPosBuffer);

	// send player dot data to buffer
	DotBufferType* playerDotBufferPtr;
	deviceContext->Map(dotBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	playerDotBufferPtr = (DotBufferType*)mappedResource.pData;

	playerDotBufferPtr->dotColor = extraShaderParams.colorOfDot;
	playerDotBufferPtr->radius = extraShaderParams.radiusOfDot;
	playerDotBufferPtr->shouldDrawDot = extraShaderParams.drawPlayer;

	deviceContext->Unmap(dotBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &dotBuffer);

	// Set shader texture and sampler resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}





