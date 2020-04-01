#pragma once
#include "Scene.h"
#include "app/Time.h"
#include "gfx/RenderContext.h"
#include "gfx/VertexDeclarations.h"
#include "gfx/shader/PerspectiveVertexShader.h"
#include "gfx/shader/VertexColorPixelShader.h"

class ExampleScene : public Scene {

public:

	ExampleScene();

	virtual void Update() override;
	virtual void Draw( Graphics &gfx ) override;

private:

	RenderContext<
		PosColorVertex, 
		PerspectiveVertexShader<PosColorVertex, PosColorVertex>, 
		VertexColorPixelShader<PosColorVertex>
	> ctx;

	std::vector<PosColorVertex> cubeVertices;
	std::vector<size_t> cubeIndices;

	float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
	float rotationSpeed = 1.0f;

	std::string storedWindowName;
	UnscaledTimer winTitleUpdater;
	void ShowFPS();

};