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

static int in_box(struct box b, int x, int y)
{
	if (x >= b.x && x < b.x +b.w)
	{
		if (y >= b.y && y < b.y + b.h)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int seg_find_list(SDL_Surface *bw, struct box grid, struct box *list)
{
	int min_x = bw->w;
	int max_x = -1;

	int min_y = bw->h;
	int max_y = -1;

	for (int x = 0 ; x < bw->w ; x++)
	{
		for (int y = 0 ; y < bw->h ; y++)
		{
			if (in_box(grid,x,y) == 1)
			{
				continue;
			}

			if (is_black(bw,x,y) == 1)
			{
				if (x < min_x)
				{
					min_x = x;
				}

				if (y < min_y)
				{
					min_y = y;
				}

				if (x > max_x)
				{
					max_x = x;
				}

				if (y > max_y)
				{
					max_y = y;
				}
			}

		}
	}

	if (max_x == -1 || max_y == -1)
	{
		return -1;
	}

	list->x = min_x;
	list->y = min_y;

	list->w = max_x - min_x +1;
	list->h = max_y - min_y +1;

	return 0;
}


// Assuming image.h is included via segmentation.h for SDL_Surface and is_black

#define LINE_THRESHOLD 0.8 // 80% of pixels must be black to be considered a line
#define MARGIN_RATIO 0.1   // 10% margin on each side (total 20% removed)

// --- HELPER FUNCTIONS ---

// Counts the number of black pixels in a specific row within a given area
static int row_black_count(SDL_Surface *bw, struct box area, int y) {
    int count = 0;
    for (int x = area.x; x < area.x + area.w; x++) {
        if (is_black(bw, x, y)) {
            count++;
        }
    }
    return count;
}

// Counts the number of black pixels in a specific column within a given area
static int col_black_count(SDL_Surface *bw, struct box area, int x) {
    int count = 0;
    for (int y = area.y; y < area.y + area.h; y++) {
        if (is_black(bw, x, y)) {
            count++;
        }
    }
    return count;
}

// --- MAIN SEGMENTATION FUNCTIONS ---

// Finds the bounding box of the main grid
int seg_find_grid(SDL_Surface *bw, struct box *grid) {
    // Initialize boundaries with impossible values to detect if they are found
    int left = -1, right = -1, top = -1, bottom = -1;

    // 1. Find Left Boundary
    for (int x = 0; x < bw->w; x++) {
        int count = 0;
        for (int y = 0; y < bw->h; y++) if (is_black(bw, x, y)) count++;
        if (count > bw->h * LINE_THRESHOLD) { left = x; break; }
    }

    // 2. Find Right Boundary
    for (int x = bw->w - 1; x >= 0; x--) {
        int count = 0;
        for (int y = 0; y < bw->h; y++) if (is_black(bw, x, y)) count++;
        if (count > bw->h * LINE_THRESHOLD) { right = x; break; }
    }

    // 3. Find Top Boundary
    for (int y = 0; y < bw->h; y++) {
        int count = 0;
        for (int x = 0; x < bw->w; x++) if (is_black(bw, x, y)) count++;
        if (count > bw->w * LINE_THRESHOLD) { top = y; break; }
    }

    // 4. Find Bottom Boundary
    for (int y = bw->h - 1; y >= 0; y--) {
        int count = 0;
        for (int x = 0; x < bw->w; x++) if (is_black(bw, x, y)) count++;
        if (count > bw->w * LINE_THRESHOLD) { bottom = y; break; }
    }

    // Check if all boundaries were successfully found
    if (left == -1 || right == -1 || top == -1 || bottom == -1) {
        return 0; // Failure
    }

    // Fill the grid structure with the found coordinates
    grid->x = left;
    grid->y = top;
    grid->w = right - left + 1;
    grid->h = bottom - top + 1;

    return 1; // Success
}

// Extracts the individual cells of the grid and returns an array of boxes
struct box *seg_grid_cells(SDL_Surface *bw, struct box grid, int *rows, int *cols) {
    // Allocate temporary arrays to store the coordinates of horizontal and vertical lines
    // Max possible lines is bounded by the width/height of the grid
    int *h_lines = malloc(sizeof(int) * grid.h);
    int *v_lines = malloc(sizeof(int) * grid.w);
    int h_count = 0;
    int v_count = 0;

    // 1. Find Horizontal Lines
    for (int y = grid.y; y < grid.y + grid.h; y++) {
        if (row_black_count(bw, grid, y) > grid.w * LINE_THRESHOLD) {
            // Check if we are not on the same thick line (avoid duplicates)
            if (h_count == 0 || y - h_lines[h_count - 1] > 5) {
                h_lines[h_count++] = y;
            }
        }
    }

    // 2. Find Vertical Lines
    for (int x = grid.x; x < grid.x + grid.w; x++) {
        if (col_black_count(bw, grid, x) > grid.h * LINE_THRESHOLD) {
            if (v_count == 0 || x - v_lines[v_count - 1] > 5) {
                v_lines[v_count++] = x;
            }
        }
    }

    // Deduce rows and cols (number of lines minus 1)
    *rows = h_count - 1;
    *cols = v_count - 1;

    // If detection failed, return NULL to avoid crashes
    if (*rows <= 0 || *cols <= 0) {
        free(h_lines);
        free(v_lines);
        return NULL;
    }

    // 3. Allocate the array of cells
    struct box *cells = malloc((*rows) * (*cols) * sizeof(struct box));
    if (!cells) {
        free(h_lines);
        free(v_lines);
        return NULL;
    }

    // 4. Fill the array with individual cell bounding boxes
    for (int r = 0; r < *rows; r++) {
        for (int c = 0; c < *cols; c++) {
            int idx = r * (*cols) + c;
            
            int raw_x = v_lines[c];
            int raw_y = h_lines[r];
            int raw_w = v_lines[c + 1] - v_lines[c];
            int raw_h = h_lines[r + 1] - h_lines[r];

            // Apply 20% total margin (10% on each side) to avoid grid lines
            int margin_x = raw_w * MARGIN_RATIO;
            int margin_y = raw_h * MARGIN_RATIO;

            cells[idx].x = raw_x + margin_x;
            cells[idx].y = raw_y + margin_y;
            cells[idx].w = raw_w - (2 * margin_x);
            cells[idx].h = raw_h - (2 * margin_y);
        }
    }

    // Free temporary line arrays
    free(h_lines);
    free(v_lines);

    return cells;
}
