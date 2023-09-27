#pragma once
#include "GamePad.h"
#include "Input.h"

class InputGame {
public:
    GamePad& gamePad = Input::Instance().GetGamePad();
    //�����͏���
    static bool InputRoll()
    {
        if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_TRIGGER)
        {
            return true;
        }
        return false;
    }
    //����A�^�b�N���͏���
    static bool InputSpecial()
    {
        if (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_LEFT_SHOULDER)
        {
            return true;
        }
        return false;
    }
    //�V�t�g���͏���
    static bool InputShift()
    {
        if (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_LEFT_TRIGGER)
        {
            return true;
        }
        return false;
    }



};