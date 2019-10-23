// Application.h
#ifndef _APP1_H
#define _APP1_H

#include <memory>

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TextureShader.h"
#include "PositionShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void firstPass();
	void secondPass();
	void finalPass();
	void gui();

	void minimapCirclePass();
private:
	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;
	PlaneMesh* planeMesh;

	SphereMesh* posSphere;

	OrthoMesh* orthoMesh;
	OrthoMesh* orthoMesh2; //both ortho meshes are for the render textures

	LightShader* lightShader;
	TextureShader* textureShader;
	std::unique_ptr<PositionShader> positionShader;

	RenderTexture* renderTexture; //for the sphere
	RenderTexture* renderTexture2; //for the plane
	RenderTexture* minimapCircleRenderTexture;

	ExtraShaderParams extraShaderParams;

	Light* light;
};

#endif