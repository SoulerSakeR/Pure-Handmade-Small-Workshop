#pragma once
#include <string>

class Component;

class Property
{

public:
	enum PropertyType
	{
		INT,FLOAT,STRING,BOOL,VECTOR2D
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

