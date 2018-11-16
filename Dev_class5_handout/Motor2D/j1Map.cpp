#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Map.h"
#include <math.h>

//#include "Brofiler/Brofiler.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	/*BROFILER_CATEGORY("Map Draw", Profiler::Color::SlateGray);*/
	if (map_loaded == false)
		return;

	p2List_item<MapLayer*>* item = data.layers.start;

	for (; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("draw") != 1)
			continue;

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = MapToWorld(x, y);

					App->render->Blit(tileset->texture, pos.x, pos.y, &r, SDL_FLIP_NONE, layer->properties.Get("parallax_speed"));
				}
			}
		}
	}
}

float Properties::Get(const char* value, float default_value) const
{
	p2List_item<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2) - 1;
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item_tileset = data.tilesets.end;
	while (item_tileset->data->firstgid > id)
	{
		item_tileset = item_tileset->prev;
	}
	return item_tileset->data;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();


	// Remove all layers
	p2List_item<MapLayer*>* items;
	items = data.layers.start;

	while (items != NULL)
	{
		RELEASE(items->data);
		items = items->next;
	}
	data.layers.clear();

	// Remove all collision layers
	p2List_item<CollisionLayer*>* item_coll_layers;
	item_coll_layers = data.collision_layer.start;

	while (item_coll_layers != NULL)
	{
		// Remove all coll objects
		p2List_item<CollObject*>* item_coll_objects;
		item_coll_objects = item_coll_layers->data->coll_object.start;

		while (item_coll_objects != NULL)
		{
			RELEASE(item_coll_objects->data);
			item_coll_objects = item_coll_objects->next;
		}

		item_coll_layers->data->coll_object.clear();
		RELEASE(item_coll_layers->data);
		item_coll_layers = item_coll_layers->next;
	}
	data.collision_layer.clear();

	//Remove al Colliders
	p2List_item<Collider*>* item_collider;
	item_collider = data.collider_list.end;
	while (item_collider != NULL)
	{
		if (item_collider->data != nullptr) 
		{
			item_collider->data->to_delete = true;
		}
		item_collider = item_collider->prev;
	}
	data.collider_list.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------

	for (pugi::xml_node layers = map_file.child("map").child("layer"); layers; layers = layers.next_sibling("layer"))
	{
		MapLayer* layer_map = new MapLayer();
		if (ret == true)
		{
			LoadLayer(layers, layer_map);
		}

		data.layers.add(layer_map);
	}

	// Load collision layer info ----------------------------------------------

	for (pugi::xml_node collision_layer = map_file.child("map").child("objectgroup"); collision_layer; collision_layer = collision_layer.next_sibling("objectgroup"))
	{
		CollisionLayer* collision = new CollisionLayer();
		if (ret == true)
		{
			LoadCollisionLayer(collision_layer, collision);
		}
		data.collision_layer.add(collision);
	}

	// Load Player Properties
	LoadProperties(map_file.child("map"), data.player_properties);

	//Create Colliders ----------------------------------------------------------
	p2List_item<CollisionLayer*>* item_collision_layer = data.collision_layer.start;
	while (item_collision_layer!=NULL)
	{
		p2List_item<CollObject*>* item_coll_object = item_collision_layer->data->coll_object.start;
		while (item_coll_object != NULL) 
		{
			SDL_Rect r = { (int)item_coll_object->data->x, (int)item_coll_object->data->y, (int)item_coll_object->data->width, (int)item_coll_object->data->height };
			Collider* coll = new Collider();
			coll = App->collision->AddCollider({ r.x, r.y, r.w, r.h }, item_coll_object->data->coll_type, App->map);
			data.collider_list.add(coll);
			item_coll_object = item_coll_object->next;
		}
		item_collision_layer = item_collision_layer->next;
	}

	//Show info
	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}

		p2List_item<CollisionLayer*>* item_col_layer = data.collision_layer.start;
		while (item_col_layer != NULL)
		{
			CollisionLayer* c = item_col_layer->data;
			LOG("Collision Layer -----");
			LOG("name: %s", c->name.GetString());
			p2List_item<CollObject*>* item_coll_object = c->coll_object.start;
			while (item_coll_object != NULL) 
			{
				CollObject* collobject = item_coll_object->data;
				LOG("x: %f y: %f width: %f height: %f", collobject->x, collobject->y, collobject->width, collobject->height);
				item_coll_object=item_coll_object->next;
			}
			item_col_layer = item_col_layer->next;
		}

		map_loaded = ret;
	}

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = (set->tex_width - 2*set->margin) / (set->spacing +set->tile_width);
		set->num_tiles_height = (set->tex_height - 2 * set->margin) / (set->spacing + set->tile_height);
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	LoadProperties(node, layer->properties);
	layer->data = new uint[layer->width*layer->height];
	memset(layer->data, 0, sizeof(uint)*layer->width*layer->height);

	uint i = 0;
	for (pugi::xml_node tiles = node.child("data").child("tile"); tiles; tiles = tiles.next_sibling("tile")) {
		layer->data[i] = tiles.attribute("gid").as_uint();
		i++;
	}

	return true;
}

bool j1Map::LoadCollisionLayer(pugi::xml_node& node, CollisionLayer* coll_layer)
{
	coll_layer->name = node.attribute("name").as_string();

	for (pugi::xml_node object_node = node.child("object"); object_node; object_node = object_node.next_sibling("object")) {
		CollObject* collobject = new CollObject();
		LoadObject(object_node, collobject);
		coll_layer->coll_object.add(collobject);
	}

	return true;
}

bool j1Map::LoadObject(pugi::xml_node& node, CollObject* coll_object)
{
	coll_object->id = node.attribute("id").as_int();
	coll_object->x = node.attribute("x").as_float();
	coll_object->y = node.attribute("y").as_float();
	coll_object->width = node.attribute("width").as_float();
	coll_object->height = node.attribute("height").as_float();
	p2SString coll_name(node.attribute("type").as_string());
	if (coll_name == "GROUND_COLLIDER") 
	{
		coll_object->coll_type = COLLIDER_GROUND;
	}
	else if (coll_name == "PLATFORM_COLLIDER")
	{
		coll_object->coll_type = COLLIDER_PLATFORM;
	}
	else if (coll_name == "WALL_COLLIDER")
	{
		coll_object->coll_type = COLLIDER_WALL;
	}
	else if (coll_name == "DEATH_COLLIDER")
	{
		coll_object->coll_type = COLLIDER_DEATH;
	}
	else if (coll_name == "WIN_COLLIDER")
	{
		coll_object->coll_type = COLLIDER_WIN;
	}
	return true;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;
	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_float();

			properties.list.add(p);
		}
	}
	return ret;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
						map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}