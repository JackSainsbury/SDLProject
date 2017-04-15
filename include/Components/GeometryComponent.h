#ifndef GEOMETRYCOMPONENT_H
#define GEOMETRYCOMPONENT_H
#include "Component.h"
#include <memory>
#include <ngl/Obj.h>

class GeometryComponent : public Component
{
public:
    GeometryComponent();//constructor passes a component type back to Component by default
    void update() override;//override virtual update function

    ~GeometryComponent(){}//virtual destructor (though I believe it would be virtual anyway)

    void loadMesh(std::string _fName, std::string _texName);

    std::unique_ptr<ngl::Obj> m_mesh;

};

#endif // GEOMETRY_COMPONENT_H
