#include <gtk/gtk.h>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>

// File where the workspace number is stored
const std::string CONFIG_FILE = "/tmp/workspace_id";

GtkWidget *workspace_label; // Label for workspace
GtkWidget *clock_label;     // Label for clock

// Function to read the workspace from the file
std::string read_workspace()
{
    std::ifstream file(CONFIG_FILE);
    std::string workspace = "N/A"; // Default value

    if (file.is_open())
    {
        std::getline(file, workspace);
        file.close();
    }

    return workspace;
}

// Function to update the workspace label
gboolean update_workspace_label(gpointer data)
{
    (void)data;
    std::string workspace = read_workspace();
    gtk_label_set_text(GTK_LABEL(workspace_label), ("Workspace: " + workspace).c_str());
    return G_SOURCE_CONTINUE;
}

// Function to update the clock label
gboolean update_clock_label(gpointer data)
{
    (void)data;
    time_t now = time(0);
    struct tm *ltm = localtime(&now);
    
    char time_str[10];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", ltm);

    gtk_label_set_text(GTK_LABEL(clock_label), time_str);
    return G_SOURCE_CONTINUE;
}

// Thread to monitor workspace changes
void start_monitoring()
{
    while (true)
    {
        g_idle_add(update_workspace_label, nullptr);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// Thread to update clock every second
void start_clock()
{
    while (true)
    {
        g_idle_add(update_clock_label, nullptr);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Create floating window without borders
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Workspace Panel");
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE); // No borders


    // Create horizontal box layout
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    // Create workspace label (left)
    workspace_label = gtk_label_new(("Workspace: " + read_workspace()).c_str());
    gtk_box_pack_start(GTK_BOX(hbox), workspace_label, FALSE, TRUE, 50);

    // Create a spacer in the middle
    GtkWidget *spacer = gtk_label_new(""); 
    gtk_box_pack_start(GTK_BOX(hbox), spacer, TRUE, TRUE, 10);

    // Create clock label (right)
    clock_label = gtk_label_new("00:00:00");
    gtk_box_pack_end(GTK_BOX(hbox), clock_label, FALSE, TRUE, 50);


    // Close application on window close
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Start monitoring thread
    std::thread monitor_thread(start_monitoring);
    monitor_thread.detach();

    // Start clock update thread
    std::thread clock_thread(start_clock);
    clock_thread.detach();

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
