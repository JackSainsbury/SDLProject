#ifndef INPUT_H__
#define INPUT_H__

#include <iostream>
#include <SDL.h>

#include "unordered_map"

struct EnumClass_HashCast
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};


namespace MouseCode{
    enum MouseCodeEnum
    {
        left_Mouse_Button,
        middle_Mouse_Button,
        right_Mouse_Button,
        move_x_Mouse_Motion,
        move_y_Mouse_Motion
    };
}

namespace KeyCode{
    enum KeyCodeEnum
    {
        w_Key = SDLK_w,
        a_Key = SDLK_a,
        s_Key = SDLK_s,
        d_Key = SDLK_d
    };
}

//Hash the Enum for unordered map with Enum cast - (Considered language defect in C++14 onwards and defaulted)
template <typename Key>
using Hash_Type = typename std::conditional<std::is_enum<Key>::value, EnumClass_HashCast, std::hash<Key>>::type;
//NewStructured unordered "enum" map
template <typename Key, typename T>
using HashEnumUnorderedMap = std::unordered_map<Key, T, Hash_Type<Key>>;


class Input
{
public:
    Input();

    //Setters (SDL FUNCTION USE) (public)
    bool SetKeyDown( KeyCode::KeyCodeEnum _refKey, bool _value );                   //Set the tick a key is pressed
    bool SetKeyUp( KeyCode::KeyCodeEnum _refKey, bool _value );                     //Set the tick a key is released

    bool SetMouseButtonDown( MouseCode::MouseCodeEnum _refButton, bool _value );    //Set the tick a "mouse button" is pressed/activated (includes, mouse motion)
    bool SetMouseButtonUp( MouseCode::MouseCodeEnum _refButton, bool _value);       //Set the tick a "mouse button" is pressed/activated (includes, mouse motion)

    //Getters (General Use)
    bool GetKey( KeyCode::KeyCodeEnum _refKey );                                    //Get all ticks a key is held down
    bool GetKeyDown( KeyCode::KeyCodeEnum _refKey );                                //Get the tick a key is pressed
    bool GetKeyUp( KeyCode::KeyCodeEnum _refKey );                                  //Get the tick a key is released

    bool GetMouseButton( MouseCode::MouseCodeEnum _refButton );                     //Get all ticks a "mouse button" is pressed/activated (includes, mouse motion)
    bool GetMouseButtonDown( MouseCode::MouseCodeEnum _refButton );                 //Get the tick a mouse button is pressed/stops moving
    bool GetMouseButtonUp( MouseCode::MouseCodeEnum _refButton );                   //Get the tick a mouse button is released/stops moving

private:
    void LoadInputListeners();

    //Setters (SDL FUNCTION USE) (private)
    bool SetKey( KeyCode::KeyCodeEnum _refKey, bool _value );                       //Set a key to held down
    bool SetMouseButton( MouseCode::MouseCodeEnum _refButton, bool _value );        //Set a mouse button to held down or moving


    //Map structures Key and Mouse maps should be initialized as my Enums
    //(improves readability and user control, also stops listening for ALL keys ALWAYS doing lookup
    HashEnumUnorderedMap<KeyCode::KeyCodeEnum, bool> m_GetKeyMap;        //Keys being held
    HashEnumUnorderedMap<KeyCode::KeyCodeEnum, bool> m_GetKeyDownMap;    //Keys on the tick pressed down
    HashEnumUnorderedMap<KeyCode::KeyCodeEnum, bool> m_GetKeyUpMap;      //Keys on the tick released

    HashEnumUnorderedMap<MouseCode::MouseCodeEnum, bool> m_GetMouseButtonMap;        //Mouse buttons being held
    HashEnumUnorderedMap<MouseCode::MouseCodeEnum, bool> m_GetMouseButtonDownMap;    //Mouse buttons on the tick pressed down
    HashEnumUnorderedMap<MouseCode::MouseCodeEnum, bool> m_GetMouseButtonUpMap;      //Mouse buttons on the tick released
};

#endif
