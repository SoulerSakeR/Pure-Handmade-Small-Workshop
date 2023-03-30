#pragma once
#include <string>

class PHString
{
private:
    std::string* data;
public:
    PHString(const std::string& src="");
    PHString(const PHString& src);
    ~PHString();
    void appendLine(const std::string& value);
    void appendLine(const std::string& value0, const std::string& value1);
    void append(const std::string& value);
    void append(const std::string& value0, const std::string& value1);
    void append(const std::string& value0, const std::string& value1,const std::string& value2);
    std::string& str();
};
