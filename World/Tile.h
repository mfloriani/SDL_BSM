#pragma once
#include "SDL.h"
#include "../Utils/Texture.h"

class Tile
{
private:
	int			m_type;
	int			m_id;
	bool		m_isCollidable;
	SDL_Rect*	m_box;
	Texture*	m_texture;
	SDL_Rect*	m_clip;
	
public:
	Tile(SDL_Rect *box, int type, int id, Texture *texture, SDL_Rect *clip, bool isCollidable);
	virtual ~Tile();
	void Draw();
	SDL_Rect* GetBox();
	int GetType();
	int GetId();
	bool isCollidable();

};

