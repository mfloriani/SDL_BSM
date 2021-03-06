#include "Tile.h"


Tile::Tile(SDL_Rect *box, int type, int id, Texture *texture, SDL_Rect *clip, bool isCollidable) : m_box(box), m_type(type), m_id(id), m_texture(texture), m_clip(clip), m_isCollidable(isCollidable)
{
}


Tile::~Tile()
{
	m_texture->Free();
	delete m_box;
}

void Tile::Draw()
{
	m_texture->Render(m_box->x, m_box->y, m_clip);
}

SDL_Rect* Tile::GetBox()
{
	return m_box;
}

int Tile::GetType()
{
	return m_type;
}

int Tile::GetId()
{
	return m_id;
}

bool Tile::isCollidable()
{
	return m_isCollidable;
}
