#include "platform.h"
#include "FileLoader.h"
#pragma warning (push, 0) // remove warnings for OBJ Loader, devs must know what they'r doing
#include "OBJ_Loader.h"
#pragma warning (pop)
#include <fstream>

bool FileLoader::FileExists( const std::string &filename ) {
	struct stat buffer;
	return ( stat( filename.c_str(), &buffer ) == 0 );
}

std::string FileLoader::LoadTextFile( const std::string &filename ) {
	if ( !FileExists( filename ) ) {
		std::string msg = filename + " file could not be found.";
		throw std::exception( msg.c_str() );
	}
	std::stringstream strStream;
	while ( true ) {
		std::ifstream ifs( filename );
		if ( !ifs.is_open() ) {
			std::string msg =
				"Couldn't load text file " + filename + ".\n" +
				"File was actually found but application could not load it's contents.";
			throw std::exception( msg.c_str() );
		}

		strStream << ifs.rdbuf();
		ifs.close();
		break;
	}

	return strStream.str();
}

void LoadIndices( objl::Loader &objLoader, std::vector<size_t> &indices ) {
	const auto &objIndices = objLoader.LoadedIndices;

	indices.reserve( objIndices.size() );
	for ( const auto &idx : objIndices ) {
		indices.push_back( size_t( idx ) );
	}
}

Model<PosVertex> FileLoader::LoadPosModelFromFile( const std::string &path ) {
	Model<PosVertex> model;
	objl::Loader objLoader;
	objLoader.LoadFile( path );

	const auto &objVertices = objLoader.LoadedVertices;
	model.vertices.reserve( objVertices.size() );
	for ( const auto &vert : objVertices ) {
		model.vertices.push_back(
			PosVertex{
				Vector4f( vert.Position.X, vert.Position.Y, vert.Position.Z, 1.0f )
			}
		);
	}

	LoadIndices( objLoader, model.indices );
	return model;
}

Model<PosNVertex> FileLoader::LoadPosNModelFromFile( const std::string &path ) {
	Model<PosNVertex> model;
	objl::Loader objLoader;
	objLoader.LoadFile( path );

	const auto &objVertices = objLoader.LoadedVertices;
	model.vertices.reserve( objVertices.size() );
	for ( const auto &vert : objVertices ) {
		model.vertices.push_back( 
			PosNVertex{ 
				Vector4f( vert.Position.X, vert.Position.Y, vert.Position.Z, 1.0f ),
				Vector3f( vert.Normal.X, vert.Normal.Y, vert.Normal.Z )
			} 
		);
	}

	LoadIndices( objLoader, model.indices );
	return model;
}

Model<PosUvVertex> FileLoader::LoadPosUvModelFromFile( const std::string &path ) {
	Model<PosUvVertex> model;
	objl::Loader objLoader;
	objLoader.LoadFile( path );

	const auto &objVertices = objLoader.LoadedVertices;
	model.vertices.reserve( objVertices.size() );
	for ( const auto &vert : objVertices ) {
		model.vertices.push_back(
			PosUvVertex{
				Vector4f( vert.Position.X, vert.Position.Y, vert.Position.Z, 1.0f ),
				Vector2f( vert.TextureCoordinate.X, vert.TextureCoordinate.Y )
			}
		);
	}

	LoadIndices( objLoader, model.indices );
	return model;
}

Model<PosNUvVertex> FileLoader::LoadPosNUvModelFromFile( const std::string &path ) {
	Model<PosNUvVertex> model;
	objl::Loader objLoader;
	objLoader.LoadFile( path );

	const auto &objVertices = objLoader.LoadedVertices;
	model.vertices.reserve( objVertices.size() );
	for ( const auto &vert : objVertices ) {
		model.vertices.push_back( 
			PosNUvVertex{ 
				Vector4f( vert.Position.X, vert.Position.Y, vert.Position.Z, 1.0f ),
				Vector3f( vert.Normal.X, vert.Normal.Y, vert.Normal.Z ),
				Vector2f( vert.TextureCoordinate.X, vert.TextureCoordinate.Y)
			} 
		);
	}

	LoadIndices( objLoader, model.indices );
	return model;
}