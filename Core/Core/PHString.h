#pragma once
#include <string>

class PHString
{
private:
    std::string* data;
public:
    PHString(const std::string& src);
    ~PHString();
    void appendLine(const std::string& value);
    void appendLine(const std::string& value0, const std::string& value1);
    void append(const std::string& value);
    void append(const std::string& value0, const std::string& value1);
    void append(const std::string& value0, const std::string& value1,const std::string& value2);
    std::string& str();
};

PHString::PHString(const std::string& src)
{
    data = new std::string(src);
}

PHString::~PHString()
{
    delete data;
}

inline void PHString::appendLine(const std::string &value)
{
    data->append(value);
    data->append("\n");
}

inline void PHString::appendLine(const std::string &value0, const std::string &value1)
{
    this->append(value0);
    this->appendLine(value1);
}

inline void PHString::append(const std::string &value)
{
    data->append(value);
}

inline void PHString::append(const std::string &value0, const std::string &value1)
{
    this->append(value0);
    this->append(value1);
}

inline void PHString::append(const std::string &value0, const std::string &value1, const std::string &value2)
{
    this->append(value0);
    this->append(value1);
    this->append(value2);
}

inline std::string& PHString::str()
{
    std::string& result = *data; 
    return result;
}
