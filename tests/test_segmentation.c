#include <stdio.h>
#include <stdlib.h>

#include <segmentation.h>
#include "image.h"

int main(void)
{
	SDL_Surface *s = SDL_CreateRGBSurfaceWithFormat(0, 10, 10, 32,
			SDL_PIXELFORMAT_ARGB8888);
	struct box area = {0,0,10,10};
	size_t count;
	struct box *rows;

	//Case 1: two words separated by empty rows
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format,255,255,255));
	SDL_Rect w1 = {2,1,6,2}; // rows 1-2 
	SDL_Rect w2 = {3,5,4,3}; // rows 5-7
	
	SDL_FillRect(s, &w1, SDL_MapRGB(s->format, 0, 0, 0));
	SDL_FillRect(s, &w2, SDL_MapRGB(s->format, 0, 0, 0));

	rows = seg_split_rows(s, area, &count);
	printf("========== TEST 1 ==========\n");
	printf("count=%zu\n", count);
	for (size_t i = 0; i < count ; i++)
	{
		printf("%zu: x=%d y=%d w=%d h=%d\n",
			       	i,rows[i].x, rows[i].y, rows[i].w, rows[i].h);
	}
	free(rows);

	//Case 2: one word at the end
	SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 255, 255, 255));
	SDL_Rect w3 = { 1, 7, 5, 3 }; //rows 7-9
	SDL_FillRect(s, &w3, SDL_MapRGB(s->format, 0, 0, 0));

	rows = seg_split_rows(s, area, &count);
	printf("========== TEST 2 ==========\n");
	printf("count=%zu\n", count);
	for (size_t i = 0; i < count ; i++)
        {
                printf("%zu: x=%d y=%d w=%d h=%d\n",
                                i,rows[i].x, rows[i].y, rows[i].w, rows[i].h);
        }
        free(rows);

	printf("========== TEST 3 ==========\n");
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format,255,255,255));
	SDL_Rect h1 = {1,2,2,5}; // cols 1-2
	SDL_Rect h2 = {5,2,3,5}; // cols 5-7
	SDL_FillRect(s, &h1, SDL_MapRGB(s->format, 0, 0, 0));
	SDL_FillRect(s, &h2, SDL_MapRGB(s->format, 0, 0, 0));

	rows = seg_split_cols(s, area, &count);
	printf("count=%zu\n", count);
        for (size_t i = 0; i < count ; i++)
        {
                printf("%zu: x=%d y=%d w=%d h=%d\n",
                                i,rows[i].x, rows[i].y, rows[i].w, rows[i].h);
        }
        free(rows);
	
	printf("========== TEST 4 ==========\n");
        SDL_FillRect(s,NULL,SDL_MapRGB(s->format,255,255,255));
        SDL_Rect h3 = {7,2,3,5}; // cols 7-9 last letter
        SDL_FillRect(s, &h3, SDL_MapRGB(s->format, 0, 0, 0));
	struct box sub = { 2, 1, 8, 8 };

        rows = seg_split_cols(s, sub, &count);
        printf("count=%zu\n", count);
        for (size_t i = 0; i < count ; i++)
        {
                printf("%zu: x=%d y=%d w=%d h=%d\n",
                                i,rows[i].x, rows[i].y, rows[i].w, rows[i].h);
        }
        free(rows);

	printf("========== TEST 5 ==========\n");
	image_init();
	SDL_FillRect(s,NULL,SDL_MapRGB(s->format, 255, 255, 255));
	struct box ok = {5,2,3,5};
	struct box edge = {7,0,3,10};
	struct box out = {8,8,5,5};
	struct box flat = {2,2,0,4};

	printf("ok: ret=%d\n", seg_save_box(s,ok, "/tmp/letter.png"));
	printf("edge: ret=%d\n", seg_save_box(s,edge, "/tmp/edge.png"));
	printf("out: ret=%d\n", seg_save_box(s,out, "/tmp/out.png"));
	printf("flat: ret=%d\n", seg_save_box(s,flat, "/tmp/flat.png"));

	SDL_Surface *taille = image_load("/tmp/letter.png");
	if (taille != NULL)
	{
		printf("size=%dx%d\n" ,taille->w, taille->h);
	}
	SDL_FreeSurface(s);
	SDL_FreeSurface(taille);
	image_quit();
	
	return 0;	

}
