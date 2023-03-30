#include "Debug.h"

const std::string& infoPrefix = "[info]";

void Debug::log(const std::string& info)
{
	std::string text = infoPrefix + info;
	auto str = string2Lpwstr(text);
#ifndef NDEBUG 
	OutputDebugString(str);
#else

#endif
	delete str;
}

LPWSTR Debug::string2Lpwstr(const std::string& str)
{
	auto cstr = str.c_str();
	int dwLen = strlen(cstr) + 1;
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, cstr, dwLen, NULL, 0);//算出合适的长度
	LPWSTR result = new WCHAR[dwLen];
	MultiByteToWideChar(CP_UTF8, 0, cstr, dwLen, result, nwLen);
	return result;
}


