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
	
	textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());

	posSphere = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	///TODO: THE RENDER TEXTURE HAS TO BE MAX SIZE TO BE ABLE TO MATCH THE COORDINATES 1:1 WITH SCREEN SPACE, IT HAS TO BE SCALED DOWN LATER -- PROBABLY SHOULD NOT RENDER THE FULL SIZED ONE, JUST THE SMALL ONE WITH A DIFFERENT TEXTURE!
	rightSmallOrthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, screenWidth / 2.7, screenHeight / 2.7); //TODO GET RID OF X,Y POS (DEFAULT TO 0) AND SET WIDTH HEIGHT TO SCREEN SIZE
	//orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight); //TODO GET RID OF X,Y POS (DEFAULT TO 0) AND SET WIDTH HEIGHT TO SCREEN SIZE
	leftSmallOrthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, -screenWidth / 2.7, screenHeight / 2.7);

	fullScreenOrthoMesh = make_unique<OrthoMesh>(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);

	lightShader = new LightShader(renderer->getDevice(), hwnd);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);

	positionShader = make_unique<PositionShader>(renderer->getDevice(), hwnd);

	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	renderTexture2 = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	minimapCircleRenderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	light = new Light;
	light->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.f, -0.7f, 0.7f);

	//Create top down camera at y 10 looking down
	topDownCamera = std::make_unique<Camera>();
	topDownCamera->setPosition(0.f, 10.f, 0.f);
	topDownCamera->setRotation(90.f, 0.f, 0.f);
	topDownCamera->update();
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
	// Render first pass to render texture
	//spherePass();

	//render scene from top down
	minimapTopDownGeometryPass();

	//render the circle on top down
	minimapCirclePass();

	// Render final pass to frame buffer
	finalPass();

	return true;
}

void App1::spherePass()
{
	// Set the render target to be the render to texture and clear it
	renderTexture->setRenderTarget(renderer->getDeviceContext());
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 0.0f, 0.0f, 1.0f);

	// Get matrices

	//set camera to position - could add second camera but would need to fix
	auto originalCameraPos = camera->getPosition();
	auto originalCameraRotation = camera->getRotation();
	camera->setPosition(-1.f, 0.f, -2.f);
	camera->setRotation(0.f, 20.f, 0.f); //look slightly right

	camera->update();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	// Render shape with simple lighting shader set.
	sphereMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"default"), light);
	lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	//reset camera to old position and rotation.
	camera->setPosition(originalCameraPos.x, originalCameraPos.y, originalCameraPos.z);
	camera->setRotation(originalCameraRotation.x, originalCameraRotation.y, originalCameraRotation.z);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::minimapTopDownGeometryPass()
{
	renderTexture2->setRenderTarget(renderer->getDeviceContext());
	renderTexture2->clearRenderTarget(renderer->getDeviceContext(), 0.f, 1.f, 0.f, 1.f);

	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = topDownCamera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	//extraShaderParams.worldAtTopDown = worldMatrix;
	//extraShaderParams.viewAtTopDown = viewMatrix;
	//extraShaderParams.projectionAtTopDown = projectionMatrix;
	////for projecting the circle
	//extraShaderParams.orthoViewMatrix = camera->getOrthoViewMatrix();
	//extraShaderParams.orthoMatrix = renderer->getOrthoMatrix();

	//render the cube with default texture
	planeMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"default"), light);
	lightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());	
	
	cubeMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"default"), light);
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());



	//render the position sphere as the last thing and disable z buffer so it draws on top
	/*renderer->setZBuffer(false);
	posSphere->sendData(renderer->getDeviceContext());
	XMMATRIX posMatrix = XMMatrixTranslation(originalCameraPos.x, 0.f, originalCameraPos.z);
	XMMATRIX scaleMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f);
	lightShader->setShaderParameters(renderer->getDeviceContext(), XMMatrixMultiply(XMMatrixMultiply(worldMatrix, scaleMatrix), posMatrix), viewMatrix, projectionMatrix, textureMgr->getTexture(L"default"), light);
	lightShader->render(renderer->getDeviceContext(), posSphere->getIndexCount());
	renderer->setZBuffer(true);*/

	//renderer->getDeviceContext()->VSSetShader(NULL, NULL, NULL);
	//render the dot at the camera pos

	// reset the render target back to the original one
	renderer->setBackBufferRenderTarget();
}

void App1::minimapCirclePass()
{
	minimapCircleRenderTexture->setRenderTarget(renderer->getDeviceContext());
	minimapCircleRenderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.f, 0.f, 1.f, 1.f);

	XMFLOAT3 playerPos;
	playerPos.x = camera->getPosition().x;
	playerPos.y = camera->getPosition().y;
	playerPos.z = camera->getPosition().z;

	fullScreenOrthoMesh->sendData(renderer->getDeviceContext());
	positionShader.get()->setShaderParameters(renderer->getDeviceContext(), renderer->getWorldMatrix(), topDownCamera->getOrthoViewMatrix(), renderer->getOrthoMatrix(), renderTexture2->getShaderResourceView(), playerPos, topDownCamera->getViewMatrix(), renderer->getProjectionMatrix());
	positionShader.get()->render(renderer->getDeviceContext(), fullScreenOrthoMesh->getIndexCount());

	renderer->setBackBufferRenderTarget();
}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Get matrices
	camera->update();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	// Render normal scene, with light shader set.
	planeMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), light);
	lightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	cubeMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"default"), light);
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// RENDER THE RENDER TEXTURE SCENE
	// Requires 2D rendering and an ortho mesh.
	renderer->setZBuffer(false);
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	/*rightSmallOrthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, renderTexture->getShaderResourceView(), extraShaderParams);
	textureShader->render(renderer->getDeviceContext(), rightSmallOrthoMesh->getIndexCount());*/

	//extraShaderParams.drawPlayer = true;
	//extraShaderParams.playerPos = camera->getPosition();
	//extraShaderParams.playerPos.y = 0.f;

	// render the minimap
	leftSmallOrthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, minimapCircleRenderTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), leftSmallOrthoMesh->getIndexCount());
	//extraShaderParams.drawPlayer = false;

	// enable z buffer
	renderer->setZBuffer(true);
	
	// Render GUI
	gui();

	// Present the rendered scene to the screen.
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


