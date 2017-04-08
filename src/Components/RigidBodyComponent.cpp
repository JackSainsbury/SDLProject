#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent() : Component(ComponentType::RigidBody){
    velocity = 15.0f;
}

void RigidBodyComponent::update(){
    std::cout << "\n";
    std::cout << velocity;
    std::cout << "\n";
}
