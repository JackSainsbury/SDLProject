#include "Input.h"

Input::Input(){
    //Get the prefs for input from file, ready to listen for keypresses and set respective states
    LoadInputListeners();
}

void Input::LoadInputListeners(){
    //for(KeyCode::KeyCodeEnum myType : KeyCode::KeyCodeEnum){
    //    m_GetKeyMap.insert(myType, false);
    //}
}

//===========================Setters (SDL FUNCTION USE)===========================
//<key>---------------------------------------------------------------------------
bool Input::SetKey( KeyCode::KeyCodeEnum _refKey, bool _value ){
//Private held set function, handled by down and up setters
    if(_value)
        SetKeyDown(_refKey, false);//Now being held, set Down to false as we want to isolate "Down" function for one tick
    else
        SetKeyUp(_refKey, false);//No longer being held, set Up to false as we want to isolate "Up" function for one tick
}

bool Input::SetKeyDown(KeyCode::KeyCodeEnum _refKey, bool _value ){
    SetKey(_refKey, true);//Now being held (even if just for a single Game tick)
}

bool Input::SetKeyUp( KeyCode::KeyCodeEnum _refKey, bool _value ){
    SetKey(_refKey, false);//No longer being held
}
//</key>--------------------------------------------------------------------------

//<mouse>-------------------------------------------------------------------------
bool Input::SetMouseButton( MouseCode::MouseCodeEnum _refButton, bool _value ){
//Private held set function, handled by down and up setters
    if(_value)
        SetMouseButtonDown(_refButton, false);//Now being held, set Down to false as we want to isolate "Down" function for one tick
    else
        SetMouseButtonUp(_refButton, false);//No longer being held, set Up to false as we want to isolate "Up" function for one tick
}

bool Input::SetMouseButtonDown( MouseCode::MouseCodeEnum _refButton, bool _value ){
    SetMouseButton(_refButton, true);//Now being held (even if just for a single Game tick)
}

bool Input::SetMouseButtonUp( MouseCode::MouseCodeEnum _refButton, bool _value ){
    SetMouseButton(_refButton, false);//No longer being held

}
//</mouse>------------------------------------------------------------------------

//============================Getters (General Use)===============================
//<key>---------------------------------------------------------------------------
bool Input::GetKey( KeyCode::KeyCodeEnum _refKey ){

}

bool Input::GetKeyDown( KeyCode::KeyCodeEnum _refKey ){

}

bool Input::GetKeyUp( KeyCode::KeyCodeEnum _refKey ){

}
//</key>--------------------------------------------------------------------------

//<mouse>-------------------------------------------------------------------------
bool Input::GetMouseButton( MouseCode::MouseCodeEnum _refButton ){

}

bool Input::GetMouseButtonDown( MouseCode::MouseCodeEnum _refButton ){

}

bool Input::GetMouseButtonUp( MouseCode::MouseCodeEnum _refButton ){

}
//</mouse>------------------------------------------------------------------------
