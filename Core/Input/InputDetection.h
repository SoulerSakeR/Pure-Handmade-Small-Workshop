#pragma once
#include <stdlib.h>
#include <Windows.h>
#include <vector>



class InputDetection {
public:
	static bool GetKey(int);
	static bool GetKeyDown(int);
	static bool GetKeyUp(int);

};
