#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <SDL2/SDL.h>

#include "preprocessing.h"

#define WINDOW_WIDTH 1180
#define WINDOW_HEIGHT 780

#define IMAGE_MAX_WIDTH 900
#define IMAGE_MAX_HEIGHT 500

enum step
{
    STEP_ORIGINAL,
    STEP_GRAY,
    STEP_BW,
    STEP_ROTATE,
    STEP_COUNT
};

/* Images and data of the current picture. */
struct image
{
    char *path;
    SDL_Surface *original;
    SDL_Surface *gray;
    SDL_Surface *bw;
    SDL_Surface *rotated;
    double angle;
};

struct work
{
    GtkWidget *title;
    GtkWidget *info;
    GtkWidget *view;
    GtkWidget *rotate_scale;
    GtkWidget *prev;
    GtkWidget *next;
};

/* Widgets and data shared by every callback. */
struct app
{
    GtkWidget *window;
    GtkWidget *stack;
    int step;
    struct work work;
    struct image img;
};

void init_window(int *argc, char ***argv, GtkWidget **window);
int gui_run(int argc, char **argv);
void load_theme(void);
GtkWidget *build_welcome(struct app *app);
GtkWidget *build_work(struct app *app);
void on_import_clicked(GtkWidget *button, gpointer data);

#endif /* GUI_H */
