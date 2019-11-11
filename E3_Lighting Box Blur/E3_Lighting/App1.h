// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "LightingDetails.h"
#include "BoxBlurShader.h"
#include <memory>

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void renderMeshes();
	void gui();

private:
	LightShader* shader;
	PlaneMesh* mesh;
	SphereMesh* sphereMesh;
	SphereMesh* lightDebugSphere;
	Light* light;

	std::unique_ptr<RenderTexture> renderTexture;
	std::unique_ptr<OrthoMesh> orthoMesh;

	std::unique_ptr<BoxBlurShader> boxBlurShader;

	LightingDetails lightingDetails;
};

#endif