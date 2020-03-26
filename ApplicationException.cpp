#include "pch.h"
#include "ApplicationException.h"
#include <sstream>

ApplicationException::ApplicationException( int line, const char *file ) noexcept
	:
	line( line ),
	file( file ) {}

const char *ApplicationException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char *ApplicationException::GetType() const noexcept {
	return "Application Exception";
}

int ApplicationException::GetLine() const noexcept {
	return line;
}

const std::string &ApplicationException::GetFile() const noexcept {
	return file;
}

std::string ApplicationException::GetOriginString() const noexcept {
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}

