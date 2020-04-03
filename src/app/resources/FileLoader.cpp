#include "pch.h"
#include "FileLoader.h"
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