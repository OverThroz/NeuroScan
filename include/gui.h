#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 1180
#define WINDOW_HEIGHT 780

#define IMAGE_MAX_WIDTH 900
#define IMAGE_MAX_HEIGHT 560

/* Images and data of the current picture. */
struct image
{
    char *path;
    SDL_Surface *original;
};

/* Widgets and data shared by every callback. */
struct app
{
    GtkWidget *window;
    GtkWidget *stack;
    GtkWidget *image;
    GtkWidget *info;
    struct image img;
};

void init_window(int *argc, char ***argv, GtkWidget **window);
int gui_run(int argc, char **argv);
void load_theme(void);
GtkWidget *build_welcome(struct app *app);
GtkWidget *build_work(struct app *app);
void on_import_clicked(GtkWidget *button, gpointer data);

#endif /* GUI_H */
