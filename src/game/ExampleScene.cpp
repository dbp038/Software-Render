#include "pch.h"
#include "ExampleScene.h"
#include "GeometryGenerator.h"
#include "app/Application.h"
#include "gfx/Viewport.h"

#include "gfx/TextureBitmap.h"
#include "gfx/VertexDeclarations.h"
#include "gfx/TextureSampler.h"
#include "gfx/shader/PerspectiveVertexShader.h"
#include "gfx/shader/TexturePixelShader.h"

using Sampler = TextureLinearSampler;

using VS = PerspectiveVertexShader<PosUvVertex, PosUvVertex>;
using PS = TexturePixelShader<PosUvVertex, Sampler>;

using CtxType = RenderContext<
	PosUvVertex,
	VS,
	PS
>;

ExampleScene::ExampleScene() : winTitleUpdater( 1.0f / 60.0f ) {
	auto vertices = GeometryGenerator::Cube::GetWrapedVertices( 1.0f );
	auto uvs = GeometryGenerator::Cube::GenerateWrapedUVs();
	
	std::vector<PosUvVertex> cubeVertices;
	auto cubeIndices = GeometryGenerator::Cube::GetWrapedIndices();
	for ( size_t i = 0; i < vertices.size(); i++ ) {
		cubeVertices.push_back( PosUvVertex{ vertices[ i ], uvs[ i ] } );
	}

	pCtx = std::make_unique<CtxType>();
	CtxType &ctx = *static_cast<CtxType *>( pCtx.get() );

	auto &ps = ctx.GetPixelShaderData();
	ps.texture.SetTexture( "bin\\resources\\box_sd.png" );
	ps.sampler.SetUVMode( ITextureSampler::UVMode::WRAP );

	ctx.BindVertexData( cubeVertices );
	ctx.BindIndexData( cubeIndices );
}

void ExampleScene::Update() {
	auto &kb = Keyboard::Get();
	float speed = rotationSpeed;
	if ( kb.IsKeyDown( Keyboard::LeftShift ) )
		speed *= 2;
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

	if ( kb.IsKeyJustPressed( Keyboard::Escape ) )
		App.RequestClose();
}

void ExampleScene::Draw( Graphics &gfx ) {
	ShowFPS();

	gfx.ClearBackground();

	CtxType &ctx = *static_cast<CtxType *>( pCtx.get() );
	ctx.BindViewport( Viewport( gfx.GetWidth(), gfx.GetHeight() ) );

	Matrix4f transform = Matrices::RotationYawPitchRoll4f( yaw, pitch, roll );
	transform = Matrices::Translation4f( 0.0f, 0.0f, 1.5f ) * transform;
	Matrix4f perspecive = Matrices::PerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 1000.0f );
	ctx.BindVertexShaderBuffer( { transform, perspecive } );

	gfx.BindContext( ctx );
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

