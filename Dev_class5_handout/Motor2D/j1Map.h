#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Collision.h"

// ----------------------------------------------------

struct CollObject
{
	int         id = 0;
	float		x = 0.0f;
	float		y = 0.0f;
	float		width = 0.0f;
	float		height = 0.0f;
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

	//Check Collision with Map
	void OnCollision(Collider* c1, Collider* c2);

	iPoint MapToWorld(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	// TODO 3: Create a method that loads a single laye
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadCollisionLayer(pugi::xml_node& node, CollisionLayer* coll_layer);
	bool LoadObject(pugi::xml_node& node, CollObject* coll_obj);
	bool CreateCollider(SDL_Rect rect, Collider* coll, COLLIDER_TYPE coll_type);

public:

	MapData data;

	inline uint Get(int x, int y) const {
		return y * (data.width) + x;
	}

	p2List<Collider*> collider_list;

private:

	pugi::xml_document	map_file;
	
	p2SString			folder;
	bool				map_loaded;
	int					coll_layer = 0;
};

#endif // __j1MAP_H__