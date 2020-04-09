#pragma once
#include "Model.h"
#include "gfx/context/VertexDeclarations.h"

namespace FileLoader {

	bool FileExists( const std::string &filename );
	std::string LoadTextFile( const std::string &filename );

	Model<PosNVertex> LoadPosNModelFromFile( const std::string &path );
	Model<PosNUvVertex> LoadPosNUvModelFromFile( const std::string &path );

}