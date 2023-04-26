#pragma once
#include <string>
#include <unordered_map>

class Component;

class Property
{

public:
	enum PropertyType
	{
		INT,FLOAT,STRING,BOOL,VECTOR2D,COLOR,COMBO_BOX,TEXTURE2D
	};
	
	Property(const std::string& name, void* data,PropertyType type,Component* src);
	~Property();
	template<typename T>
	T& get_data() const
	{
		return *(T*)(data);
	}

	/*template <typename T>
	void set_data(const T& value)
	{
		*(T*)(data) = value;
	}*/

	std::string get_name();
	Component* const get_component();

	PropertyType type;

private:
	
	std::string name;
	void* data;
	Component* component;
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
	}
	void emplace(const key& key, const value& value)
	{
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
