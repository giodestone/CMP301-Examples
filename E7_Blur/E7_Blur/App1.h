// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TextureShader.h"
#include "VerticalBlurShader.h"
#include "HorizontalBlurShader.h"

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
	void firstPass();
	void verticalBlur();
	void horizontalBlur();
	void upscale();
	void finalPass();
	void gui();

private:
	CubeMesh* cubeMesh;
	OrthoMesh* orthoMesh;
	LightShader* lightShader;
	TextureShader* textureShader;

	RenderTexture* renderTexture;
	RenderTexture* horizontalBlurTexture;
	RenderTexture* verticalBlurTexture;
	
	std::unique_ptr<OrthoMesh> smallOrthoMesh;
	std::unique_ptr<RenderTexture> smallRenderTexture;
	std::unique_ptr<RenderTexture> smallHorizontalBlurTexture;
	std::unique_ptr<RenderTexture> smallVerticalBlurTexture;

	VerticalBlurShader* verticalBlurShader;
	HorizontalBlurShader* horizontalBlurShader;
	
	Light* light;
};

#endif