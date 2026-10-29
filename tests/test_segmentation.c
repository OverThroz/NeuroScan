#include <stdio.h>
#include <stdlib.h>

#include <segmentation.h>

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
	SDL_FreeSurface(s);
	return 0;	

}
