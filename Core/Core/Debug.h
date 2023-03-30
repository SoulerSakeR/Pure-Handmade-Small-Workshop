#pragma once
#include <string>
#include <Windows.h>
class Debug
{
public:
	static void log(const std::string& info);
	//static void log(const char* info);
	static void logError(std::string&);
	static LPWSTR string2Lpwstr(const std::string& str);
};

