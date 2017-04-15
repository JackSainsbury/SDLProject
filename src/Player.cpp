#include "Player.h"
#include "NGLDraw.h"

#include <ngl/ShaderLib.h>

Player::Player()
{
    playerWalkSpeed = 5.0f;
}
//init player function ran after m_map has been set, messy data encapsulation - must do m_map nullptr check here
void Player::initPlayer(){    
    playerObj.reset(new ngl::Obj("Assets/models/player.obj","Assets/textures/player.png"));
    playerObj->createVAO();
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

void Player::DrawPlayer(){
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();
    ngl::Transformation trans;
    if(main_map->MainDrawLoop->OnUpPress){
        trans.setRotation(0,180,0);
    }else if(main_map->MainDrawLoop->OnDownPress){
        trans.setRotation(0,0,0);
    }else if(main_map->MainDrawLoop->OnRightPress){
        trans.setRotation(0,90,0);
    }else if(main_map->MainDrawLoop->OnLeftPress){
        trans.setRotation(0,-90,0);
    }

    trans.addPosition(0,.5,0);
    main_map->MainDrawLoop->loadMatricesToShader(trans.getMatrix());
    playerObj->draw();
}

void Player::ManagePlayerInput(){

/*
    //disgusting function for now
    if(GetInputVector().m_x != 0){
        if(_m_player->GetInputVector().m_x > 0){
            if(GetChunk((ChunkVectorDimension-1)/2,
                        (ChunkVectorDimension-1)/2).GetBlock(int(m_transform.getPosition().m_x) +1,
                        int(m_transform.getPosition().m_y)) == 0 || m_transform.getPosition().m_x<ChunkDimension-.5){

                m_transform.addPosition(_m_player->GetInputVector().m_x * _m_player->GetPlayerWalkSpeed() * (MainDrawLoop->deltaTimeMiliseconds/1000.0f),0,0);
            }
        }else{

        }
    }

    if(_m_player->GetInputVector().m_y != 0){
        if(_m_player->GetInputVector().m_y > 0){

        }else{

        }
    }*/

    //,_m_player->GetInputVector().m_y * _m_player->GetPlayerWalkSpeed() * (MainDrawLoop->deltaTimeMiliseconds/1000.0f)


    //apply the inverse of the current player input vector via his movespeed to my position. (fram independent)
    main_map->GetTransform()->addPosition(GetInputVector().m_x * GetPlayerWalkSpeed() * (main_map->MainDrawLoop->deltaTimeMiliseconds/1000.0f)
                            ,0
                            ,GetInputVector().m_y * GetPlayerWalkSpeed() * (main_map->MainDrawLoop->deltaTimeMiliseconds/1000.0f));


    //CYLCING
    if(main_map->GetTransform()->getPosition().m_x>0.5){
        main_map->CyclePushChunks(MapHandlerCycleSpace::East);
        main_map->GetTransform()->setPosition(-(main_map->GetChunkDimension()-0.5),0,main_map->GetTransform()->getPosition().m_z);
    }else if(main_map->GetTransform()->getPosition().m_x < -(main_map->GetChunkDimension()-0.5)){
        main_map->CyclePushChunks(MapHandlerCycleSpace::West);
        main_map->GetTransform()->setPosition(0.5,0,main_map->GetTransform()->getPosition().m_z);
    }

    if(main_map->GetTransform()->getPosition().m_z>0.5){
        main_map->CyclePushChunks(MapHandlerCycleSpace::South);
        main_map->GetTransform()->setPosition(main_map->GetTransform()->getPosition().m_x,0,-(main_map->GetChunkDimension()-0.5));
    }else if(main_map->GetTransform()->getPosition().m_z < -(main_map->GetChunkDimension()-0.5)){
        main_map->CyclePushChunks(MapHandlerCycleSpace::North);
        main_map->GetTransform()->setPosition(main_map->GetTransform()->getPosition().m_x,0,0.5);
    }

}
