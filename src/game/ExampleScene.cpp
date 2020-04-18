#include "pch.h"
#include "ExampleScene.h"
#include "app/Application.h"
#include "app/resources/FileLoader.h"
#include "gfx/context/Viewport.h"

#include "gfx/context/TextureSampler.h"
#include "gfx/shader/PerspectiveVertexShader.h"
#include "gfx/shader/BasicIlluminationPixelShader.h"

using VertexType = PosNUvVertex;

using Sampler = TextureLinearSampler;

using VS = PerspectiveVertexShader;
using PS = BasicIlluminationTexturePixelShader<Sampler>;

using CtxType = RenderContext<VertexType, VS, PS>;

Model<VertexType> model;

ExampleScene::ExampleScene() : winTitleUpdater( 1.0f / 23.0f ) {
	int width = App.GetWindow().GetWidth();
	int height = App.GetWindow().GetHeight();

	// initialize render context
	pCtx = std::make_unique<CtxType>();
	CtxType &ctx = *static_cast<CtxType *>( pCtx.get() );

	// pass texture to pixel shader
	auto &ps = ctx.GetPixelShaderData();
	ps.texture.SetTexture( "bin\\resources\\textures\\skull.png" );
	ps.sampler.SetUVMode( ITextureSampler::UVMode::WRAP );

	ctx.BindViewport( Viewport( width, height ) );

	// load model and pass it to render context
	model = FileLoader::LoadPosNUvModelFromFile( "bin\\resources\\models\\skull.obj" );
	ctx.BindVertexData( model.vertices );
	ctx.BindIndexData( model.indices );

	Matrix4f perspecive = Matrices::PerspectiveFovLH( PI / 3.0f, float( width ) / height, 0.5f, 1000.0f );
	ctx.BindVertexShaderBuffer( { Matrix4f::Identity(), perspecive } );

	ctx.BindDepthBuffer( width, height );

	screenSize = Vector2f( width, height );
}

void ExampleScene::Update() {
	auto &kb = Keyboard::Get();
	float speed = rotationSpeed;
	if ( kb.IsKeyDown( Keyboard::LeftShift ) )
		speed *= 2.0f;
	if ( kb.IsKeyDown( Keyboard::LeftControl ) )
		speed *= 0.5f;

	// rotate model
	if ( kb.IsKeyDown( Keyboard::W ) ) {
		pitch -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::S ) ) {
		pitch += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::D ) ) {
		yaw += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::A ) ) {
		yaw -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::E ) ) {
		roll -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::Q ) ) {
		roll += Time.delta * speed;
	}

	// rotate light direction
	if ( kb.IsKeyDown( Keyboard::I ) ) {
		lightPitch += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::K ) ) {
		lightPitch -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::L ) ) {
		lightYaw += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::J ) ) {
		lightYaw -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::U ) ) {
		lightRoll += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::O ) ) {
		lightRoll -= Time.delta * speed;
	}

	// translate model
	if ( kb.IsKeyDown( Keyboard::NumPad8 ) ) {
		yOffset += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::NumPad2 ) ) {
		yOffset -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::NumPad6 ) ) {
		xOffset += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::NumPad4 ) ) {
		xOffset -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::NumPad5 ) ) {
		zOffset += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::NumPad0 ) ) {
		zOffset -= Time.delta * speed;
	}

	if ( kb.IsKeyJustPressed( Keyboard::Escape ) )
		App.RequestClose();
}

void ExampleScene::Draw( Graphics &gfx ) {
	ShowFPS();

	gfx.ClearBackground();

	CtxType &ctx = *static_cast<CtxType *>( pCtx.get() );
	ctx.ClearDepthBuffer();

	// apply transformations to model
	auto &psBuffer = ctx.GetVertexShaderData();
	psBuffer.transformMatrix = Matrices::RotationYawPitchRoll4f( yaw, pitch, roll ) * Matrices::Scale4f(1.0f);
	psBuffer.transformMatrix = Matrices::Translation4f( xOffset, yOffset, zOffset + 1.5f ) * psBuffer.transformMatrix;
	ctx.BindVertexShaderBuffer( psBuffer ); // not technically needed

	// apply transformations to light
	auto &ps = ctx.GetPixelShaderData();
	Vector4f lightDir;
	lightDir = Matrices::RotationYawPitchRoll4f( lightYaw, lightPitch, lightRoll ) * Vector4f( 0.0f, 0.0f, 1.0f, 1.0f );
	ps.lightDirection.x() = lightDir.x();
	ps.lightDirection.y() = lightDir.y();
	ps.lightDirection.z() = lightDir.z();

	// tell graphics what we want to draw passing the render context with all this info
	gfx.BindContext( ctx );
	// perform the actual draw
	gfx.Draw();
}

void ExampleScene::ShowFPS() {
	if ( storedWindowName.empty() )
		storedWindowName = App.GetWindow().GetTitle();
	if ( winTitleUpdater.IsFinished() ) {
		App.GetWindow().SetTitle( storedWindowName + " (FPS = " + std::to_string( int( 1.0f / Time.delta ) ) + ")" );
		winTitleUpdater.Reset();
	}
}

