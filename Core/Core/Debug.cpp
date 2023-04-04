#include "Debug.h"
#include "Core/Core/PHString.h"
#include "Core/Utils/Test.h"
#include <iostream>

const std::string& infoPrefix = "[info] ";
const std::string& errorPrefix = "[error] ";

void Debug::log(const std::string& info)
{	
#ifndef NDEBUG 
	std::string text = infoPrefix + info;
	if (text.back() != '\n')
		text.append("\n");
#ifdef LOG_TO_CONSOLE	
	Log2Console(text);
#endif // LOG_TO_CONSOLE

#ifdef LOG_TO_OUTPUT_WINDOW
	Log2OutputWindow(text);
#endif // LOG_TO_OUTPUT_WINDOW

#endif // NDEBUG
	
}



void Debug::logError(std::string& errorInfo)
{
	log(errorPrefix + errorInfo);
}

LPWSTR Debug::string2Lpwstr(const std::string& str)
{
	auto cstr = str.c_str();
	int dwLen = strlen(cstr) + 1;
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, cstr, dwLen, NULL, 0);
	LPWSTR result = new WCHAR[dwLen];
	MultiByteToWideChar(CP_UTF8, 0, cstr, dwLen, result, nwLen);
	return result;
}

void Debug::Log2Console(const std::string& info)
{
	std::cout << info;
}

void Debug::Log2OutputWindow(const std::string& text)
{
	auto str = string2Lpwstr(text);
	OutputDebugString(str);
	delete str;
}


