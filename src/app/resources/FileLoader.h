#pragma once
#include <string>

namespace FileLoader {

	bool FileExists( const std::string &filename );
	std::string LoadTextFile( const std::string &filename );

	// TODO : Create loadBinaryFile... maybe later create json files functions too

}