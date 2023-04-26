#include "Debug.h"
#include "Core/Core/PHString.h"
#include "Core/Utils/Test.h"
#include <iostream>
#include "qmessagebox.h"
#include "Core/UI/mainwindow.h"

const std::string& INFO_PREFIX = "[info] ";
const std::string& ERROR_PREFIX = "[error] ";
const std::string& WARNING_PREFIX = "[warning] ";

Debug::Debug(LogLevel level):level(level)
{

}

Debug Debug::logWarning(const std::string& info)
{
	if (info.empty())
		return Debug(LogLevel::WARNING);
	log(WARNING_PREFIX + info);
}

void Debug::log(const std::string& info)
{	
#ifndef NDEBUG 
	std::string text = info;
#ifdef LOG_TO_CONSOLE	
	Log2Console(text);
	//这里如果不注释掉会报错，光夫哥来看看什么情况
	if(MainWindow::getConsoleTextEdit()!=nullptr)
		MainWindow::getConsoleTextEdit()->insertPlainText(QString::fromStdString(text));
#endif // LOG_TO_CONSOLE

#ifdef LOG_TO_OUTPUT_WINDOW
	Log2OutputWindow(text);
#endif // LOG_TO_OUTPUT_WINDOW

#endif // NDEBUG
	
}

Debug Debug::logError(const std::string& error)
{
	if(error.empty())
		return Debug(LogLevel::ERROR_);
	log(ERROR_PREFIX + error);
}

Debug Debug::logInfo(const std::string& info)
{
	if(info.empty())
		return Debug(LogLevel::INFO);
	log(INFO_PREFIX + info);
}

void Debug::warningBox(QWidget* parent, const std::string& info)
{
	QMessageBox::warning(parent, QString::fromStdString("Warning"), QString::fromStdString(info), QMessageBox::Ok);
}

Debug& Debug::operator<<(const char* info)
{
	return operator<<(std::string(info));
}

Debug& Debug::operator<<(const std::string& info)
{
	if (usePrefix)
	{
		if(level==LogLevel::ERROR_)
			logError(info);
		else if (level == LogLevel::INFO)
			logInfo(info);
		else if (level == LogLevel::WARNING)
			logWarning(info);
		usePrefix = false;
	}
	else
		log(info);
	return *this;
}

Debug& Debug::operator<<(const QString& info)
{
	return operator<<(info.toStdString());
}

Debug& Debug::operator<<(int info)
{
	return operator<<(std::to_string(info));
}

Debug& Debug::operator<<(float info)
{
	return operator<<(std::to_string(info));
}

Debug& Debug::operator<<(Vector2D info)
{
	return operator<<(info.tostring());
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

