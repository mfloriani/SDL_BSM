#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Texture
{
private:
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;
	int m_width;
	int m_height;

public:
	Texture();
	~Texture();
	bool LoadFromFile(SDL_Renderer* renderer, std::string path);
	bool LoadFromRenderedText(SDL_Renderer* renderer, std::string textureText, SDL_Color textColor, TTF_Font* font);
	void Free();
	void Render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	int GetWidth();
	int GetHeight();
};

#endif
