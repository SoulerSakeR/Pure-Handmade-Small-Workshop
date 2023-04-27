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
template <typename RT, typename Args >
class Event
{
public:
	Event(){}
	RT operator()(Args args) 
	{
		for (auto& handle : handles)
		{
			std::function<RT(Args)>& f = handle.second;
			f(args);
		}
	};
	template <typename Func,typename Instance>
	void registerFunc(Func func,Instance instance)
	{
		void* function = union_cast<void*>(func);
		void* inst = (void*)instance;
		size_t hash = (size_t)function;
		hash = hash / 2 + (size_t)(inst) / 2;
		handles.emplace(hash, std::bind(func, instance, std::placeholders::_1));
	};
	template <typename Func, typename Instance>
	void unRegisterFunc(Func func, Instance instance)
	{
		void* function = union_cast<void*>(func);
		void* inst = (void*)instance;
		size_t hash = (size_t)(function);
		hash = hash / 2 + (size_t)(inst) / 2;
		handles.erase(hash);
	}
	template<typename dst_type, typename src_type>
	dst_type union_cast(src_type src)
	{
		union {
			src_type s;
			dst_type d;
		}u;
		u.s = src;
		return u.d;
	}
private:
	std::unordered_map<size_t,std::function<RT(Args)>> handles;
};