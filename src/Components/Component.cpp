#include "Component.h"
//Basic contructor, gets passed a type, sets the active by default
Component::Component(ComponentType::EnumType _type){
    Type = _type;
    isActive = true;
    multipleInstancesAllowed = false;
}
