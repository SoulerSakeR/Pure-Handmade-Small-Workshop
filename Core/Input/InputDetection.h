#pragma once
#include <stdlib.h>
#include <Windows.h>



class InputDetection {
public:
	static bool GetKey(int);
	static bool GetKeyDown(int);
	static bool GetKeyUp(int);
	static bool GetMouseButton(int);
	static bool GetMouseButtonDown(int);
	static bool GetMouseButtonUp(int);

};
