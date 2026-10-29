#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "image.h"
#include "preprocessing.h"
#include "segmentation.h"

/* Save every cell of the grid. Returns the number of saved letters. */
static size_t save_grid_letters(SDL_Surface *bw, struct box grid)
{
	size_t nb_saved = 0;
	int rows = 0;
	int cols = 0;
	struct box *cells = seg_grid_cells(bw, grid, &rows, &cols);
	if (cells == NULL)
	{
		fprintf(stderr, "segment: no letters found in the grid\n");
		return 0;
	}

	printf("Grid: x=%d y=%d w=%d h=%d, %dx%d cells\n",
	       grid.x, grid.y, grid.w, grid.h, rows, cols);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			char path[128];
			snprintf(path, sizeof(path),
			         "out/grid/grid_%02d_%02d.png", i, j);

			if (seg_save_box(bw, cells[i * cols + j], path) == 0)
			{
				nb_saved++;
			}
		}
	}

	free(cells);
	return nb_saved;
}

/* Save every letter of every word of the list. Returns the count. */
static size_t save_list_letters(SDL_Surface *bw, struct box list)
{
	size_t nb_words = 0;
	size_t nb_saved = 0;
	struct box *words = seg_split_rows(bw, list, &nb_words);

	if (words == NULL)
	{
		fprintf(stderr, "segment: no word found in the list\n");
		return 0;
	}

	printf("List: x=%d y=%d w=%d h=%d, %zu words\n",
	       list.x, list.y, list.w, list.h, nb_words);

	for (size_t i = 0; i < nb_words; i++)
	{
		char word_path[128];
		snprintf(word_path, sizeof(word_path),
		         "out/words/word_%02zu.png", i);
		seg_save_box(bw, words[i], word_path);

		size_t nb_letters = 0;
		struct box *letters = seg_split_cols(bw, words[i], &nb_letters);
		for (size_t j = 0; j < nb_letters; j++)
		{
			char path[128];
			snprintf(path, sizeof(path),
			         "out/words/word_%02zu_letter_%02zu.png", i, j);

			if (seg_save_box(bw, letters[j], path) == 0)
			{
				nb_saved++;
			}
		}
		free(letters);
	}
	free(words);

	return nb_saved;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <image>\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (SDL_Init(0) != 0) 
	{
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (image_init() != 0)
	{
		return EXIT_FAILURE;
	}

	mkdir("out", 0777);
	mkdir("out/grid", 0777);
	mkdir("out/words", 0777);

	int status = EXIT_FAILURE;
	SDL_Surface *img = NULL;
	SDL_Surface *bw = NULL;
	struct box grid;
	struct box list;
	int ret;

	img = image_load(argv[1]);
	if (img == NULL)
	{
		fprintf(stderr, "segment: image not found\n");
		image_quit();
		return status;
	}

	bw = binarize_image(img);
	if (bw == NULL)
	{
		fprintf(stderr, "segment: binarization failed\n");
		SDL_FreeSurface(img);
		image_quit();
		return status;
	}

	ret = seg_find_grid(bw, &grid);
	/* IMPORTANT: ret is 1 on success, 0 on failure */
	if (ret == 0 || grid.w <= 0 || grid.h <= 0)
	{
		fprintf(stderr, "segment: grid not found\n");
		SDL_FreeSurface(bw);
		SDL_FreeSurface(img);
		image_quit();
		return status;
	}

	size_t save_grid = save_grid_letters(bw, grid);

	ret = seg_find_list(bw, grid, &list);
	if (ret == -1)
	{
		fprintf(stderr, "segment: list not found\n");
		SDL_FreeSurface(bw);
		SDL_FreeSurface(img);
		image_quit();
		return status;
	}

	size_t save_list = save_list_letters(bw, list);

	printf("Saved %zu grid letters\n", save_grid);
	printf("Saved %zu list letters\n", save_list);

	status = EXIT_SUCCESS;

	SDL_FreeSurface(bw);
	SDL_FreeSurface(img);
	image_quit();
	SDL_Quit();
	return status;
}
