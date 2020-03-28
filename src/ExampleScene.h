#pragma once
#include "Scene.h"
#include "TestCube.h"

class ExampleScene : public Scene {

public:

	ExampleScene();

	virtual void Update() override;
	virtual void Draw( Graphics &gfx ) override;

private:

	TestCube cube;

};