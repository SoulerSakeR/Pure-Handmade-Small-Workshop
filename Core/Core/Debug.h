#pragma once
#include <string>
#include <Windows.h>

class Debug
{
public:
	static void log(const std::string& info); //需要绑定
	//static void log(const char* info);
	static void logError(std::string&); //需要绑定
	

private:
	static void Log2Console(const std::string& info);
	static void Log2OutputWindow(const std::string& str);
	static LPWSTR string2Lpwstr(const std::string& str);
};

