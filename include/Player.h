#ifndef PLAYER_H
#define PLAYER_H

#include <ngl/VAOPrimitives.h>
#include "MapHandler.h"
#include "Entity.h"
#include <ngl/Transformation.h>

class Player : public Entity
{
public:
    Player();

    void initPlayer();

    inline void SetMainMap(MapHandler* _mainMap){ main_map = _mainMap; }

    void ModInputVector(ngl::Vec2 _value);

    inline ngl::Vec2 GetInputVector(){ return m_InputVector; }
    inline float GetPlayerWalkSpeed(){ return playerWalkSpeed; }

private:

    MapHandler *main_map;
    ngl::Vec2 m_InputVector;
    float playerWalkSpeed;
};

#endif // PLAYER_H
