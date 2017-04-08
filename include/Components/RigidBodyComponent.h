#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H
#include "Component.h"

class RigidBodyComponent : public Component
{
public:
    RigidBodyComponent();
    void update() override;

    ~RigidBodyComponent() = default;

    inline void setVelocity(float _vel) { velocity = _vel; }
    inline float getVelocity() { return velocity; }

private:
    float velocity;
};

#endif // RIGIDBODYCOMPONENT_H
