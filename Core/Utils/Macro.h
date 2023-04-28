#pragma once

#define SINGLETON(Class) \
public: \
	static Class* get_instance(){\
		if(instance==nullptr)\
			instance = new Class();\
		return instance;\
	}\
private:\
	Class(){}\
	NO_COPY_AND_ASSIGN(Class)\
	static Class* instance;\

#define SINGLETON_IMPL(Class) Class* Class::instance = nullptr;
											
#define NO_COPY_AND_ASSIGN(Class) \
	Class(const Class&) = delete;\
	Class& operator=(const Class&) = delete;