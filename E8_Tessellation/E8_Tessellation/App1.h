// Application.h
#ifndef _APP1_H
#define _APP1_H

#include <memory>

// Includes
#include "DXF.h"	// include dxframework
#include "TessellationShader.h"
#include "TesselationQuadShader.h"
#include "QuadTessellationMesh.h"

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
	TessellationMesh* mesh;
	std::unique_ptr<QuadTessellationMesh> quadMesh;

	std::unique_ptr<TesselationQuadShader> quadTessShader;
	TessellationShader* shader;

	ExtraShaderParams esp;

	float tessFactor = 1.f;

	float tessFactorEdgeTop = 1.f;
	float tessFactorEdgeBL = 1.f;
	float tessFactorEdgeBR = 1.f;

	float tessFactor2 = 1.f;
	float tessFactorEdgeTLQuad = 1.f;
	float tessFactorEdgeTRQuad = 1.f;
	float tessFactorEdgeBLQuad = 1.f;
	float tessFactorEdgeBRQuad = 1.f;


};

#endif