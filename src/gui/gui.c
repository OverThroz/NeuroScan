#include "gui.h"
#include "SDL_pixels.h"
#include "SDL_surface.h"
#include "gdk-pixbuf/gdk-pixbuf.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtkcssprovider.h"
#include "image.h"

#include <err.h>
#include <gtk/gtk.h>
#include <stdlib.h>

/*
===== Cour GTK =====
Au cas ou ou vous tomber la dessus c'est pr que je comprenne GTK
GTK MAJUSCULE = macro de conversion ex : GTK_WINDOW
gtk minuscule = fonction de gtk

Conteneur : a l'inverse de SDL avec GTK on on renge des widgets das des boite (GtkBox) ou gtk les places lui meme donc pas besoin de coordonne

Pour le titre, le point indique une classe : la règle s'applique uniquement aux widgets à qui on a donné cette classe

GtkStack est le fait d'enpiler des page "stack" pour passer de l'une a l'autre

label = texte et app->info  text of the work page, updated after import

struct data pk c'est utilise car cest GTK qui appel les fonctions comme pour lorsqu'on clic sur le bouton mais dcp on a pas bcp d'info et on a bessoin de plein de truc pour que sa marche donc on creer une struct pour palie a sa
window = fenettre addresse principale pour lorsqu'on doit switch et autre office de parents pr le stack
stack (pile pour switch de fenetre)
info (changer texte principalemetn pr l'import mtn)
path (retiens l'image pr la suite des etapes blanc et noir et etc)

g_free = free bloc memoir GTK

GdkPixbuf c'est quoi ??????????

*/

static const char *THEME_CSS =
	"window { background-color: #0b1a33; color: #e6edf7; }" //window = tt avc regle a appliaue par exemple le fond et autre plus simple que RVB
	"headerbar { background-image: none; background-color: #11284d;"
	" border: none; box-shadow: none; }"
	"headerbar button { color: #e6edf7; }"
	".welcome-text { font-size: 20px; color: #94a3b8; }"
	".import-button { background-image: none; background-color: #2f7df6;"
	" color: #ffffff; border: none; box-shadow: none; text-shadow: none;"
	" border-radius: 14px; padding: 22px 70px; margin-top: 40px;"
	" font-size: 22px; font-weight: bold; }"
	".import-button:hover { background-color: #5a9cff;"
	" box-shadow: 0 6px 16px rgba(0, 0, 0, 0.6); }"
	".import-button:active { background-color: #1a46c4;"
                        " box-shadow: none; }";

//A comprendre et savoir expliquer
//Une image c'est du RGBA et pb SDL range pas parei que GTK
static GdkPixbuf *surface_to_pixbuf(SDL_Surface *surface)
{
    SDL_Surface *rgba;
    GdkPixbuf *view;
    GdkPixbuf *pixbuf; //resulat final version de rgba sosus GTK

    rgba = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    if (rgba == NULL)
    {
        return NULL;
    }
    view = gdk_pixbuf_new_from_data(rgba->pixels, GDK_COLORSPACE_RGB,
		TRUE, 8, rgba->w, rgba->h, rgba->pitch, NULL, NULL); // on regarde tout les pixels en memoire qui sont sous forme rgba
    pixbuf = gdk_pixbuf_copy(view); //on diminue le compteur de reference en disant qu'on a pas besoin
    g_object_unref(view);
    SDL_FreeSurface(rgba);
    return pixbuf;

}

static void show_surface(struct app *app, SDL_Surface *surface)
{
    GdkPixbuf *full;
    GdkPixbuf *scaled;
    double scale;
    double scale_h;
    int width;
    int height;

    full = surface_to_pixbuf(surface);
    if (full == NULL)
    {
        warnx("show_surface: %s", SDL_GetError()); //warnx et pas erx pour dire que l'erreur n'est pas bloquane on arrete pas tout
        return;
    }

    scale = (double)IMAGE_MAX_WIDTH / surface->w;
    scale_h = (double)IMAGE_MAX_HEIGHT / surface->h;
    if (scale_h < scale) // on garde le plus petit des deux, on a deux cas possible par prevision au prent le plus petit
    {
        scale = scale_h;
    }
    int int_surface_W = (surface->w * scale);
    int int_surface_H = (surface->h * scale);
    width = int_surface_W;
    height = int_surface_H;

    if (width < 1 || height < 1)
    {
        width = 1;
        height = 1;
    }
    scaled = gdk_pixbuf_scale_simple(full, width,height,
                    GDK_INTERP_BILINEAR); //creer une nouvelle iamge a la taille demander
    g_object_unref(full);
    gtk_image_set_from_pixbuf(GTK_IMAGE(app->image), scaled);
    g_object_unref(scaled);
}

static GtkFileFilter *build_image_filter(void)
{
    GtkFileFilter *filter;  //creer un filtre

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Images (PNG, JPEG, BMP)");
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_filter_add_mime_type(filter,"image/jpeg");
    gtk_file_filter_add_mime_type(filter, "image/bmp");
    return filter;
}

static char *choose_image(GtkWindow *parent)
{
    GtkWidget *dialog;
    char *path = NULL;

    dialog = gtk_file_chooser_dialog_new("Choisir une image", parent,       //creation feunettre choix
         GTK_FILE_CHOOSER_ACTION_OPEN, "_Annuler", GTK_RESPONSE_CANCEL,
          "_Ouvrir",GTK_RESPONSE_ACCEPT,NULL);

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),build_image_filter()); // demande a gtk d'appeler la fonction en regardant si elle respecte bien les criteres
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), "tests"); // pour mettre la ou sa s'ouvre par defaut modifier avec les imges pour etre plus rapide

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }
    gtk_widget_destroy(dialog);
    return path;

}

static void show_error(GtkWindow *parent , const char *message)
{
    GtkWidget *dialog;

    dialog = gtk_message_dialog_new(parent, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", message); //crrer la fenettre d'erreur
    gtk_dialog_run(GTK_DIALOG(dialog)); //affiche un dialogue et atetnds t'appuyer sr okay
    gtk_widget_destroy(dialog);
}

void on_clicked(GtkWidget *button, gpointer data)
{
    struct app *app = data;
    SDL_Surface *surface;
    char *path;
    char *name;
    char *text;

    (void)button;
    path = choose_image(GTK_WINDOW(app->window));
    if (path == NULL)
    {
        return;
    }
    surface = image_load(path);
    if (surface == NULL) // image_load renvoie NULL si le fichier est illisible
    {
        show_error(GTK_WINDOW(app->window),
        "Ce fichier n'est pas une image lisible");
        g_free(path);
        return;
    }
    SDL_FreeSurface(app->img.original);

    app->img.original = surface;
    g_free(app->img.path);
    app->img.path = path;

    show_surface(app, app->img.original);
    name = g_path_get_basename(path); //garde le nom du fichier
    text = g_strdup_printf("%s\n%d x %d pixels", name, surface->w, surface->h);
    gtk_label_set_text(GTK_LABEL(app->info), text);
    g_free(name);
    g_free(text);

    gtk_stack_set_visible_child_name(GTK_STACK(app->stack), "work");
}

GtkWidget *build_welcome(struct app *app)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12); // GTK_ORIENTATION_VERTICAL on add a la vercical 16 = distance entre 2 pixel
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER); // centre horizontal
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER); // centre vertical

    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("src/gui/data.png",400,-1, TRUE, &error);
    if (pixbuf == NULL)
    {
        warnx("logo: %s", error->message);
        g_error_free(error);
    }
    else
    {
        GtkWidget *logo = gtk_image_new_from_pixbuf(pixbuf);
        gtk_container_add(GTK_CONTAINER(box), logo);
        g_object_unref(pixbuf);
    }

    GtkWidget *text = gtk_label_new("Résolvez une grille de mots cachés "
                                    "à partir d'une simple image");
    gtk_style_context_add_class(gtk_widget_get_style_context(text), "welcome-text");
    gtk_container_add(GTK_CONTAINER(box),text);

    //Bouton
    GtkWidget *button = gtk_button_new_with_label("Importer une image");
    g_signal_connect(button, "clicked", G_CALLBACK(on_clicked),
                            app);
    gtk_style_context_add_class(gtk_widget_get_style_context(button),
                                "import-button");
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(box), button);


    return box;
}

GtkWidget *build_work(struct app *app)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL,16);
        gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

        app->image = gtk_image_new();
	    gtk_container_add(GTK_CONTAINER(box), app->image);

        app->info = gtk_label_new("Aucune image");
        gtk_label_set_justify(GTK_LABEL(app->info), GTK_JUSTIFY_CENTER);
        gtk_container_add(GTK_CONTAINER(box), app->info);

        return box;
}

void load_theme(void)
{
    g_object_set(gtk_settings_get_default(),"gtk-application-prefer-dark-theme", TRUE, NULL);
    GtkCssProvider *provider = gtk_css_provider_new(); // une (boite) vide comme SDL_CreateRenderer
    GError *error = NULL; //struct de gtk avec 3 paramettre pr les erreurs

    if (gtk_css_provider_load_from_data(provider, THEME_CSS, -1, &error) == FALSE)
    {
        errx(EXIT_FAILURE, "load-theme: %s", error->message); //mesage dans la struct error
    }

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION); //utilise la boite crer dnc provider sur tt ecran
    //gdk_screen_get_default : couche par defaut
    //GTK_STYLE_PROVIDER : convertie au bon format
    //GTK_STYLE_PROVIDER_PRIORITY_APPLICATION : modifier le theme avec les couleurs qu'on veut

    g_object_unref(provider); //fin de la fonction / libere l'object
}


void init_window(int *argc, char ***argv, GtkWidget **window)
{
    if (gtk_init_check(argc,argv) == FALSE) // gtk pas NULL mais false si pas d'ecran
    {
        errx(EXIT_FAILURE, "gtk_init: cannot open the display");
    }

    *window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //creer une nouvelle feunettre
    if( *window == NULL)
    {
        errx(EXIT_FAILURE,"gtk_window_new: cannot create the window");
    }

    gtk_window_set_title(GTK_WINDOW(*window), "NeuroScan");
    gtk_window_set_default_size(GTK_WINDOW(*window), WINDOW_WIDTH,
                                    WINDOW_HEIGHT);
    g_signal_connect(*window, "destroy", G_CALLBACK(gtk_main_quit),NULL); //en gros auqdn on appelle destroy sa fere la fenetre
}

int gui_run(int argc, char **argv)
{
    struct app app;
    app.window = NULL;
    app.info = NULL;
    app.image = NULL;
    app.stack = NULL;
    app.img.original = NULL;

    init_window(&argc, &argv, &app.window);
    app.img.path = NULL;
    load_theme();

    app.stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(app.stack),
                            GTK_STACK_TRANSITION_TYPE_SLIDE_UP);
    gtk_stack_set_transition_duration(GTK_STACK(app.stack), 300);

    gtk_stack_add_named(GTK_STACK(app.stack), build_welcome(&app),
                                         "welcome");
    gtk_stack_add_named(GTK_STACK(app.stack), build_work(&app), "work");


    gtk_container_add(GTK_CONTAINER(app.window), app.stack); //convertie widow et appel build_welcome
    gtk_widget_show_all(app.window);
	gtk_main(); // remplace  la boucle while de SDL

    g_free(app.img.path);
    SDL_FreeSurface(app.img.original);
    return EXIT_SUCCESS;
}
