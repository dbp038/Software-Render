#include "pch.h"
#include "ExampleScene.h"
#include "Viewport.h"
#include "Mouse.h"

ExampleScene::ExampleScene() : cube( 1.0f ) {}

void ExampleScene::Update() {

}

void ExampleScene::Draw( Graphics &gfx ) {
	Viewport screenTransformer( gfx.GetScreenSize() );
	auto lines = cube.GetLines();
	for ( auto &vert : lines.vertices ) {
		// add some value to z, for now we ignore z axis but later we'll not.
		vert += Vector3f( 0.0f, 0.0f, 2.0f );
		screenTransformer.Transform( vert );
	}
	auto &indices = lines.indices;
	for ( auto i = lines.indices.cbegin(),
		end = lines.indices.cend();
		i != end; std::advance( i, 2 ) )
	{
		gfx.DrawLine( lines.vertices[ *i ], lines.vertices[ *std::next( i ) ], Colors::White );
	}
}

