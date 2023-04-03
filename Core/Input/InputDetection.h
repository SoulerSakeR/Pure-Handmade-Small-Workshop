#pragma once
#include <stdlib.h>
#include <Windows.h>

class InputDetection {
public:
	static bool GetKey(int); // 某个键是否按住 需要绑定
	static bool GetKeyDown(int);// 某个键是否按下 需要绑定
	static bool GetKeyUp(int);// 某个键是否松开 需要绑定
};
