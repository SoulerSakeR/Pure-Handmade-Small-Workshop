#include "Debug.h"
#include "Core/Core/PHString.h"
#include "Core/Utils/Test.h"
#include <iostream>
#include "qmessagebox.h"

const std::string& infoPrefix = "[info] ";
const std::string& errorPrefix = "[error] ";

Debug::Debug(bool isInfo, bool isError)
{
	this->isInfo = isInfo;
	this->isError = isError;
}

Debug Debug::log(const std::string& info)
{	
	if (info.empty())
		return Debug();
#ifndef NDEBUG 
	std::string text = info;
#ifdef LOG_TO_CONSOLE	
	Log2Console(text);
#endif // LOG_TO_CONSOLE

#ifdef LOG_TO_OUTPUT_WINDOW
	Log2OutputWindow(text);
#endif // LOG_TO_OUTPUT_WINDOW

#endif // NDEBUG
	
}

Debug Debug::logError(const std::string& error)
{
	if(error.empty())
		return Debug(false,true);
	log(errorPrefix + error);
}

Debug Debug::logInfo(const std::string& info)
{
	if(info.empty())
		return Debug(true,false);
	log(infoPrefix + info);
}

void Debug::warningBox(QWidget* parent, const std::string& info)
{
	QMessageBox::warning(parent, QString::fromStdString("Warning"), QString::fromStdString(info), QMessageBox::Ok);
}

Debug& Debug::operator<<(const std::string& info)
{
	if (usePrefix)
	{
		if (isError)
			logError(info);
		else if (isInfo)
			logInfo(info);
		usePrefix = false;
	}
	else
		log(info);
	return *this;
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

