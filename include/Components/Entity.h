#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <memory>
#include <vector>
#include "Component.h"
#include "RigidBodyComponent.h"
#include "GeometryComponent.h"
#include <cstring>
#include <ngl/Transformation.h>

template <class T>
struct Holder
{
    T t;
};

class Entity
{
public:
    Entity();
    Entity(std::string _name);


    template <class S>
    S *getComponent()
    {
        if(typeid(S) == typeid(RigidBodyComponent)){
            return dynamic_cast<S*>(searchComponent(ComponentType::RigidBody));
        }else if(typeid(S) == typeid(GeometryComponent)){
            return dynamic_cast<S*>(searchComponent(ComponentType::Geometry));
        }else{
            return nullptr;
        }
    }

    template <class S>
    inline void addComponent()
    {
        if(typeid(S) == typeid(RigidBodyComponent)){
            createComponent(ComponentType::RigidBody);
        }else if(typeid(S) == typeid(GeometryComponent)){
            createComponent(ComponentType::Geometry);
        }
    }

    //update the entity's components
    void update(){for(auto& c: components) c->update();}

    inline ngl::Transformation* GetTransform(){return &m_transform;}

    //private:
    //vector of all this entities components (usually private but for NGLScene::InitializeGL std::cout<<MyEntity.components.size(); made public
    std::vector<std::unique_ptr<Component>> components;

    std::string m_name;//non unique name of the entity instance
private:

    //search for the component of "type" return a pointer to it and return a null ptr if it couldn't be found. Returning a pointer? maybe not being deleted
    Component *searchComponent(ComponentType::EnumType _type);

    //create a new component of component type on the vector
    void createComponent(ComponentType::EnumType _type);

    ngl::Transformation m_transform;
};

#endif // ENTITY_H
