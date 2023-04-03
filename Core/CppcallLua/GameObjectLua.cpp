#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp> // or #include "sol.hpp", whichever suits your needs
#include <iostream>
#include "Core/GameObject.h"

using namespace std;


sol::table GameObjectLua(sol::state lua) {
	lua.open_libraries(sol::lib::base);

	/*
	��һ��c++��
	*/

	/*
	lua.new_usertype<func_plus>("func_plus",
		//���캯��
		sol::constructors<func_plus(int, int)>(),
		//��������
		"plus", &func_plus::plus
		);

	//��lua�д���һ��func_plusʾ��
	lua.script("obj=func_plus.new(1,2)");

	// �� Lua �е��ó�Ա����
	lua.script("print(obj:plus())");

	*/

	//��GameObject.hpp��
	lua.new_usertype<GameObject>("GameObject",
		//���캯��
		sol::constructors<GameObject(), GameObject(std::string name)>(),
		//��������
		"isActive", &GameObject::isActive,
		"name", &GameObject::name,
		"id", &GameObject::id,
		"transform", &GameObject::transform,
		"components", &GameObject::components,
		//˽�в���

		//��ͨ����
		"addComponent", sol::overload<Component* (GameObject::*)(ComponentType), void(GameObject::*)(Component*)>(
			&GameObject::addComponent, &GameObject::addComponent),
		"serialize", &GameObject::serialize,
		"deserialize", &GameObject::deserialize
		);

}