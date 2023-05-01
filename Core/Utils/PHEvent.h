#pragma once

#include <vector>
#include <functional>

//template <typename RT, typename ... Args>
//class PHEvent
//{
//public:
//	RT operator()()
//	{
//		for (auto& handle : handles)
//		{
//			std::function<RT()>& f = handle.second;
//			return f();
//		}
//	};
//	template <typename Func, typename Instance>
//	void registerFunc(Func func, Instance instance)
//	{
//		void* function = union_cast<void*>(func);
//		void* inst = (void*)instance;
//		size_t hash = (size_t)function;
//		hash = hash / 2 + (size_t)(inst) / 2;
//		handles.emplace(hash, std::bind(func, instance));
//	};
//
//	template <typename Func, typename Instance>
//	void unRegisterFunc(Func func, Instance instance)
//	{
//		void* function = union_cast<void*>(func);
//		void* inst = (void*)instance;
//		size_t hash = (size_t)(function);
//		hash = hash / 2 + (size_t)(inst) / 2;
//		handles.erase(hash);
//	}
//
//	template<typename dst_type, typename src_type>
//	dst_type union_cast(src_type src)
//	{
//		union {
//			src_type s;
//			dst_type d;
//		}u;
//		u.s = src;
//		return u.d;
//	}
//private:
//	std::unordered_map<size_t, std::function<RT()>> handles;
//};


template <typename RT, typename Args >
class PHEvent
{
	using handle_type = std::function<RT(Args)>;
public:
	const std::vector<RT>& operator()(Args args) 
	{
		std::vector<RT> results;
		for (auto& handle : handles)
		{
			std::function<RT(Args)>& f = handle.second;
			results.push_back(f(args));
		}
		return results;
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
	};

	template<typename dst_type, typename src_type>
	dst_type union_cast(src_type src)
	{
		union {
			src_type s;
			dst_type d;
		}u;
		u.s = src;
		return u.d;
	};
private:
	std::unordered_map<size_t,std::function<RT(Args)>> handles;
};

template <typename RT>
class PHEvent<RT,void>
{
public:
	RT operator()()
	{
		for (auto& handle : handles)
		{
			std::function<RT()>& f = handle.second;
			return f();
		}
	};
	template <typename Func, typename Instance>
	void registerFunc(Func func, Instance instance)
	{
		void* function = union_cast<void*>(func);
		void* inst = (void*)instance;
		size_t hash = (size_t)function;
		hash = hash / 2 + (size_t)(inst) / 2;
		handles.emplace(hash, std::bind(func, instance));
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
	std::unordered_map<size_t, std::function<RT()>> handles;
};

template <typename Args >
class PHEvent<void,Args>
{
	using handle_type = std::function<void(Args)>;
public:
	void operator()(Args args)
	{
		for (auto& handle : handles)
		{
			handle_type& f = handle.second;
			f(args);
		}
	};
	template <typename Func, typename Instance>
	void registerFunc(Func func, Instance instance)
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
	};

	template<typename dst_type, typename src_type>
	dst_type union_cast(src_type src)
	{
		union {
			src_type s;
			dst_type d;
		}u;
		u.s = src;
		return u.d;
	};
private:
	std::unordered_map<size_t, std::function<void(Args)>> handles;
};