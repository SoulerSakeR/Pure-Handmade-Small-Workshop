#pragma once
#include <string>
#include <Windows.h>
#include <qwidget.h>

class Debug
{
public:
	static void log(const std::string& mes); //需要绑定
	//static void log(const char* info);
	static void logError(const std::string&); //需要绑定
	static void logInfo(const std::string&); //需要绑定
	static void warningBox(QWidget* parent ,  const std::string& info);

private:
	static void Log2Console(const std::string& info);
	static void Log2OutputWindow(const std::string& str);
	static LPWSTR string2Lpwstr(const std::string& str);
};

