#include "image.h"
#include "preprocessing.h"

#include <stdio.h>
#include <stdlib.h>

static int parse_angle(const char *str, double *angle)
{
	if (str == NULL)
	{
		return -1;
	}
	
	char *end;
	double val = strtod(str, &end);
	if (end == str || *end != '\0')
	{
		return -1;
	}
	else
	{
		*angle = val;
		return 0;
	}
}

int main (int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <image> <angle_degrees>\n", argv[0]);
		return EXIT_FAILURE;
	}
	double angle;
	if (parse_angle(argv[2], &angle) != 0)
	{
		fprintf(stderr, "preprocess: invalid angle: %s\n", argv[2]);
		return EXIT_FAILURE;
	}

	if (image_init() != 0)
	{
		return EXIT_FAILURE;
	}

	int status = EXIT_FAILURE;
	SDL_Surface *img = NULL;
	SDL_Surface *gray = NULL;
	SDL_Surface *bw = NULL;
	SDL_Surface *rot = NULL;

	img = image_load(argv[1]);
	if (img != NULL)
	{
		gray = convert_to_grayscale(img);
		bw = binarize_image(img);
		if (bw != NULL)
		{
			rot = rotate_image(bw, angle);
		}
	}

	if (gray != NULL && bw != NULL && rot != NULL)
	{
		if (image_save(gray, "out_gray.png") == 0
    			&& image_save(bw, "out_bw.png") == 0
    			&& image_save(rot, "out_rotated.png") == 0)
		{
			printf("Saved: out_gray.png\n");
			printf("Saved: out_bw.png\n");
			printf("Saved: out_rotated.png\n");
			status = EXIT_SUCCESS;
		}
	}

	SDL_FreeSurface(img);
	SDL_FreeSurface(gray);
	SDL_FreeSurface(bw);
	SDL_FreeSurface(rot);

	image_quit();
	return status ;
}
