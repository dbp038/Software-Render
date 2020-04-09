#include "pch.h"
#include "ExampleScene.h"
#include "GeometryGenerator.h"
#include "app/Application.h"
#include "gfx/context/Viewport.h"

#include "gfx/context/TextureBitmap.h"
#include "gfx/context/VertexDeclarations.h"
#include "gfx/context/TextureSampler.h"
#include "gfx/shader/PerspectiveVertexShader.h"
#include "gfx/shader/FaceColorGeometryShader.h"
#include "gfx/shader/FaceNormalsGeometryShader.h"
#include "gfx/shader/TexturePixelShader.h"
#include "gfx/shader/VertexColorPixelShader.h"
#include "gfx/shader/BasicIlluminationPixelShader.h"

using VertexType = PosUvVertex;
using Sampler = TextureLinearSampler;

using VS = PerspectiveVertexShader<VertexType, VertexType>;
using GS = FaceNormalsGeometryShader;
using PS = BasicIlluminationPixelShader<TextureLinearSampler>;
//using PS = TexturePixelShader<VertexType, Sampler>;

using CtxType = RenderContext<
	VertexType,
	VS,
	PS,
	GS
>;

ExampleScene::ExampleScene() : winTitleUpdater( 1.0f / 23.0f ) {
	int width = App.GetWindow().GetWidth();
	int height = App.GetWindow().GetHeight();

	auto vertices = GeometryGenerator::Cube::GetWrapedVertices( 1.0f );
	auto colors = GeometryGenerator::Cube::GetColors();
	auto uvs = GeometryGenerator::Cube::GenerateWrapedUVs();
	
	std::vector<VertexType> cubeVertices;
	auto cubeIndices = GeometryGenerator::Cube::GetWrapedIndices();
	for ( size_t i = 0; i < vertices.size(); i++ ) {
		cubeVertices.push_back( VertexType{ vertices[ i ], uvs[ i ] } );
	}

	pCtx = std::make_unique<CtxType>();
	CtxType &ctx = *static_cast<CtxType *>( pCtx.get() );

	auto &ps = ctx.GetPixelShaderData();
	ps.texture.SetTexture( "bin\\resources\\box_hd.png" );
	ps.sampler.SetUVMode( ITextureSampler::UVMode::WRAP );

	ctx.BindViewport( Viewport( width, height ) );

	ctx.BindVertexData( cubeVertices );
	ctx.BindIndexData( cubeIndices );

	Matrix4f perspecive = Matrices::PerspectiveLH( 1.0f, float( height ) / width, 0.5f, 1000.0f );
	ctx.BindVertexShaderBuffer( { Matrix4f::Identity(), perspecive } );
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

	if ( kb.IsKeyDown( Keyboard::I ) ) {
		yOffset += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::K ) ) {
		yOffset -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::L ) ) {
		xOffset += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::J ) ) {
		xOffset -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::U ) ) {
		zOffset += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::O ) ) {
		zOffset -= Time.delta * speed;
	}

	if ( kb.IsKeyJustPressed( Keyboard::Escape ) )
		App.RequestClose();
}

void ExampleScene::Draw( Graphics &gfx ) {
	ShowFPS();

	gfx.ClearBackground();

	CtxType &ctx = *static_cast<CtxType *>( pCtx.get() );

	auto &psBuffer = ctx.GetVertexShaderData();
	psBuffer.transformMatrix = Matrices::RotationYawPitchRoll4f( yaw, pitch, roll );
	psBuffer.transformMatrix = Matrices::Translation4f( xOffset, yOffset, zOffset + 3.0f ).inverse() * psBuffer.transformMatrix;
	ctx.BindVertexShaderBuffer( psBuffer ); // not technically needed

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

