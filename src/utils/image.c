#include "image.h"

#include <SDL2/SDL_image.h>
#include <stdio.h>

int image_init(void)
{
	int type = IMG_INIT_PNG | IMG_INIT_JPG;
	int ret = IMG_Init(type); 
	if ((type & ret) != type)
	{
		fprintf(stderr, "image_init: %s\n", IMG_GetError());
		return -1;
	}
	else
	{
		return 0;
	}
}

SDL_Surface *image_load(const char *path)
{
	SDL_Surface *igm = IMG_Load(path);
	if (igm == NULL)
	{
		fprintf(stderr, "image_load: %s: %s\n", path, IMG_GetError());
		return NULL;
	}
	
	SDL_Surface *conv = SDL_ConvertSurfaceFormat(igm,
						SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(igm);
	if (conv == NULL)
	{
		fprintf(stderr, "image_load: %s: %s\n", path, SDL_GetError());
	}
	return conv;
}

int image_save(SDL_Surface *surface, const char *path)
{
	if (IMG_SavePNG(surface, path) < 0)
	{
		fprintf(stderr, "image_save: %s: %s\n", path, IMG_GetError());
		return -1;
	}
	else 
	{
		return 0;
	}
}

void image_quit(void)
{
	IMG_Quit();
}
