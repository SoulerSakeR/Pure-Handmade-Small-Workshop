#include <sol/sol.hpp>
#include "Input/InputDetection.h"

using namespace std;

sol::table InputDetectionLua(sol::state lua) {
	lua.open_libraries(sol::lib::base, sol::lib::package);

    lua.new_usertype<InputDetection>("InputDetection",
        // ��̬��Ա����
        "getKey", &InputDetection::GetKey,
        "getKeyDown", &InputDetection::GetKeyDown,
        "getKeyUp", &InputDetection::GetKeyUp
        );
}