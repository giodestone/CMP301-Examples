// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"


App1::App1()
{
	mesh = nullptr;
	shader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Create Mesh object and shader object
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	shader = new LightShader(renderer->getDevice(), hwnd);
	light = new Light;
	light->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setPosition(50.0f, 10.0f, 50.0f);

	auto light1Point = std::make_unique<Light>();
	light1Point.get()->setAmbientColour(0.f, 0.f, 0.f, 1.f);
	light1Point.get()->setDiffuseColour(1.f, 0.f, 1.f, 1.f);
	light1Point.get()->setPosition(-10.f, 0.f, 0.f);
	lights.push_back(std::move(light1Point));

	auto light2Point = std::make_unique<Light>();
	light2Point.get()->setAmbientColour(0.f, 0.f, 0.f, 1.f);
	light2Point.get()->setDiffuseColour(0.f, 1.f, 1.f, 1.f);
	light2Point.get()->setPosition(10.f, 0.f, 0.f);
	lights.push_back(std::move(light2Point));

	//auto lightDirectional = std::make_unique<Light>();
	//lightDirectional.get()->setAmbientColour(0.f, 0.f, 0.f, 1.f);
	//lightDirectional.get()->setDiffuseColour(1.f, 1.f, 1.f, 1.f);
	//lightDirectional.get()->setDirection(0.5f, 0.5f, 0.f);
	//lights.push_back(std::move(lightDirectional));

	ball1 = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cube1 = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());

	//Set position ptr for ImGUI
	lightPos[0] = light->getPosition().x;
	lightPos[1] = light->getPosition().y;
	lightPos[2] = light->getPosition().z;


	extraLightParams.directionalDiffuse[0] = 1.f;
	extraLightParams.directionalDiffuse[1] = 0.5f;
	extraLightParams.directionalDiffuse[2] = 0.f;
	extraLightParams.directionalDiffuse[3] = 0.f;

	extraLightParams.directionalDirection[0] = 0.5f;
	extraLightParams.directionalDirection[1] = -0.5f;
	extraLightParams.directionalDirection[2] = 0.f;
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

	delete ball1;
	ball1 = nullptr;

	delete cube1;
	cube1 = nullptr;
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	//update position from ImGUI
	light->setPosition(lightPos[0], lightPos[1], lightPos[2]);

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
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), getLights(), extraLightParams);
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	ball1->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), getLights(), extraLightParams);
	shader->render(renderer->getDeviceContext(), ball1->getIndexCount());

	auto transform = XMMatrixTranslation(15.f, 25.f, 20.f) * worldMatrix;
	cube1->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), transform, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), getLights(), extraLightParams);
	shader->render(renderer->getDeviceContext(), cube1->getIndexCount());

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
	ImGui::Text("Press E to raise camera \nto see the plane being rendered");

	//ImGui::DragFloat3("Light Position", lightPos, 0.1f);
	ImGui::DragFloat("Constant Attenuation", &extraLightParams.attConst, 0.1f, 0.f);
	ImGui::DragFloat("Linear Attenuation", &extraLightParams.attLin, 0.01f, 0.f);
	ImGui::DragFloat("Exponential Attenuation", &extraLightParams.attExp, 0.0001f, 0.f);

	ImGui::ColorEdit4("Directional Diffuse", extraLightParams.directionalDiffuse);
	ImGui::DragFloat3("Directional Direction", extraLightParams.directionalDirection, 0.1f);


	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

std::vector<Light*> App1::getLights()
{
	std::vector<Light*> lightReturn;
	
	for (size_t i = 0; i < lights.size(); ++i)
	{
		lightReturn.push_back(lights.at(i).get());
	}

	return lightReturn;
}

