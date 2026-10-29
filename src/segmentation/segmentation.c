#include "segmentation.h"
#include "image.h"
#include <stdlib.h>

static int is_black(SDL_Surface *s, int x, int y)
{
	Uint8 *base = s->pixels;
	Uint8 *row = base + y * s->pitch;
	Uint32 px = ((Uint32 *)row)[x];

	Uint8 r, g, b;
	SDL_GetRGB(px, s->format, &r, &g, &b);
	return (r < 128);
}

static int row_has_black(SDL_Surface *bw, struct box area, int y)
{
	for (int x = area.x; x < area.x + area.w; x++)
		if (is_black(bw, x, y)) return 1;
	return 0;
}

static int col_has_black(SDL_Surface *bw, struct box area, int x)
{
	for (int y = area.y; y < area.y + area.h; y++)
		if (is_black(bw, x, y)) return 1;
	return 0;
}

struct box *seg_split_rows(SDL_Surface *bw, struct box area, size_t *count)
{
	*count = 0;
	size_t n = 0;
	int in_band = 0, black = 0;
	for (int y = area.y; y < area.y + area.h; y++) {
		black = row_has_black(bw, area, y);
		if (black && !in_band) { n++; in_band = 1; }
		else if (!black) in_band = 0;
	}
	if (n == 0) return NULL;

	struct box *bands = malloc(n * sizeof(struct box));
	if (bands == NULL) return NULL;

	*count = n;
	size_t i = 0;
	int start = 0;
	in_band = 0;

	for (int y = area.y; y < area.y + area.h; y++) {
		black = row_has_black(bw, area, y);
		if (black && !in_band) { start = y; in_band = 1; }
		if (!black && in_band) {
			bands[i].x = area.x;
			bands[i].y = start;
			bands[i].w = area.w;
			bands[i].h = (y - 1) - start + 1;
			i++; in_band = 0;
		}
	}
	if (in_band) {
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
	size_t n = 0;
	int in_band = 0, black = 0;
	for (int x = area.x; x < area.x + area.w; x++) {
		black = col_has_black(bw, area, x);
		if (black && !in_band) { n++; in_band = 1; }
		else if (!black) in_band = 0;
	}
	if (n == 0) return NULL;

	struct box *bands = malloc(n * sizeof(struct box));
	if (bands == NULL) return NULL;

	*count = n;
	size_t i = 0;
	int start = 0;
	in_band = 0;

	for (int x = area.x; x < area.x + area.w; x++) {
		black = col_has_black(bw, area, x);
		if (black && !in_band) { start = x; in_band = 1; }
		if (!black && in_band) {
			bands[i].x = start;
			bands[i].y = area.y;
			bands[i].w = (x - 1) - start + 1;
			bands[i].h = area.h;
			i++; in_band = 0;
		}
	}
	if (in_band) {
		bands[i].x = start;
		bands[i].y = area.y;
		bands[i].w = (area.x + area.w - 1) - start + 1;
		bands[i].h = area.h;
	}
	return bands;
}

int seg_save_box(SDL_Surface *src, struct box b, const char *path)
{
	if (b.w <= 0 || b.h <= 0 || b.x < 0 || b.y < 0
	    || b.x + b.w > src->w || b.y + b.h > src->h) return -1;

	SDL_Surface *cpy = SDL_CreateRGBSurfaceWithFormat(0, b.w, b.h, 32,
	                        SDL_PIXELFORMAT_ARGB8888);
	if (cpy == NULL) return -1;

	SDL_Rect r = {b.x, b.y, b.w, b.h};
	int blit_res = SDL_BlitSurface(src, &r, cpy, NULL);
	int save_res = image_save(cpy, path);
	SDL_FreeSurface(cpy);

	return (blit_res == 0 && save_res == 0) ? 0 : -1;
}

static int in_box(struct box b, int x, int y)
{
	return (x >= b.x && x < b.x + b.w && y >= b.y && y < b.y + b.h);
}

int seg_find_list(SDL_Surface *bw, struct box grid, struct box *list)
{
	int min_x = bw->w, max_x = -1, min_y = bw->h, max_y = -1;

	for (int x = 0; x < bw->w; x++) {
		for (int y = 0; y < bw->h; y++) {
			if (in_box(grid, x, y)) continue;
			if (is_black(bw, x, y)) {
				if (x < min_x) min_x = x;
				if (y < min_y) min_y = y;
				if (x > max_x) max_x = x;
				if (y > max_y) max_y = y;
			}
		}
	}
	if (max_x == -1 || max_y == -1) return -1;

	list->x = min_x; list->y = min_y;
	list->w = max_x - min_x + 1;
	list->h = max_y - min_y + 1;
	return 0;
}

#define GRID_THRESHOLD 0.25 /* 25% of image for outer grid border */
#define LINE_THRESHOLD 0.8  /* 80% of grid for inner cell lines */
#define MARGIN_RATIO 0.1    /* 10% margin inside cells */

static int row_black_count(SDL_Surface *bw, struct box area, int y) {
	int count = 0;
	for (int x = area.x; x < area.x + area.w; x++)
		if (is_black(bw, x, y)) count++;
	return count;
}

static int col_black_count(SDL_Surface *bw, struct box area, int x) {
	int count = 0;
	for (int y = area.y; y < area.y + area.h; y++)
		if (is_black(bw, x, y)) count++;
	return count;
}

int seg_find_grid(SDL_Surface *bw, struct box *grid)
{
	int left = -1, right = -1, top = -1, bottom = -1;

	for (int x = 0; x < bw->w; x++) {
		int count = 0;
		for (int y = 0; y < bw->h; y++) if (is_black(bw, x, y)) count++;
		if (count > bw->h * GRID_THRESHOLD) { left = x; break; }
	}

	for (int x = bw->w - 1; x >= 0; x--) {
		int count = 0;
		for (int y = 0; y < bw->h; y++) if (is_black(bw, x, y)) count++;
		if (count > bw->h * GRID_THRESHOLD) { right = x; break; }
	}

	for (int y = 0; y < bw->h; y++) {
		int count = 0;
		for (int x = 0; x < bw->w; x++) if (is_black(bw, x, y)) count++;
		if (count > bw->w * GRID_THRESHOLD) { top = y; break; }
	}

	for (int y = bw->h - 1; y >= 0; y--) {
		int count = 0;
		for (int x = 0; x < bw->w; x++) if (is_black(bw, x, y)) count++;
		if (count > bw->w * GRID_THRESHOLD) { bottom = y; break; }
	}

	if (left == -1 || right == -1 || top == -1 || bottom == -1) return 0;

	grid->x = left; grid->y = top;
	grid->w = right - left + 1;
	grid->h = bottom - top + 1;
	return 1;
}

struct box *seg_grid_cells(SDL_Surface *bw, struct box grid, int *rows, int *cols)
{
	int *h_lines = malloc(sizeof(int) * grid.h);
	int *v_lines = malloc(sizeof(int) * grid.w);
	if (!h_lines || !v_lines) { free(h_lines); free(v_lines); return NULL; }

	int h_count = 0, v_count = 0;

	for (int y = grid.y; y < grid.y + grid.h; y++) {
		if (row_black_count(bw, grid, y) > grid.w * LINE_THRESHOLD) {
			if (h_count == 0 || y - h_lines[h_count - 1] > 5)
				h_lines[h_count++] = y;
		}
	}

	for (int x = grid.x; x < grid.x + grid.w; x++) {
		if (col_black_count(bw, grid, x) > grid.h * LINE_THRESHOLD) {
			if (v_count == 0 || x - v_lines[v_count - 1] > 5)
				v_lines[v_count++] = x;
		}
	}

	*rows = h_count - 1;
	*cols = v_count - 1;

	if (*rows <= 0 || *cols <= 0) {
		free(h_lines); free(v_lines); return NULL;
	}

	struct box *cells = malloc((*rows) * (*cols) * sizeof(struct box));
	if (!cells) { free(h_lines); free(v_lines); return NULL; }

	for (int r = 0; r < *rows; r++) {
		for (int c = 0; c < *cols; c++) {
			int idx = r * (*cols) + c;
			int raw_x = v_lines[c];
			int raw_y = h_lines[r];
			int raw_w = v_lines[c + 1] - v_lines[c];
			int raw_h = h_lines[r + 1] - h_lines[r];

			int margin_x = raw_w * MARGIN_RATIO;
			int margin_y = raw_h * MARGIN_RATIO;

			cells[idx].x = raw_x + margin_x;
			cells[idx].y = raw_y + margin_y;
			cells[idx].w = raw_w - (2 * margin_x);
			cells[idx].h = raw_h - (2 * margin_y);
		}
	}
	free(h_lines); free(v_lines);
	return cells;
}
