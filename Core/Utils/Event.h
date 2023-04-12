#pragma once
#include <vector>
#include <functional>
/*
template <typename Sender,typename Args >
class Event
{
public:
	void operator()(Sender sender,Args args) const
	{
		for (auto& handle : handles)
		{
			handle(sender,args);
		}
	};
	void Register(const std::function<void(Sender,Args)> f)
	{
		handles.push_back(f);
	};
private:
	std::vector<std::function<void(Sender,Args)>> handles;
};
*/
template <typename RT, typename... Args >
class Event
{
public:
	RT operator()(Args ...args) const
	{
		for (auto& handle : handles)
		{
			return handle(args ...);
		}
	};
	void registerFunc(const std::function<RT(Args...)> f)
	{
		handles.push_back(f);
	};
	void unRegisterFunc(){}
private:
	std::vector<std::function<RT(Args...)>> handles;
};