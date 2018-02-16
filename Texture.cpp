#include "Texture.h"
#include <SDL_image.h>


Texture::Texture()
{
	m_renderer = NULL;
	m_texture = NULL;
	m_width = 0;
	m_height = 0;
}

Texture::~Texture()
{
	Free();
}

bool Texture::LoadFromFile(SDL_Renderer* renderer, std::string path)
{
	Free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x00, 0xFF));
		m_renderer = renderer;
		newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
		if (newTexture == NULL)
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		else
		{
			m_width = loadedSurface->w;
			m_height = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	m_texture = newTexture;
	return m_texture != NULL;
}

bool Texture::LoadFromRenderedText(SDL_Renderer* renderer, std::string textureText, SDL_Color textColor, TTF_Font* font)
{
	Free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		m_renderer = renderer;
		m_texture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
		if (m_texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			m_width = textSurface->w;
			m_height = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}

	return m_texture != NULL;
}


void Texture::Free()
{
	if (m_texture != NULL)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
		m_renderer = NULL;
		m_width = 0;
		m_height = 0;
	}
}

void Texture::Render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderRect = { x, y, m_width, m_height };
	if (clip != NULL)
	{
		renderRect.w = clip->w;
		renderRect.h = clip->h;
	}
	SDL_RenderCopyEx(m_renderer, m_texture, clip, &renderRect, angle, center, flip);
}

int Texture::GetHeight()
{
	return m_height;
}

int Texture::GetWidth()
{
	return m_width;
}
