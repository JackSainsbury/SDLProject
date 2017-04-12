#include "Entity.h"
#include "RigidBodyComponent.h"
#include "GeometryComponent.h"
#include <algorithm>

Entity::Entity(){
    m_name = "new_Game_Entity";
}

Entity::Entity(std::string _name){
    m_name = _name;
}

//used for adding a "new" components which is created via the default constructors (this seems very messy - consider revision)
void Entity::createComponent(ComponentType::EnumType _type){
    if(searchComponent(_type) == nullptr){ //if the component check returns null, the component doesn't exist (future proof multiple same type components distinguishing)
        switch(_type){
        case 0x00000001:
            std::cout<<"adding new RigidBodyComponent";
            components.push_back(std::unique_ptr<Component>(new RigidBodyComponent()));//std::vector<std::shared_ptr<Component> components, pushed Component new RBC?
            break;
        case 0x00000002:
            std::cout<<"adding new GeometryComponent";
            components.push_back(std::unique_ptr<Component>(new GeometryComponent()));
            break;
        }
    }else
        std::cout<< m_name + " already has a component of type " + typeid(_type).name();//component already exists so return this info (need to revise - this returns CompType)
}

//pass a type and check if component exists - return pointer so null can be tested or Component
Component *Entity::searchComponent(ComponentType::EnumType _type){
    for(auto& c : components){ //search vector<sharedptr<Comonent>> components
        if(c->Type == _type){
            return &*c; //this is the component
        }
    }
    return nullptr;//component doesn't exist return null
}



/*
//pass exisingComponent add, messy messy messy. delete ptr?
void Entity::removeComponent(Component *referenceComponent){
    if(referenceComponent->Type == ComponentType::Transform){ //similar default transform check
        std::cout<< m_name + "Cannot remove Transform component; default component of all Entities";
    }else{
        std::vector<std::unique_ptr<Component>>::iterator _it = std::find(components.begin(), components.end(), std::unique_ptr<Component>(referenceComponent));

        if (_it != components.end())// == myVector.end() means the element was not found
        {
            //components.erase(_it);
            delete referenceComponent;
        }
    }
}



//pass exisingComponent add, messy messy messy. delete ptr?
void Entity::removeComponent(ComponentType _type){
    if(_type == ComponentType::Transform){ //similar default transform check
        std::cout<< m_name + "Cannot remove Transform component; default component of all Entities";
    }else{
        std::vector<std::unique_ptr<Component>>::iterator _it = std::find(components.begin(), components.end(), std::unique_ptr<Component>(referenceComponent));

        if (_it != components.end())// == myVector.end() means the element was not found
        {
            //components.erase(_it);
            delete referenceComponent;
        }
    }
}

*/
