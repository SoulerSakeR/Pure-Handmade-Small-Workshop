#include "PHString.h"

PHString::PHString(const std::string& src)
{
    data = new std::string(src);
}

PHString::~PHString()
{
    delete data;
}

void PHString::appendLine(const std::string &value)
{
    data->append(value);
    data->append("\n");
}

void PHString::appendLine(const std::string &value0, const std::string &value1)
{
    this->append(value0);
    this->appendLine(value1);
}

void PHString::append(const std::string &value)
{
    data->append(value);
}

void PHString::append(const std::string &value0, const std::string &value1)
{
    this->append(value0);
    this->append(value1);
}

void PHString::append(const std::string &value0, const std::string &value1, const std::string &value2)
{
    this->append(value0);
    this->append(value1);
    this->append(value2);
}

std::string& PHString::str()
{
    std::string& result = *data; 
    return result;
}
