// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"

#include <memory>

#include "ExtraLightParams.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

private:
	LightShader* shader;
	PlaneMesh* mesh;

	SphereMesh* ball1;
	CubeMesh* cube1;

	std::vector<std::unique_ptr<Light>> lights;

	Light* light;

	float lightPos[3];

	ExtraLightParams extraLightParams;


	std::vector<Light*> getLights();
};

#endif