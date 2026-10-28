#include "Player.hpp"
#include "raymath.h"
void Player::BeginMove(Vector2 d,const World& w){int x=static_cast<int>(gridPosition_.x+d.x),y=static_cast<int>(gridPosition_.y+d.y);if(w.IsWalkable(x,y)){targetGrid_={static_cast<float>(x),static_cast<float>(y)};moving_=true;}}
void Player::Update(float dt,const World& w){
 if(moving_){Vector2 d=Vector2Subtract(targetGrid_,renderGrid_);float distance=Vector2Length(d),step=moveSpeed_*dt;if(distance<=step){renderGrid_=targetGrid_;gridPosition_=targetGrid_;moving_=false;}else renderGrid_=Vector2Add(renderGrid_,Vector2Scale(Vector2Normalize(d),step));return;}
 Vector2 d{0,0};if(IsKeyPressed(KEY_RIGHT)||IsKeyPressed(KEY_D))d.x=1;else if(IsKeyPressed(KEY_LEFT)||IsKeyPressed(KEY_A))d.x=-1;else if(IsKeyPressed(KEY_DOWN)||IsKeyPressed(KEY_S))d.y=1;else if(IsKeyPressed(KEY_UP)||IsKeyPressed(KEY_W))d.y=-1;
 if(d.x!=0||d.y!=0)BeginMove(d,w);
}
void Player::Draw(const World& w)const{Vector2 p=w.GridToWorld(renderGrid_);DrawCircleV({p.x,p.y-9},9,{232,118,73,255});DrawCircleLines(static_cast<int>(p.x),static_cast<int>(p.y-9),9,MAROON);}
