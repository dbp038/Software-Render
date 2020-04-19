#pragma once
#include "Scene.h"
#include "app/Time.h"
#include "app/resources/Model.h"
#include "gfx/context/RenderContext.h"
#include "gfx/context/VertexDeclarations.h"

class ExampleScene : public Scene {

public:

	ExampleScene();

	virtual void Update() override;
	virtual void Draw( Graphics &gfx ) override;

private:

	std::unique_ptr<IRenderContext> pCtx;

	float yaw = PI, pitch = 0.0f, roll = 0.0f;
	float lightYaw = 0.0f, lightPitch = 0.0f, lightRoll = 0.0f;
	float rotationSpeed = 1.0f;
	float xOffset = 0.0f, yOffset = 0.0f, zOffset = 0.0f;

	Vector2f screenSize;
	Vector2f highlightedPixel = Vector2f::Zero();

	std::string storedWindowName;
	UnscaledTimer winTitleUpdater;
	void ShowFPS();

};