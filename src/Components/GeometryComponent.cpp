#include "GeometryComponent.h"
#include <ngl/VAOPrimitives.h>

//set the type by default
GeometryComponent::GeometryComponent() : Component(ComponentType::Geometry){
}
//out for testing
void GeometryComponent::update(){
    std::cout<<"update geometry - runtime geo switch \n";
}

void GeometryComponent::loadMesh(std::string _fName, std::string _texName){
    m_mesh.reset(new ngl::Obj(_fName,_texName));
    m_mesh->createVAO();
}
