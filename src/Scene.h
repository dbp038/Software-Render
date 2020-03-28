#pragma once
#include "Graphics.h"

class Scene {

public:

	Scene() = default;
	Scene( const Scene & ) = delete;
	Scene &operator=( const Scene & ) = delete;

	virtual void Update() {}
	virtual void Draw( Graphics &gfx ) {}

};