#pragma once

template<typename Vertex>
class ClippingCullingUnit {

public:

	void SetInitialTriangle( const Vertex &v0, const Vertex &v1, const Vertex &v2 ) {
		polygon[ 0 ] = { v0, 2, 1 };
		polygon[ 1 ] = { v1, 0, 2 };
		polygon[ 2 ] = { v2, 1, 0 };
		polygonSize = 3;
		
		ResetTriangle();
	}
	void ResetTriangle() {
		nextTriangleVertex = 0;
	}

	void ClipCullNegativeXAxis( float threshold ) {
		ClipCullWithAxis( X_AXIS, threshold, lessThan, greaterThan );
	}
	void ClipCullPositiveXAxis( float threshold ) {
		ClipCullWithAxis( X_AXIS, threshold, greaterThan, lessThan );
	}
	void ClipCullNegativeYAxis( float threshold ) {
		ClipCullWithAxis( Y_AXIS, threshold, lessThan, greaterThan );
	}
	void ClipCullPositiveYAxis( float threshold ) {
		ClipCullWithAxis( Y_AXIS, threshold, greaterThan, lessThan );
	}
	void ClipCullNegativeZAxis( float threshold ) {
		ClipCullWithAxis( Z_AXIS, threshold, lessThan, greaterThan );
	}
	void ClipCullPositiveZAxis( float threshold ) {
		ClipCullWithAxis( Z_AXIS, threshold, greaterThan, lessThan );
	}
	bool IsCurrentTriangleBackfaced() {
		int nextVertex = 0;
		const Vector3f *pV0 = reinterpret_cast<const Vector3f *>( &polygon[ nextVertex ].vertex.position );
		nextVertex = polygon[ nextTriangleVertex ].next;
		const Vector3f *pV1 = reinterpret_cast<const Vector3f *>( &polygon[ nextVertex ].vertex.position );
		nextVertex = polygon[ nextVertex ].next;
		const Vector3f *pV2 = reinterpret_cast<const Vector3f *>( &polygon[ nextVertex ].vertex.position );
		// all polygon vertices share the same plane so we only need first three
		return ( *pV1 - *pV0 ).cross( *pV2 - *pV0 ).dot( Vector3f( 0.0f, 0.0f, 1.0f ) ) >= 0.0f;
	}

	int GetPolygonSize() const {
		return polygonSize;
	}
	Vertex &GetPolygonVertex( int id ) {
		return polygon[ id ].vertex;
	}
	int GetTriangleCount() const {
		return polygonSize - 2;
	}
	void GetCurrentTriangle( Vertex &v0, Vertex &v1, Vertex &v2 ) {
		// using fan triangulation
		int nextVertex = 0;
		v0 = polygon[ nextVertex ].vertex;
		nextVertex = polygon[ nextTriangleVertex ].next;
		v1 = polygon[ nextVertex ].vertex;
		nextVertex = polygon[ nextVertex ].next;
		v2 = polygon[ nextVertex ].vertex;
	}
	void NextTriangle() {
		nextTriangleVertex = polygon[ nextTriangleVertex ].next;
	}

private:

	static constexpr int X_AXIS = 0, Y_AXIS = 1, Z_AXIS = 2;

	static std::greater<float> greaterThan;
	static std::less<float> lessThan;

	struct PolygonVertex {
		Vertex vertex;
		int previous, next;
	};

	std::array<PolygonVertex, 7> polygon;
	int polygonSize = 0;
	int nextTriangleVertex = 0;

	void AddVertexToPolygon( const Vertex &vtx, int previousNeighbour ) {
		// add to last position in the vertices array
		PolygonVertex &newVtx = polygon[ polygonSize ];
		newVtx.vertex = vtx;
		// insert into the polygon logic structure
		newVtx.next = polygon[ previousNeighbour ].next;
		newVtx.previous = previousNeighbour;
		polygon[ previousNeighbour ].next = polygonSize;
		polygon[ newVtx.next ].previous = polygonSize;

		polygonSize++;
	}
	void RemoveFromPolygon( int position ) {
		// actual removal
		// remove from the polygon logic structure
		polygon[ polygon[ position ].previous ].next = polygon[ position ].next;
		polygon[ polygon[ position ].next ].previous = polygon[ position ].previous;
		// remove from the array of vertices
		polygonSize--;
		if ( position < polygonSize ) {
			// move last vertex to the position of the recently removed one so there isn't
			// any blank spaces in the array
			polygon[ position ] = polygon[ polygonSize ];
			// inform the polygon of the recently moved vertex
			polygon[ polygon[ position ].next ].previous = position;
			polygon[ polygon[ position ].previous ].next = position;
		}
	}

	Vertex GetCuttingPoint( int axis, int threshold, const Vertex &v1, const Vertex &v2 ) {
		return VertexLerp(
			v1, v2, 
			( threshold - v1.position[axis] ) / ( v2.position[axis] - v1.position[ axis ] )
		);
	}

	template<typename CmpType1, typename CmpType2>
	void ClipCullWithAxis( int axis, float threshold, const CmpType1 &isOutsideFunction, const CmpType2 &isInsideFunction ) {
		float neighbourCoord;
		int size = polygonSize;

		for ( int i = 0; i < size; i++ ) {
			PolygonVertex &currentVtx = polygon[ i ];
			// is vertex off limits?
			if ( isOutsideFunction( currentVtx.vertex.position[ axis ], threshold ) ) {

				// is next neighbour in polygon inside the screen?
				neighbourCoord = polygon[ currentVtx.next ].vertex.position[ axis ];
				if ( isInsideFunction( neighbourCoord, threshold ) ) {
					
					// is previous neighbour in polygon inside the screen TOO?
					neighbourCoord = polygon[ currentVtx.previous ].vertex.position[ axis ];
					if ( isInsideFunction( neighbourCoord, threshold ) ) {
						// we need to create a new vertex in this case with valid values (i.e. inside the screen)
						AddVertexToPolygon(
							GetCuttingPoint( axis, threshold,
								currentVtx.vertex,
								polygon[ currentVtx.previous ].vertex
							), currentVtx.previous
						);
					}

					// invalidate current vertex and overwite it with one inside the limits of the screen
					currentVtx.vertex = GetCuttingPoint( axis, threshold,
						currentVtx.vertex, polygon[ currentVtx.next ].vertex );
				}
				// is previous neighbour in polygon inside the screen?
				else if ( isInsideFunction( polygon[ currentVtx.previous ].vertex.position[ axis ], threshold ) ) {
					// we can only interpolate with one of the two neighbours
					// invalidate vertex and overwite it with one inside the limits of the screen
					currentVtx.vertex = GetCuttingPoint( axis, threshold,
						currentVtx.vertex, polygon[ currentVtx.previous ].vertex );
				}
				// all vertices are outside, current vertex is not necessary, cull it
				else {
					// if those were the last vertices, triangle is totally out of the screen
					// no need to process it further
					if ( polygonSize == 3 ) {
						polygonSize = 0;
						return;
					}
					RemoveFromPolygon( i );
					// we don't want to process the removed vertex
					size--;
					i--;
				}
			}
		}
	}

};

template<typename Vertex>
__declspec( selectany ) std::less<float> ClippingCullingUnit<Vertex>::lessThan;

template<typename Vertex>
__declspec( selectany ) std::greater<float> ClippingCullingUnit<Vertex>::greaterThan;
