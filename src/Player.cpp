#include "Player.h"

Player::Player() : Entity ("PlayerEntity")
{
    playerWalkSpeed = 3.0f;
}
//init player function ran after m_map has been set, messy data encapsulation - must do m_map nullptr check here
void Player::initPlayer(){    
    addComponent<GeometryComponent>();
    getComponent<GeometryComponent>()->loadMesh("Assets/models/player.obj","Assets/textures/player.png");
}

void Player::ModInputVector(ngl::Vec2 _value){
    m_InputVector += _value;

    if(m_InputVector.m_x > 1)
        m_InputVector.m_x = 1;
    if(m_InputVector.m_x < -1)
        m_InputVector.m_x = -1;

    if(m_InputVector.m_y > 1)
        m_InputVector.m_y = 1;
    if(m_InputVector.m_y < -1)
        m_InputVector.m_y = -1;
}
