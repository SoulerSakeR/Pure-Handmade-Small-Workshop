#include "Debug.h"
#include <Core/Core/PHString.h>

const std::string& infoPrefix = "[info] ";
const std::string& errorPrefix = "[error] ";

void Debug::log(const std::string& info)
{	
#ifndef NDEBUG 
	std::string text = infoPrefix + info;
	if (text.back() != '\n')
		text.append("\n");
	auto str = string2Lpwstr(text);
	OutputDebugString(str);
	delete str;
#else

#endif
	
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


