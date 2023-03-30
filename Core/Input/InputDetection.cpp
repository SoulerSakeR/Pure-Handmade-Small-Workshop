#include "InputDetection.h"

bool InputDetection::GetKey(int keyCode)
{
    return (GetAsyncKeyState(keyCode) & 0x8000) != 0;
}

bool InputDetection::GetKeyDown(int keyCode)
{
    static bool wasPressed[256] = { false };

    bool isPressed = (GetAsyncKeyState(keyCode) & 0x8000) != 0;
    bool result = !wasPressed[keyCode] && isPressed;

    wasPressed[keyCode] = isPressed;
    return result;
}

bool InputDetection::GetKeyUp(int keyCode)
{
    static bool wasPressed[256] = { false };

    bool isPressed = (GetAsyncKeyState(keyCode) & 0x8000) != 0;
    bool result = wasPressed[keyCode] && !isPressed;

    wasPressed[keyCode] = isPressed;
    return result;
}


