#pragma once
#include "Scene.h"
#include "TestCube.h"
#include "Time.h"

class ExampleScene : public Scene {

public:

	ExampleScene();

	virtual void Update() override;
	virtual void Draw( Graphics &gfx ) override;

private:

	TestCube cube;
	float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
	float rotationSpeed = 1.0f;

	std::string storedWindowName;
	UnscaledTimer winTitleUpdater;
	void ShowFPS();

};