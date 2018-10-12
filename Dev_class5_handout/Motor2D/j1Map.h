#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Collision.h"

// ----------------------------------------------------
struct Properties
{
	SDL_Rect	coll = { 0,0,0,0 };
	fPoint		position = { 0.0F,0.0F };
	fPoint		acceleration = { 0.0F,0.0F };
	fPoint		maxVelocity = { 0.0F,0.0F };
	float		jumpAcceleration = 0.0F;
	float		jumpMaxVelocity = 0.0F;
};

struct CollObject
{
	int				id = 0;
	float			x = 0.0f;
	float			y = 0.0f;
	float			width = 0.0f;
	float			height = 0.0f;
	COLLIDER_TYPE	coll_type = COLLIDER_NONE;
};

struct CollisionLayer
{
	p2SString			name;
	p2List<CollObject*> coll_object;
	~CollisionLayer()
	{
		coll_object.clear();
	}
};

struct MapLayer 
{
	p2SString	name;
	uint		width = 0u;
	uint		height = 0u;
	float		parallax_speed = 0.0F;
	uint*		data = nullptr;
	~MapLayer()
	{
		if (data != nullptr) 
		{
			delete[] data;
		}
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};


// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture = nullptr;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int						width;
	int						height;
	int						tile_width;
	int						tile_height;
	SDL_Color				background_color;
	MapTypes				type;
	p2List<TileSet*>		tilesets;
	p2List<MapLayer*>		layers;
	p2List<CollisionLayer*>	collision_layer;
	p2List<Collider*>		collider_list;
	Properties				properties_map;
	
	// TODO 2: Add a list/array of layers to the map!
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// Check Collision
	void OnCollision(Collider* c1, Collider* c2);

	iPoint MapToWorld(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadCollisionLayer(pugi::xml_node& node, CollisionLayer* coll_layer);
	bool LoadObject(pugi::xml_node& node, CollObject* coll_obj);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__