#include "World.hpp"
#include "raymath.h"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
using nlohmann::json;

bool World::Load(const std::string& path) {
 std::ifstream file(path); if(!file){std::cerr<<"Could not open map: "<<path<<'\n';return false;}
 try {
  const json map=json::parse(file); width_=map.at("width").get<int>(); height_=map.at("height").get<int>();
  tileWidth_=map.value("tilewidth",64); tileHeight_=map.value("tileheight",32);
  if(map.value("orientation",std::string("isometric"))!="isometric"){std::cerr<<"Map must use Tiled's isometric orientation.\n";return false;}
  const auto& layers=map.at("layers");
  const auto layer=std::find_if(layers.begin(),layers.end(),[](const json& item){return item.value("type",std::string())=="tilelayer";});
  if(layer==layers.end()){std::cerr<<"Map has no tile layer.\n";return false;}
  const auto data=layer->at("data").get<std::vector<int>>();
  if(width_<=0||height_<=0||data.size()!=static_cast<std::size_t>(width_*height_)){std::cerr<<"Invalid tile layer dimensions.\n";return false;}
  tiles_.clear(); tiles_.reserve(data.size());
  for(int gid:data){switch(gid){case 0:tiles_.push_back(TileType::Empty);break;case 2:tiles_.push_back(TileType::Path);break;case 3:tiles_.push_back(TileType::Water);break;case 4:tiles_.push_back(TileType::Wall);break;default:tiles_.push_back(TileType::Grass);break;}}
  Image image=LoadImageSvg("assets/tileset.svg",256,32);
  if(image.data){tileset_=LoadTextureFromImage(image);UnloadImage(image);hasTileset_=IsTextureValid(tileset_);}
  return true;
 } catch(const std::exception& e){std::cerr<<"Could not read map "<<path<<": "<<e.what()<<'\n';return false;}
}
void World::Unload(){if(hasTileset_)UnloadTexture(tileset_);hasTileset_=false;}
TileType World::TileAt(int x,int y) const {if(x<0||y<0||x>=width_||y>=height_)return TileType::Wall;return tiles_[static_cast<std::size_t>(y*width_+x)];}
bool World::IsWalkable(int x,int y) const {auto t=TileAt(x,y);return t==TileType::Grass||t==TileType::Path;}
Vector2 World::GridToWorld(Vector2 g) const{return {(g.x-g.y)*tileWidth_*0.5f,(g.x+g.y)*tileHeight_*0.5f};}
Vector2 World::WorldToGrid(Vector2 p) const {float hw=tileWidth_*0.5f,hh=tileHeight_*0.5f;return {p.x/(2*hw)+p.y/(2*hh),p.y/(2*hh)-p.x/(2*hw)};}
std::vector<Vector2> World::FindPath(Vector2 start,Vector2 goal) const {
 auto key=[this](int x,int y){return y*width_+x;};
 int sx=static_cast<int>(start.x),sy=static_cast<int>(start.y),gx=static_cast<int>(goal.x),gy=static_cast<int>(goal.y);
 if(!IsWalkable(sx,sy)||!IsWalkable(gx,gy))return {};
 struct Node{int x,y,cost,score;}; struct Later{bool operator()(const Node&a,const Node&b)const{return a.score>b.score;}};
 std::priority_queue<Node,std::vector<Node>,Later> open;std::vector<int> prev(static_cast<std::size_t>(width_*height_),-1),cost(static_cast<std::size_t>(width_*height_),1000000);
 auto heuristic=[gx,gy](int x,int y){return std::abs(gx-x)+std::abs(gy-y);};
 cost[static_cast<std::size_t>(key(sx,sy))]=0;open.push({sx,sy,0,heuristic(sx,sy)});
 const int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
 while(!open.empty()){auto n=open.top();open.pop();int nk=key(n.x,n.y);if(n.cost!=cost[static_cast<std::size_t>(nk)])continue;if(n.x==gx&&n.y==gy)break;
  for(int i=0;i<4;++i){int x=n.x+dx[i],y=n.y+dy[i];if(!IsWalkable(x,y))continue;int k=key(x,y),c=n.cost+1;if(c>=cost[static_cast<std::size_t>(k)])continue;cost[static_cast<std::size_t>(k)]=c;prev[static_cast<std::size_t>(k)]=nk;open.push({x,y,c,c+heuristic(x,y)});}
 }
 int cursor=key(gx,gy),source=key(sx,sy);if(cursor!=source&&prev[static_cast<std::size_t>(cursor)]==-1)return {};
 std::vector<Vector2> path;while(cursor!=source){path.push_back({static_cast<float>(cursor%width_),static_cast<float>(cursor/width_)});cursor=prev[static_cast<std::size_t>(cursor)];}
 std::reverse(path.begin(),path.end());return path;
}
void World::DrawTile(int x,int y,TileType type) const {
 if(type==TileType::Empty)return;Vector2 c=GridToWorld({static_cast<float>(x),static_cast<float>(y)});
 if(hasTileset_){int i=0;switch(type){case TileType::Grass:i=0;break;case TileType::Path:i=1;break;case TileType::Water:i=2;break;case TileType::Wall:i=3;break;case TileType::Empty:return;}
  DrawTexturePro(tileset_,{static_cast<float>(i*tileWidth_),0,static_cast<float>(tileWidth_),static_cast<float>(tileHeight_)},{c.x-tileWidth_*0.5f,c.y-tileHeight_*0.5f,static_cast<float>(tileWidth_),static_cast<float>(tileHeight_)},{0,0},0,WHITE);return;}
 Color color=GREEN;switch(type){case TileType::Grass:color={82,142,91,255};break;case TileType::Path:color={190,157,105,255};break;case TileType::Water:color={65,133,178,255};break;case TileType::Wall:color={104,108,112,255};break;case TileType::Empty:return;}
 Vector2 top{c.x,c.y-tileHeight_*0.5f},right{c.x+tileWidth_*0.5f,c.y},bottom{c.x,c.y+tileHeight_*0.5f},left{c.x-tileWidth_*0.5f,c.y};
 DrawTriangle(top,left,bottom,color);DrawTriangle(top,bottom,right,color);DrawLineV(top,right,Fade(BLACK,0.25f));DrawLineV(right,bottom,Fade(BLACK,0.25f));DrawLineV(bottom,left,Fade(BLACK,0.25f));DrawLineV(left,top,Fade(BLACK,0.25f));
}
void World::Draw() const {for(int sum=0;sum<width_+height_-1;++sum)for(int y=0;y<height_;++y){int x=sum-y;if(x>=0&&x<width_)DrawTile(x,y,TileAt(x,y));}}
