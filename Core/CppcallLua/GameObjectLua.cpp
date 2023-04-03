#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp> // or #include "sol.hpp", whichever suits your needs
#include <iostream>
#include "Core/GameObject.h"

using namespace std;


sol::table GameObjectLua(sol::state lua) {
	lua.open_libraries(sol::lib::base);

	/*
	绑定一个c++类
	*/

	/*
	lua.new_usertype<func_plus>("func_plus",
		//构造函数
		sol::constructors<func_plus(int, int)>(),
		//其他函数
		"plus", &func_plus::plus
		);

	//在lua中创造一个func_plus示例
	lua.script("obj=func_plus.new(1,2)");

	// 在 Lua 中调用成员函数
	lua.script("print(obj:plus())");

	*/

	//绑定GameObject.hpp类
	lua.new_usertype<GameObject>("GameObject",
		//构造函数
		sol::constructors<GameObject(), GameObject(std::string name)>(),
		//公共参数
		"isActive", &GameObject::isActive,
		"name", &GameObject::name,
		"id", &GameObject::id,
		"transform", &GameObject::transform,
		"components", &GameObject::components,
		//私有参数

		//普通函数
		"addComponent", sol::overload<Component* (GameObject::*)(ComponentType), void(GameObject::*)(Component*)>(
			&GameObject::addComponent, &GameObject::addComponent),
		"serialize", &GameObject::serialize,
		"deserialize", &GameObject::deserialize
		);

}