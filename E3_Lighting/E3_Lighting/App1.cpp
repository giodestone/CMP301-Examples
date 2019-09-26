// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	mesh = nullptr;
	shader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	textureMgr->loadTexture(L"brick", L"res/brick1.dds");

	// Create Mesh object and shader object
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	shader = new LightShader(renderer->getDevice(), hwnd);
	light = new Light;
	
	//Setup light params
	this->lightingDetails.ambientColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.f);
	this->lightingDetails.diffuseColor = XMFLOAT4(0.9f, 1.f, 0.1f, 0.f);
	this->lightingDetails.direction = XMFLOAT3(0.f, -1.f, 0.f);
	this->lightingDetails.position = XMFLOAT3(-2.f, 0.f, 0.f);

	//initialise the floats for imgui
	lightingDetails.ambientColorF = new float[4];
	lightingDetails.ambientColorF[0] = lightingDetails.ambientColor.x;
	lightingDetails.ambientColorF[1] = lightingDetails.ambientColor.y;
	lightingDetails.ambientColorF[2] = lightingDetails.ambientColor.z;
	lightingDetails.ambientColorF[3] = lightingDetails.ambientColor.w;

	lightingDetails.diffuseColorF = new float[4];
	lightingDetails.diffuseColorF[0] = lightingDetails.diffuseColor.x;
	lightingDetails.diffuseColorF[1] = lightingDetails.diffuseColor.y;
	lightingDetails.diffuseColorF[2] = lightingDetails.diffuseColor.z;
	lightingDetails.diffuseColorF[3] = lightingDetails.diffuseColor.w;

	lightingDetails.positionF = new float[3];
	lightingDetails.positionF[0] = lightingDetails.position.x;
	lightingDetails.positionF[1] = lightingDetails.position.y;
	lightingDetails.positionF[2] = lightingDetails.position.z;
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (mesh)
	{
		delete mesh;
		mesh = 0;
	}

	if (shader)
	{
		delete shader;
		shader = 0;
	}

	delete[] lightingDetails.ambientColorF;
	lightingDetails.ambientColorF = nullptr;

	delete[] lightingDetails.diffuseColorF;
	lightingDetails.diffuseColorF = nullptr;

	delete[] lightingDetails.positionF;
	lightingDetails.positionF = nullptr;
}


bool App1::frame()
{
	bool result;

	//Update XMFLOAT4 colours from values set in the imgui
	lightingDetails.ambientColor.x = lightingDetails.ambientColorF[0];
	lightingDetails.ambientColor.y = lightingDetails.ambientColorF[1];
	lightingDetails.ambientColor.z = lightingDetails.ambientColorF[2];
	lightingDetails.ambientColor.w = lightingDetails.ambientColorF[3];

	lightingDetails.diffuseColor.x = lightingDetails.diffuseColorF[0];
	lightingDetails.diffuseColor.y = lightingDetails.diffuseColorF[1];
	lightingDetails.diffuseColor.z = lightingDetails.diffuseColorF[2];
	lightingDetails.diffuseColor.w = lightingDetails.diffuseColorF[3];

	lightingDetails.position.x = lightingDetails.positionF[0];
	lightingDetails.position.y = lightingDetails.positionF[1];
	lightingDetails.position.z = lightingDetails.positionF[2];

	//now update the values inside of the light class
	light->setDiffuseColour(lightingDetails.diffuseColor.x, lightingDetails.diffuseColor.y, lightingDetails.diffuseColor.y, lightingDetails.diffuseColor.w);
	light->setDirection(lightingDetails.direction.x, lightingDetails.direction.y, lightingDetails.direction.z); //Not updated via GUI
	light->setAmbientColour(lightingDetails.ambientColor.x, lightingDetails.ambientColor.y, lightingDetails.ambientColor.z, lightingDetails.ambientColor.w);
	light->setPosition(lightingDetails.position.x, lightingDetails.position.y, lightingDetails.position.z);

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

	// Send geometry data, set shader parameters, render object with shader
	mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), light);
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	sphereMesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), light);
	shader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());


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

	ImGui::ColorEdit4("Ambient Colour", lightingDetails.ambientColorF);
	ImGui::ColorEdit4("Diffuse Colour", lightingDetails.diffuseColorF);
	ImGui::DragFloat3("Light Position", lightingDetails.positionF, 0.1f, -1000.f, 1000.f);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

