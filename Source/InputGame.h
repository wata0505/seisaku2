#pragma once
#include "GamePad.h"
#include "Input.h"

class InputGame {
public:
    GamePad& gamePad = Input::Instance().GetGamePad();
    //回避入力処理
    static bool InputRoll()
    {
        if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_TRIGGER)
        {
            return true;
        }
        return false;
    }
    //特殊アタック入力処理
    static bool InputSpecial()
    {
        if (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_LEFT_SHOULDER)
        {
            return true;
        }
        return false;
    }
    //シフト入力処理
    static bool InputShift()
    {
        if (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_LEFT_TRIGGER)
        {
            return true;
        }
        return false;
    }



};