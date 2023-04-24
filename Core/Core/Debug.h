#pragma once
#include <string>
#include <Windows.h>
#include <qwidget.h>
#include <Core/Utils/Vector2D.h>

class Debug
{
public:
	enum class LogLevel
	{
		INFO,WARNING,ERROR_
	};
	Debug(LogLevel level);
	static Debug logWarning(const std::string& info = ""); //需要绑定
	static Debug logError(const std::string& info = ""); //需要绑定
	static Debug logInfo(const std::string& info = ""); //需要绑定
	static void warningBox(QWidget* parent, const std::string& info);
	virtual Debug& operator<<(const char* info);
	virtual Debug& operator<<(const std::string& info);
	virtual Debug& operator<<(const QString& info);
	virtual Debug& operator<<(int info);
	virtual Debug& operator<<(float info);
	virtual Debug& operator<<(Vector2D info);
protected:
	static void Log2Console(const std::string& info);
	static void Log2OutputWindow(const std::string& str);
	static void log(const std::string& mes = ""); //需要绑定
	static LPWSTR string2Lpwstr(const std::string& str);
	LogLevel level;
	bool usePrefix = true;
};






