#pragma once
#include "Viewport.h"
#include "RenderPipeline.h"
#include "ClippingCullingUnit.h"

class IRenderContext {

	friend class Graphics;

protected:

	// Pipeline::DrawCall requires the type of the son, not IRenderContext
	virtual void Draw( RenderPipeline &pipeline ) = 0;

};

template<typename VertexT, typename VST, typename PST>
class RenderContext : public IRenderContext {

	using InputVertex = VertexT;
	using VS = VST;
	using PS = PST;
	using VSOut = typename std::result_of<decltype( &VS::operator() )( VS, InputVertex )>::type;
	 
public:

	// could be avoided but it would require more type inference in RenderPipeline
	friend class RenderPipeline;

	RenderContext() : viewport( 0, 0 ) {}
	RenderContext( const VS &vs, const PS &ps ) : vs( vs ), ps( ps ), viewport( 0, 0 ) {}

	void BindVertexData( const std::vector<InputVertex> &vertList ) {
		vertices = vertList;
	}
	void BindIndexData( const std::vector<size_t> &idxList ) {
		indices = idxList;
	}
	void BindViewport( const Viewport &vp ) {
		viewport = vp;
	}
	void BindVertexShaderBuffer( const VS &buffer ) {
		vs = buffer;
	}
	void BindPixelShaderBuffer( const PS &buffer ) {
		ps = buffer;
	}

	const std::vector<InputVertex> &GetVertices() const {
		return vertices;
	}
	const std::vector<size_t> &GetIndices() const {
		return indices;
	}
	const Viewport &GetViewport() const {
		return viewport;
	}
	ClippingCullingUnit<VSOut> &GetClippingCullingUnit() {
		return clipCullUnit;
	}
	VS &GetVertexShaderData() {
		return vs;
	}
	PS &GetPixelShaderData() {
		return ps;
	}

private:

	std::vector<InputVertex> vertices;
	std::vector<size_t> indices;

	VS vs;
	PS ps;

	Viewport viewport;
	ClippingCullingUnit<VSOut> clipCullUnit;

	void Draw( RenderPipeline &pipeline ) override {
		pipeline.DrawCall( *this );
	}

};