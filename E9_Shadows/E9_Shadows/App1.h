// Application.h
#ifndef _APP1_H
#define _APP1_H

#include <memory>

// Includes
#include "DXF.h"	// include dxframework
#include "TextureShader.h"
#include "ShadowShader.h"
#include "DepthShader.h"
#include "PointShadowShader.h"
#include "PointDepthShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void renderTexturePass();
	void depthShaderRender(Light* light);
	void depthPass();
	void finalPass();
	void gui();

	void renderTexturePassPoint();
	void depthShaderRenderPoint();
	void depthPassPoint();
	void finalPassPoint();
private:
	TextureShader* textureShader;
	PlaneMesh* mesh;

	std::unique_ptr<SphereMesh> sphereMesh;
	std::unique_ptr<CubeMesh> cubeMesh;
	std::unique_ptr<OrthoMesh> orthoMesh;

	Light* light;
	std::unique_ptr<Light> light2;
	Model* model;
	ShadowShader* shadowShader;
	DepthShader* depthShader;

	ShadowMap* shadowMap;
	std::unique_ptr<ShadowMap> shadowMap2;

	std::unique_ptr<Light> pointLight;
	std::unique_ptr<PointShadowShader> pointShadowShader;
	std::unique_ptr<PointDepthShader> pointDepthShader;
	std::unique_ptr<ShadowMap> pointLightShadowMap;
	std::unique_ptr<RenderTexture> pointLightRenderTexture;

	std::unique_ptr<RenderTexture> renderTexture;

	float curRotation;
	bool isPointLight = true;
};

#endif