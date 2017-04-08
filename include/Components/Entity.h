#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <memory>
#include <vector>
#include "Component.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "GeometryComponent.h"
#include <cstring>


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

    //all entities are going to have a transform - world game objects by default (doesn't exist in the component vector but the get and set component are designed to handle this.)
    Component* Transform;

    template <class m>
    m *dostuff();

    template <class S>
    S *getComponent()
    {
        if(typeid(S) == typeid(TransformComponent)){
            return dynamic_cast<S*>(Transform);
        }else  if(typeid(S) == typeid(RigidBodyComponent)){
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
        if(typeid(S) == typeid(TransformComponent)){
            std::cerr<<("TransformComponent exists by default.");
        }else if(typeid(S) == typeid(RigidBodyComponent)){
            createComponent(ComponentType::RigidBody);
        }else if(typeid(S) == typeid(GeometryComponent)){
            createComponent(ComponentType::Geometry);
        }
    }

    //update the entity's components
    void update(){Transform->update(); for(auto& c: components) c->update();}

    //private:
    //vector of all this entities components (usually private but for NGLScene::InitializeGL std::cout<<MyEntity.components.size(); made public
    std::vector<std::unique_ptr<Component>> components;

private:

    //search for the component of "type" return a pointer to it and return a null ptr if it couldn't be found. Returning a pointer? maybe not being deleted
    Component *searchComponent(ComponentType::EnumType _type);

    //create a new component of component type on the vector
    void createComponent(ComponentType::EnumType _type);


protected:
    std::string m_name;//non unique name of the entity instance
};

#endif // ENTITY_H
