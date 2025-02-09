#include <gtk/gtk.h>
#include <cairo.h>
#include <iostream>

// Ruta de la imagen de fondo (se obtiene desde el argumento)
std::string image_path;

// Función para dibujar el fondo
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    (void)user_data;

    cairo_surface_t *image = cairo_image_surface_create_from_png(image_path.c_str());
    if (cairo_surface_status(image) != CAIRO_STATUS_SUCCESS)
    {
        g_printerr("Error al cargar la imagen: %s\n", image_path.c_str());
        return FALSE;
    }

    // Obtener dimensiones de la ventana
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);
    
    // Escalar la imagen para que cubra toda la pantalla
    double img_width = cairo_image_surface_get_width(image);
    double img_height = cairo_image_surface_get_height(image);
    double scale_x = width / img_width;
    double scale_y = height / img_height;
    double scale = std::max(scale_x, scale_y); // Mantener proporción

    cairo_save(cr);
    cairo_scale(cr, scale, scale);
    cairo_set_source_surface(cr, image, 0, 0);
    cairo_paint(cr);
    cairo_restore(cr);

    cairo_surface_destroy(image);
    return FALSE;
}

int main(int argc, char *argv[])
{
    // Validar argumento
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " /ruta/a/tu/wallpaper.png\n";
        return 1;
    }

    image_path = argv[1];

    gtk_init(&argc, &argv);

    // Crear ventana sin bordes y maximizada
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Wallpaper");
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE); // Sin bordes
    gtk_widget_set_app_paintable(window, TRUE); // Permitir el dibujo personalizado

    // Manejar evento de dibujo
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
