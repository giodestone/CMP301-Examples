// Application.h
#ifndef _APP1_H
#define _APP1_H

#include <memory>

// Includes
#include "DXF.h"	// include dxframework
#include "TessellationShader.h"
#include "TesselationQuadShader.h"
#include "QuadTessellationMesh.h"
#include "DynamicTriTesselationShader.h"

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

	std::unique_ptr<DynamicTriTesselationShader> dynamicTriTessShader;

	std::unique_ptr<PlaneMesh> planeMesh;

	ExtraShaderParams esp;

	float tessFactor = 4.f;

	float tessFactorEdgeTop = 1.f;
	float tessFactorEdgeBL = 1.f;
	float tessFactorEdgeBR = 1.f;

	float tessFactor2 = 4.f;
	float tessFactorEdgeTLQuad = 4.f;
	float tessFactorEdgeTRQuad = 4.f;
	float tessFactorEdgeBLQuad = 4.f;
	float tessFactorEdgeBRQuad = 4.f;


};

#endif