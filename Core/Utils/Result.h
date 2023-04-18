#pragma once
template <typename T>
class Result
{
public:
	Result(bool result=true, T data=nullptr,const std::string& message="") : result(result), data(data), message(message) {}
	Result(bool result,const std::string& message) : result(result),data(nullptr),message(message) {}
	bool result;
	T data;
	std::string message;
};

