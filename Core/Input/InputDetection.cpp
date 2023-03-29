#include "InputDetection.h"
static bool wasPressed = false;

bool InputDetection::GetKey(int keyCode)
{
    return GetAsyncKeyState(keyCode) & 0x8000;
}
bool InputDetection::GetKeyDown(int keyCode)
{
    bool isPressed = GetKey(keyCode);
    if (wasPressed == false && isPressed)
    {
        wasPressed = isPressed;
        return true;
    }
    else
    {
        wasPressed = isPressed;
        return false;
    }
}
// ���ĳ�����Ƿ��ɿ�
bool InputDetection::GetKeyUp(int keyCode)
{

    static bool wasPressed = false;
    bool isPressed = GetKey(keyCode);
    bool result = wasPressed && !isPressed;
    wasPressed = isPressed;
    return result;
}

