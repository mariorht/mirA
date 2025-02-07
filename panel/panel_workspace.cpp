#include <gtk/gtk.h>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// Archivo donde se almacena el número del workspace
const std::string CONFIG_FILE = "/tmp/workspace_id";

GtkWidget *label;  // Etiqueta para mostrar el número del workspace

// Función para leer el workspace desde el archivo
std::string read_workspace()
{
    std::ifstream file(CONFIG_FILE);
    std::string workspace = "N/A";  // Valor por defecto

    if (file.is_open())
    {
        std::getline(file, workspace);
        file.close();
    }

    return workspace;
}

// Función para actualizar el label en la UI
gboolean update_label(gpointer data)
{
    std::string workspace = read_workspace();
    gtk_label_set_text(GTK_LABEL(label), ("Workspace: " + workspace).c_str());
    return G_SOURCE_CONTINUE;  // Seguir ejecutando
}

// Hilo para actualizar cada segundo
void start_monitoring()
{
    while (true)
    {
        g_idle_add(update_label, nullptr);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Crear ventana flotante sin bordes
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Workspace Panel");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 50);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);  // Sin bordes
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);  // Siempre arriba
    gtk_window_move(GTK_WINDOW(window), 20, 20);  // Posición en pantalla

    // Crear etiqueta
    label = gtk_label_new(("Workspace: " + read_workspace()).c_str());
    gtk_container_add(GTK_CONTAINER(window), label);

    // Cerrar aplicación al cerrar ventana
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crear hilo para actualizar el panel
    std::thread monitor_thread(start_monitoring);
    monitor_thread.detach();

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
