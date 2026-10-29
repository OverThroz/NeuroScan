#include "segmentation.h"
#include "image.h"

#include <stdlib.h>

static int is_black(SDL_Surface *s, int x, int y)
{
	Uint8 *base = s->pixels;
	Uint8 *row = base + y * s->pitch;
	Uint32 px = ((Uint32 *)row)[x];

	Uint8 r;
	Uint8 g;
	Uint8 b;

	SDL_GetRGB(px, s->format, &r, &g, &b);

	if (r < 128)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

//if we have a word in the line(black)
static int row_has_black(SDL_Surface *bw, struct box area, int y)
{
	for (int x = area.x ; x < area.x + area.w ; x ++)
	{
		if (is_black(bw, x, y) == 1)
		{
			return 1;
		}
	}
	return 0 ;
}

//if we have a word in the colone(black)
static int col_has_black(SDL_Surface *bw, struct box area, int x)
{
        for (int y = area.y ; y < area.y + area.h ; y ++)
        {
                if (is_black(bw, x, y) == 1)
                {
                        return 1;
                }
        }
        return 0 ;
}

struct box *seg_split_rows(SDL_Surface *bw, struct box area, size_t *count)
{
	*count = 0;
	//Pass 1 : count the bands
	size_t n = 0;
	int in_band = 0;
	int black = 0;
	for (int y = area.y ; y < area.y + area.h ; y++)
	{
		black = row_has_black(bw, area, y);
		if (black == 1 && in_band == 0)
		{
			n++;
			in_band = 1;
		}
		else if (black == 0)
		{
			in_band = 0;
		}
	}
	if (n == 0)
	{
		return NULL;
	}
	//Pass 2 : fill the array with the bands' coordinates
	struct box *bands = malloc(n * sizeof(struct box));
	if(bands == NULL)
	{
		return NULL;
	}

	*count = n;

	size_t i = 0;
	int start = 0;
	in_band = 0;

	for(int y = area.y; y < area.y + area.h; y++)
	{
		black = row_has_black(bw, area, y);
		if (black == 1 && in_band == 0)
		{
			start = y;
			in_band = 1;
		}
		if (black == 0 && in_band == 1)
		{
			bands[i].x = area.x;
			bands[i].y = start;
			bands[i].w = area.w;
			bands[i].h = (y - 1) - start + 1;
			i++;
			in_band = 0;
		}
	}

	if (in_band == 1)
	{
		bands[i].x = area.x;
		bands[i].y = start;
		bands[i].w = area.w;
		bands[i].h = (area.y + area.h - 1) - start + 1;
	}

	return bands;
}

struct box *seg_split_cols(SDL_Surface *bw, struct box area, size_t *count)
{
        *count = 0;
        //Pass 1 : count the bands
        size_t n = 0;
        int in_band = 0;
        int black = 0;
        for (int x = area.x ; x < area.x + area.w ; x++)
        {
                black = col_has_black(bw, area, x);
                if (black == 1 && in_band == 0)
                {
                        n++;
                        in_band = 1;
                }
                else if (black == 0)
                {
                        in_band = 0;
                }
        }
        if (n == 0)
        {
                return NULL;
        }
        //Pass 2 : fill the array with the bands' coordinates
        struct box *bands = malloc(n * sizeof(struct box));
        if(bands == NULL)
        {
                return NULL;
        }

        *count = n;

        size_t i = 0;
        int start = 0;
        in_band = 0;

        for(int x = area.x; x < area.x + area.w; x++)
        {
                black = col_has_black(bw, area, x);
                if (black == 1 && in_band == 0)
                {
                        start = x;
                        in_band = 1;
                }
                if (black == 0 && in_band == 1)
                {
                        bands[i].x = start;
                        bands[i].y = area.y;
                        bands[i].w = (x - 1) - start + 1;
                        bands[i].h = area.h;
                        i++;
                        in_band = 0;
                }
        }

        if (in_band == 1)
        {
                bands[i].x = start;
                bands[i].y = area.y;
                bands[i].w = (area.x + area.w - 1) - start + 1;
                bands[i].h = area.h;
        }

        return bands;
}


int seg_save_box(SDL_Surface *src, struct box b, const char *path)
{
	if (b.w <= 0 || b.h <= 0 || b.x < 0 || b.y < 0 || b.x + b.w > src->w || b.y + b.h > src->h)
	{
		return -1;
	}
	SDL_Surface *cpy = SDL_CreateRGBSurfaceWithFormat(0,b.w,b.h,32,SDL_PIXELFORMAT_ARGB8888);
	if (cpy == NULL)
	{
		return -1;
	}

	SDL_Rect r = {b.x,b.y,b.w,b.h};

	int BlitSurface_res = SDL_BlitSurface(src,&r,cpy,NULL); 
	//Copy the region of src described by b into cpy
	int image_res = image_save(cpy,path);
	if (image_res == 0 && BlitSurface_res == 0)
	{
		SDL_FreeSurface(cpy);
		return 0;
	}
	else
	{
		SDL_FreeSurface(cpy);
		return -1;
	}
}
