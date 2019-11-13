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
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	model = new Model(renderer->getDevice(), renderer->getDeviceContext(), "res/teapot.obj");
	textureMgr->loadTexture(L"brick", L"res/brick1.dds");

	sphereMesh = std::make_unique<SphereMesh>(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = std::make_unique<CubeMesh>(renderer->getDevice(), renderer->getDeviceContext());
	orthoMesh = std::make_unique<OrthoMesh>(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4.f, screenHeight / 4.f, (-screenWidth / 4.f) + (-screenWidth / 8.f), (screenHeight / 4.f) + (screenHeight / 8.f));

	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	int shadowmapWidth = 1024;
	int shadowmapHeight = 1024;
	int sceneWidth = 100;
	int sceneHeight = 100;

	// This is your shadow map
	shadowMap = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	shadowMap2 = std::make_unique<ShadowMap>(renderer->getDevice(), shadowmapWidth, shadowmapHeight);

	light = new Light;
	light->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.0f, -0.2f, 0.7f);
	light->setPosition(0.f, 0.f, -10.f);
	light->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	light2 = std::make_unique<Light>();
	light2->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light2->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light2->setDirection(-0.17f, -0.88f, -0.44f);
	light2->setPosition(0.f, 0.f, 20.f);
	light2->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	renderTexture = std::make_unique<RenderTexture>(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	curRotation = 0.f;
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
	
	curRotation += XMConvertToRadians(10.f);

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

	// Perform depth pass
	depthPass();

	//render depth to render texture
	renderTexturePass();

	// Render scene
	finalPass();

	return true;
}

void App1::renderTexturePass()
{
	renderTexture->setRenderTarget(renderer->getDeviceContext());
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 1.f, 1.f, 0.f, 0.f);

	depthShaderRender(light);

	renderer->setBackBufferRenderTarget();
}

void App1::depthShaderRender(Light* light)
{
	// get the world, view, and projection matrices from the camera and d3d objects.
	light->generateViewMatrix();
	light->generateProjectionMatrix(0.1f, 1000.f);


	//light2->generateViewMatrix();
	XMMATRIX lightViewMatrix = light->getViewMatrix();
	XMMATRIX lightProjectionMatrix = light->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	
	// Render model
	model->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());

	// Render sphere
	sphereMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), XMMatrixMultiply(renderer->getWorldMatrix(), XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.f, 0.f, curRotation), XMMatrixTranslation(-10.f, 3.f + sinf(curRotation) * 2.f, 0.f))),
		lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	// Render cube
	cubeMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), XMMatrixMultiply(renderer->getWorldMatrix(), XMMatrixMultiply(XMMatrixScaling(3.f, 3.f, 3.f), XMMatrixTranslation(15.f, 3.f, 0.f))), 
		lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
}

void App1::depthPass()
{
	// Set the render target to be the render to texture.
	shadowMap->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	depthShaderRender(light);

	renderer->resetViewport();
	
	renderer->setBackBufferRenderTarget();
	//Render shadowmap 2

	shadowMap2->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());
	
	depthShaderRender(light2.get());

	// Set back buffer as render target and reset view port.
	renderer->resetViewport();
	
	renderer->setBackBufferRenderTarget();
}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
	camera->update();

	std::vector<Light*> lights;
	lights.push_back(light);
	lights.push_back(light2.get());

	std::vector<ID3D11ShaderResourceView*> depthMaps;
	depthMaps.push_back(shadowMap->getDepthMapSRV());
	depthMaps.push_back(shadowMap2.get()->getDepthMapSRV());

	// get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, 
		textureMgr->getTexture(L"brick"), depthMaps, lights);
	shadowShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	// Render model
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	model->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), depthMaps, lights);
	shadowShader->render(renderer->getDeviceContext(), model->getIndexCount());

	// Render sphere
	sphereMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), XMMatrixMultiply(renderer->getWorldMatrix(), XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.f, 0.f, curRotation), XMMatrixTranslation(-10.f, 3.f + sinf(curRotation) * 2.f, 0.f))), viewMatrix, projectionMatrix,
		textureMgr->getTexture(L"brick"), depthMaps, lights);
	shadowShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	// Render cube
	cubeMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), XMMatrixMultiply(renderer->getWorldMatrix(), XMMatrixMultiply(XMMatrixScaling(3.f, 3.f, 3.f), XMMatrixTranslation(15.f, 3.f, 0.f))), viewMatrix, projectionMatrix,
		textureMgr->getTexture(L"brick"), depthMaps, lights);
	shadowShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	//render the shadow map
	renderer->setZBuffer(false);

	orthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), renderer->getWorldMatrix(), camera->getOrthoViewMatrix(), renderer->getOrthoMatrix(), renderTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	renderer->setZBuffer(true);
	
	gui();
	renderer->endScene();
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

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

