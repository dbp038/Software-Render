#include "platform.h"
#include "ExampleScene.h"
#include "app/Application.h"
#include "app/resources/FileLoader.h"
#include "gfx/context/Viewport.h"

ExampleScene::ExampleScene() : winTitleUpdater( 1.0f / 23.0f ) {
	int width = App.GetWindow().GetWidth();
	int height = App.GetWindow().GetHeight();

	pCtx = std::make_unique<CtxType>();
	auto &ctx = static_cast<CtxType &>( *pCtx.get() );

	// load model and pass it to render context
	model = FileLoader::LoadPosNUvModelFromFile( "bin\\resources\\models\\skull.obj" );
	ctx.BindVertexData( model.vertices );
	ctx.BindIndexData( model.indices );

	// set vertex shader buffer data
	Matrix4f perspecive = Matrices::PerspectiveFovLH( PI / 3.0f, float( width ) / height, 0.5f, 1000.0f );
	auto &vs = ctx.GetVertexShaderData();
	vs.transformMatrix = Matrix4f::Identity();
	vs.perspectiveMatrix = perspecive;

	ctx.BindViewport( Viewport( width, height ) );

	// pass texture to pixel shader
	auto &ps = ctx.GetPixelShaderData();
	ps.texture.SetTexture( "bin\\resources\\textures\\skull.png" );
	ps.sampler.SetUVMode( ITextureSampler::UVMode::WRAP );

	ctx.BindDepthBuffer( width, height );

	screenSize = Vector2f( width, height );
}

void ExampleScene::Update() {
	auto &kb = Keyboard::Get();
	float speed = rotationSpeed;
	// hold shift to increase speed by 2
	if ( kb.IsKeyDown( Keyboard::LeftShift ) )
		speed *= 2.0f;
	if ( kb.IsKeyDown( Keyboard::LeftControl ) )
		speed *= 0.5f;

	// rotate model ( W, A, S, D )
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

	// rotate light direction ( I, J, K, L )
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
	// ( forward Numpad5, backward Numpad0, up Numpad8, down Numpad2, left Numpad4, right Numpad6 )
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

	auto &ctx = static_cast<CtxType &>( *pCtx.get() );

	gfx.ClearBackground();
	ctx.ClearDepthBuffer();

	// apply transformations to model
	auto &psBuffer = ctx.GetVertexShaderData();
	psBuffer.transformMatrix = Matrices::RotationYawPitchRoll4f( yaw, pitch, roll ) * Matrices::Scale4f(1.0f);
	psBuffer.transformMatrix = Matrices::Translation4f( xOffset, yOffset, zOffset + 1.5f ) * psBuffer.transformMatrix;
	ctx.BindVertexShader( psBuffer ); // not technically needed

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
		App.GetWindow().SetTitle( 
			storedWindowName + " (FPS = " + std::to_string( int( Time.timeScale / Time.delta ) ) + ")" 
		);
		winTitleUpdater.Reset();
	}
}

