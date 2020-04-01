#pragma once
#include "Viewport.h"
#include "RenderPipeline.h"

class IRenderContext {

	friend class Graphics;

protected:

	virtual void Draw( RenderPipeline &pipeline ) = 0;

};

template<typename Vertex, typename VS, typename PS>
class RenderContext : public IRenderContext {

public:

	friend class RenderPipeline;

	RenderContext() : viewport( 0, 0 ) {}

	void BindVertexData( const std::vector<Vertex> vertList ) {
		vertices = vertList;
	}
	void BindIndexData( const std::vector<size_t> idxList ) {
		indices = idxList;
	}
	void BindViewport( const Viewport vp ) {
		viewport = vp;
	}
	void BindVertexShaderBuffer( const VS buffer ) {
		vs = buffer;
	}
	void BindPixelShaderBuffer( const PS buffer ) {
		ps = buffer;
	}

	const std::vector<Vertex> &GetVertices() {
		return vertices;
	}
	std::vector<Vertex> GetVerticesCopy() {
		return vertices;
	}
	const std::vector<size_t> &GetIndices() {
		return indices;
	}
	std::vector<size_t> GetIndicesCopy() {
		return indices;
	}
	const Viewport &GetViewport() {
		return viewport;
	}
	VS &GetVertexShaderData() {
		return vs;
	}
	PS &GetPixelShaderData() {
		return ps;
	}

private:

	std::vector<Vertex> vertices;
	std::vector<size_t> indices;

	VS vs;
	PS ps;

	Viewport viewport;

	void Draw( RenderPipeline &pipeline ) override {
		pipeline.DrawCall( *this );
	}

};