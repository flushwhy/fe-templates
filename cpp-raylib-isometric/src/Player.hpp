#pragma once
#include "raylib.h"
#include "World.hpp"
class Player {
public:
 explicit Player(Vector2 p):gridPosition_(p),targetGrid_(p),renderGrid_(p){}
 void Update(float dt,const World& world);void Draw(const World& world) const;
 Vector2 GridPosition()const{return gridPosition_;} Vector2 RenderGridPosition()const{return renderGrid_;}
private:
 Vector2 gridPosition_,targetGrid_,renderGrid_;bool moving_=false;float moveSpeed_=6.0f;
 void BeginMove(Vector2 direction,const World& world);
};