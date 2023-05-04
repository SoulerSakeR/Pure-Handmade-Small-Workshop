#include "Debug.h"
#include "Core/Core/PHString.h"
#include "Core/Utils/Test.h"
#include <iostream>
#include "qmessagebox.h"
#include "Core/UI/mainwindow.h"
#include "Core/SystemStatus/GameEngine.h"
#include "Core/FileIO/IO.h"

const std::string& INFO_PREFIX = "[info] ";
const std::string& ERROR_PREFIX = "[error] ";
const std::string& WARNING_PREFIX = "[warning] ";
Debug::LogLevel Debug::logLevel = LogLevel::WARNING;

Debug::Debug(LogLevel level):level(level)
{

}

Debug Debug::logWarning(const std::string& info)
{
	if (info.empty())
		return Debug(LogLevel::WARNING);
	log(WARNING_PREFIX + info, LogLevel::WARNING);
}

void Debug::log(const std::string& info,LogLevel level)
{	
	std::string text = info;	
	if (logLevel <= level)
	{
		Log2Console(text);
		if (GameEngine::get_instance().getInEditor()) {
			if (MainWindow::getConsoleTextEdit() != nullptr)
				QMetaObject::invokeMethod(MainWindow::getConsoleTextEdit(), "insertPlainText",Qt::QueuedConnection,Q_ARG(QString, QString::fromStdString(text)));
		}
		log2File(text);
	}	
}

Debug Debug::logError(const std::string& error)
{
	if(error.empty())
		return Debug(LogLevel::ERROR_);
	log(ERROR_PREFIX + error, LogLevel::ERROR_);
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
	if (usePrefix)
	{
		if (level == LogLevel::ERROR_)
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

Debug& Debug::operator<<(const std::string& info)
{
	std::string text = '\"'+ info+'\"';
	if (usePrefix)
	{
		if(level==LogLevel::ERROR_)
			logError(text);
		else if (level == LogLevel::INFO)
			logInfo(text);
		else if (level == LogLevel::WARNING)
			logWarning(text);
		usePrefix = false;
	}
	else
		log(text);
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

void Debug::log2File(const std::string& info)
{
	IO::write(info, PHPath(GameEngine::get_instance().getRootPath()).combinePath("Debug.log").getNewPath(), 2);
}

void Debug::Log2OutputWindow(const std::string& text)
{
	auto str = string2Lpwstr(text);
	OutputDebugString(str);
	delete str;
}

