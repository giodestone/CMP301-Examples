// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Create Mesh object and shader object
	mesh = new TessellationMesh(renderer->getDevice(), renderer->getDeviceContext());
	quadMesh = std::make_unique<QuadTessellationMesh>(renderer->getDevice(), renderer->getDeviceContext());

	shader = new TessellationShader(renderer->getDevice(), hwnd);
	quadTessShader = std::make_unique<TesselationQuadShader>(renderer->getDevice(), hwnd);

	dynamicTriTessShader = std::make_unique<DynamicTriTesselationShader>(renderer->getDevice(), hwnd);

	planeMesh = std::make_unique<PlaneMesh>(renderer->getDevice(), renderer->getDeviceContext()); ///TODO: CREATE OWN PLANE MESH FOR TESSELLLATION I..E ADD deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST); TO SEND DATA OVERRIDE!!!
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();


	/*configure extra shader params*/
	esp.tessFactor = tessFactor;
	esp.tessFactorTop = tessFactorEdgeTop;
	esp.tessFactorBL = tessFactorEdgeBL;
	esp.tessFactorBR = tessFactorEdgeBR;

	esp.tessFactor2 = tessFactor2;
	esp.tessFactorEdgeBLQuad = tessFactorEdgeBLQuad;
	esp.tessFactorEdgeBRQuad = tessFactorEdgeBRQuad;
	esp.tessFactorEdgeTLQuad = tessFactorEdgeTLQuad;
	esp.tessFactorEdgeTRQuad = tessFactorEdgeTRQuad;

	esp.shouldWave = true;
	esp.totalTime += timer->getTime();

	esp.cameraPos = camera->getPosition();
	esp.maxTessFactor = 64.f;
	esp.maxDistance = 10.f;
	esp.tessFactorModifier = 1.f;

	/*draw triangle*/
	// Send geometry data, set shader parameters, render object with shader
	mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, esp);
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	/*draw quad using quad shader*/
	//renderer->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	quadMesh->sendData(renderer->getDeviceContext());
	quadTessShader->setShaderParameters(renderer->getDeviceContext(), XMMatrixMultiply(XMMatrixTranslation(4.f, 0.f, 0.f), worldMatrix), viewMatrix, projectionMatrix, esp);
	quadTessShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount());
	//renderer->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	/*Draw triangle for tessellation*/
	mesh->sendData(renderer->getDeviceContext());
	dynamicTriTessShader->setShaderParameters(renderer->getDeviceContext(), XMMatrixMultiply(XMMatrixTranslation(0.f, 2.f, 0.f), worldMatrix), viewMatrix, projectionMatrix, esp);
	dynamicTriTessShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	planeMesh->sendData(renderer->getDeviceContext());
	dynamicTriTessShader->setShaderParameters(renderer->getDeviceContext(), XMMatrixMultiply(XMMatrixTranslation(0.f, 0.f, 0.f), worldMatrix), viewMatrix, projectionMatrix, esp);
	dynamicTriTessShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	ImGui::Text("Tesselation Controls");
	ImGui::SliderFloat("Tesselation Factor Interior", &tessFactor, 1.f, 64.f);
	ImGui::Text("Tri Controls");
	ImGui::SliderFloat("Tesselation Factor Top Edge", &tessFactorEdgeTop, 1.f, 64.f);
	ImGui::SliderFloat("Tesselation Factor Bottom Left Edge", &tessFactorEdgeBL, 1.f, 64.f);
	ImGui::SliderFloat("Tesselation Factor Bottom Right Edge", &tessFactorEdgeBR, 1.f, 64.f);
	ImGui::Text("Quad Controls");
	ImGui::SliderFloat("Tesselation Factor Interior 2", &tessFactor2, 1.f, 64.f);
	ImGui::SliderFloat("Tesselation Factor Top Left", &tessFactorEdgeTLQuad, 1.f, 64.f);
	ImGui::SliderFloat("Tesselation Factor Top Right", &tessFactorEdgeTRQuad, 1.f, 64.f);
	ImGui::SliderFloat("Tesselation Factor Bottom Left", &tessFactorEdgeBLQuad, 1.f, 64.f);
	ImGui::SliderFloat("Tesselation Factor Bottom Right", &tessFactorEdgeBRQuad, 1.f, 64.f);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

