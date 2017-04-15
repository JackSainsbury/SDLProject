#include "Block.h"

Block::Block(std::string _BlockName){
    m_blockName = _BlockName;
}


void Block::OnDestroy(){
    std::cout<<",mined";

    //if no objects to drop then this will skip
    for(Entity &_obj : m_dropObjects)//by referent to prevent copies
    {
        std::cout<<"dropping : "<< _obj.m_name<<std::endl;
        //_obj->addComponent<RigidBodyComponent>();
    }
}

void Block::OnPlace(){
    std::cout<<"placed"<<std::endl;
}
