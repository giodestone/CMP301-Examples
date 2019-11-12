#pragma once
#include "DXF.h"

class MyMeshClass : public BaseMesh
{
public:
	MyMeshClass(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~MyMeshClass();

	void initBuffers(ID3D11Device* device) override;

	void sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST) override;
};

