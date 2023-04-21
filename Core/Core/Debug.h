#pragma once
#include <string>
#include <Windows.h>
#include <qwidget.h>

class Debug
{
public:
	Debug(bool isInfo=false ,bool isError=false);
	static Debug log(const std::string& mes = ""); //需要绑定
	static Debug logError(const std::string& info = ""); //需要绑定
	static Debug logInfo(const std::string& info = ""); //需要绑定
	static void warningBox(QWidget* parent, const std::string& info);
	virtual Debug& operator<<(const std::string& info);
protected:
	static void Log2Console(const std::string& info);
	static void Log2OutputWindow(const std::string& str);
	static LPWSTR string2Lpwstr(const std::string& str);
	bool usePrefix = true;
	bool isInfo = false;
	bool isError = false;
};






