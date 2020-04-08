#pragma once
#include "Viewport.h"
#include "gfx/core/RenderPipeline.h"
#include "ClippingCullingUnit.h"
#include "gfx/shader/DefaultGeometryShader.h"

class IRenderContext {

public:

	friend class Graphics;

	template<typename InputVertex, typename VS>
	using DefaultGS = DefaultGeometryShader<
		typename std::result_of<decltype( &VS::operator() )( VS, InputVertex )>::type,
		typename std::result_of<decltype( &VS::operator() )( VS, InputVertex )>::type>;

protected:

	// Pipeline::DrawCall requires the type of the son, not IRenderContext
	virtual void Draw( RenderPipeline &pipeline ) = 0;

};

template< typename VertexT, typename VST, typename PST, 
typename GST = IRenderContext::DefaultGS<VertexT, VST> >
class RenderContext : public IRenderContext {

	using InputVertex = VertexT;
	using VS = VST;
	using PS = PST;
	using GS = GST;
	using VSOut = typename std::result_of<decltype( &VS::operator() )( VS, InputVertex )>::type;
	using GSOut = typename std::result_of<decltype( &GS::operator() )( GS, size_t, VSOut, VSOut, VSOut )>::type::value_type;
	 
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
	ClippingCullingUnit<GSOut> &GetClippingCullingUnit() {
		return clipCullUnit;
	}
	VS &GetVertexShaderData() {
		return vs;
	}
	GS &GetGeometryShaderData() {
		return gs;
	}
	PS &GetPixelShaderData() {
		return ps;
	}

private:

	std::vector<InputVertex> vertices;
	std::vector<size_t> indices;

	VS vs;
	GS gs;
	PS ps;

	int triangleId = 0;

	Viewport viewport;
	ClippingCullingUnit<GSOut> clipCullUnit;

	void Draw( RenderPipeline &pipeline ) override {
		pipeline.DrawCall( *this );
	}

};