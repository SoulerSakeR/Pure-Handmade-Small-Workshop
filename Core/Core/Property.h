#pragma once
#include <string>
#include <unordered_map>

#include "Core/Utils/PHEvent.h"
#include "Core/Core/ISerializable.h"

class Component;

class Property
{

public:
	enum PropertyType
	{
		INT,FLOAT,STRING,BOOL,VECTOR2D,COLOR,COMBO_BOX,TEXTURE2D,ANIMATION_COMBOBOX,ANIMATION,SKIN_COMBOBOX,SCRIPT_COMBOBOX
	};
	
	Property(const std::string& name, void* data,PropertyType type,ISerializable* src,bool isVisible = true,bool isEditable = true);
	~Property();
	template<typename T>
	T get_data() const
	{
		auto get_ = static_cast<PHEvent<T, void>*>(get);
		return (*get_)();
	}

	template<typename T,typename GetFunc,typename SetFunc,typename Instance>
	void set_property_func(GetFunc get, SetFunc set,Instance instance )
	{
		if(get!=nullptr)
			set_get<T>(get, instance);
		if(set!=nullptr)
			set_set<T>(set, instance);
	}

	template<typename T,typename Func,typename Instance>
	void set_get(Func f, Instance instance)
	{
		get = new PHEvent<T, void>();
		auto get_= static_cast<PHEvent<T, void>*>(this->get);
		get_->registerFunc(f, instance);
	}
	
	template<typename T, typename Func, typename Instance>
	void set_set(Func f, Instance instance)
	{
		set = new PHEvent<void, T>();
		auto set_ = static_cast<PHEvent<void, T>*>(this->set);
		set_->registerFunc(f, instance);
	}

	template <typename T>
	void set_data(const T& value)
	{
		if (set != nullptr)
		{
			auto set_ = static_cast<PHEvent<void, T>*>(set);
			(*set_)(value);
		}
		else
		{
			*(T*)(data) = value;
		}		
	}

	std::string get_name();
	ISerializable* const get_object();
	bool is_visible;
	bool is_editable;
	PropertyType type;

private:	
	std::string name;
	void* data;
	ISerializable* object;
	
	void* get;
	void* set;

	friend class ISerializable;
};

template<typename key,typename value>
class PropertiesQueue
{
public:
	int size() const
	{
		return properties.size();
	}
	value& operator[](const key& key)
	{
		if (auto it = properties.find(key);it != properties.end())
			return it->second;
		//TODO: throw exception
	}
	void emplace(const key& key, const value& value)
	{
		properties.emplace(key, value);
		values.push_back(value);
	}

	void emplace(const value& value)
	{
		const key& key = value->get_name();
		properties.emplace(key, value);
		values.push_back(value);
	}
	auto begin()
	{
		return properties.begin();
	}
	auto end()
	{
		return properties.end();
	}
	auto vbegin()
	{
		return values.begin();
	}
	auto vend()
	{
		return values.end();
	}

private:
	std::unordered_map<key, value> properties;
	std::vector<value> values;
};
