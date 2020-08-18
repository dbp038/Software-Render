#pragma once
#include "Scene.h"
#include "app/Time.h"
#include "app/resources/Model.h"
#include "gfx/context/RenderContext.h"

#include "gfx/context/VertexDeclarations.h"
#include "gfx/shader/PerspectiveVertexShader.h"
#include "gfx/context/TextureSampler.h"
#include "gfx/shader/BasicIlluminationPixelShader.h"

class ExampleScene : public Scene {

public:

	ExampleScene();

	virtual void Update() override;
	virtual void Draw( Graphics &gfx ) override;

private:

	// render pipeline types definitions are specified at compile time
	// we basically need to define the vertex type, the vertex shader and the pixel shader (geometry 
	// shader could be specified too) templating RenderContext class to these types
	using VertexType = PosNUvVertex;

	using Sampler = TextureLinearSampler;

	using VS = PerspectiveVertexShaderWithNormals<VertexType>;
	using PS = BasicIlluminationTexturePixelShader<Sampler>;

	using CtxType = RenderContext<VertexType, VS, PS>;

	std::unique_ptr<CtxType> pCtx;
	Model<VertexType> model;

	float yaw = PI, pitch = 0.0f, roll = 0.0f;
	float lightYaw = 0.0f, lightPitch = 0.0f, lightRoll = 0.0f;
	float rotationSpeed = 1.0f;
	float xOffset = 0.0f, yOffset = 0.0f, zOffset = 0.0f;

	Vector2f screenSize;

	std::string storedWindowName;
	UnscaledTimer winTitleUpdater;
	void ShowFPS();

};