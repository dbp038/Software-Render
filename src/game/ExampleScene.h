#pragma once
#include "Scene.h"
#include "app/Time.h"
#include "gfx/context/RenderContext.h"

class ExampleScene : public Scene {

public:

	ExampleScene();

	virtual void Update() override;
	virtual void Draw( Graphics &gfx ) override;

private:

	std::unique_ptr<IRenderContext> pCtx;

	float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
	float rotationSpeed = 1.0f;
	float xOffset = 0.0f, yOffset = 0.0f, zOffset = 0.0f;
	
	

	std::string storedWindowName;
	UnscaledTimer winTitleUpdater;
	void ShowFPS();

};