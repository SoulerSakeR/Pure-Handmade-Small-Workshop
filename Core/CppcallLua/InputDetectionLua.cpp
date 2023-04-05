#include "lib/sol/sol.hpp"
#include "Core/Input/InputDetection.h"

using namespace std;

void InputDetectionLua(sol::state lua) {
	lua.open_libraries(sol::lib::base, sol::lib::package);

    lua.new_usertype<InputDetection>("InputDetection",
        // 静态成员函数
        "getKey", &InputDetection::GetKey,
        "getKeyDown", &InputDetection::GetKeyDown,
        "getKeyUp", &InputDetection::GetKeyUp
        );
}