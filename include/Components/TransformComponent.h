#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "Component.h"
#include <ngl/Vec3.h>
#include <ngl/Transformation.h>

class TransformComponent : public Component
{
public:
    TransformComponent();
    void update() override;

    ~TransformComponent() = default;

    //Transform Get/Sets
    inline void setPosition(ngl::Vec3 _pos) { m_position = _pos; }
    inline ngl::Vec3 getPosition() { return m_position; }

    inline void setRotation(ngl::Vec3 _rot) { m_rotation = _rot; }
    inline ngl::Vec3 getRotation() { return m_rotation; }

    inline void setScale(ngl::Vec3 _sca) { m_scale = _sca; }
    inline ngl::Vec3 getScale() { return m_scale; }
    //Transform Modifiers

    inline ngl::Transformation getTransformation() { return m_transformation; }

    void Translate(ngl::Vec3 _Dt_pos);
    void Rotate(ngl::Vec3 _Dt_rot);
    void Scale(ngl::Vec3 _Dt_sca);
private:
    ngl::Vec3 m_position;
    ngl::Vec3 m_rotation;
    ngl::Vec3 m_scale;

    ngl::Transformation m_transformation;
};

#endif // TRANSFORMCOMPONENT_H
