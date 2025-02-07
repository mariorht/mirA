#include "tiling_window_manager.h"
#include <miral/application_info.h>
#include <miral/window_info.h>
#include <miral/window_manager_tools.h>

#include <iostream> 
#include <cmath>
#include <fstream>  // Para manejar archivos

using namespace miral;

TilingWindowManagerPolicy::TilingWindowManagerPolicy(miral::WindowManagerTools const& tools)
    : tools(tools)
{
    create_workspace(1);  // Inicializamos con un workspace por defecto
    saveWorkspaceFile(1);  // Guardamos el workspace por defecto
}

auto TilingWindowManagerPolicy::place_new_window(
    miral::ApplicationInfo const& app_info,
    miral::WindowSpecification const& request_parameters) -> miral::WindowSpecification
{
    (void)app_info;

    return request_parameters;
}

auto TilingWindowManagerPolicy::confirm_placement_on_display(
    miral::WindowInfo const& window_info, MirWindowState new_state, Rectangle const& new_placement) -> Rectangle
{
    (void)window_info;
    (void)new_state;
    return new_placement;
}

auto TilingWindowManagerPolicy::confirm_inherited_move(
    miral::WindowInfo const& window_info, Displacement movement) -> Rectangle
{
    return {window_info.window().top_left() + movement, window_info.window().size()};
}

void TilingWindowManagerPolicy::handle_window_ready(miral::WindowInfo& window_info)
{
    auto window = window_info.window();
    std::string window_name = window_info.name();

    if (window_name == "Workspace Panel")  // Detectamos el panel
    {
        persistent_windows.insert(window);

        miral::WindowSpecification spec;
        spec.top_left() = {0, 0};  // Arriba, esquina izquierda
        spec.size() = {tools.active_output().size.width, 30};  // Todo el ancho, altura de 30px
        spec.state() = mir_window_state_restored;
        tools.modify_window(window, spec);

        panel_window = window;  // Guardamos la referencia del panel
    }
    else
    {
        window_workspace_map[window] = active_workspace;
        tools.add_tree_to_workspace(window, workspaces[active_workspace]);
    }

    update_tiles({tools.active_output()});

    if (window_info.can_be_active() && window_name != "Workspace Panel")  // ✅ Evita que el panel tome foco
    {
        tools.select_active_window(window);
    }
}


void TilingWindowManagerPolicy::handle_modify_window(miral::WindowInfo& window_info, miral::WindowSpecification const& modifications)
{
    (void)window_info;
    (void)modifications;
    
}

void TilingWindowManagerPolicy::handle_request_move(miral::WindowInfo& window_info, MirInputEvent const* input_event)
{
    (void) window_info;
    (void) input_event;

    // No permitimos mover ventanas en tiling
}

void TilingWindowManagerPolicy::handle_request_resize(miral::WindowInfo& window_info, MirInputEvent const* input_event, MirResizeEdge edge)
{
    (void) window_info;
    (void) input_event;
    (void) edge;

    // No permitimos redimensionar en tiling
}

void TilingWindowManagerPolicy::handle_raise_window(miral::WindowInfo& window_info)
{
    tools.select_active_window(window_info.window());
}

bool TilingWindowManagerPolicy::handle_keyboard_event(const MirKeyboardEvent* event)
{
    using namespace miral::toolkit;

    if (mir_keyboard_event_action(event) != mir_keyboard_action_down)
        return false;

    MirInputEventModifiers mods = mir_keyboard_event_modifiers(event);
    bool mod = (mods & mir_input_event_modifier_meta);

    auto window = tools.active_window();
    if (!window) return false;

    // � Cambio de Workspaces (Ctrl + Alt + Número)
    if (mod)
    {
        int key = mir_keyboard_event_keysym(event);
        if (key >= XKB_KEY_1 && key <= XKB_KEY_5)
        {
            int workspace_id = key - XKB_KEY_1 + 1;  // Convierte XKB_KEY_1 en 1, XKB_KEY_2 en 2, etc.
            switch_workspace(workspace_id);
            return true;
        }
    }

    //Resto de atajos de teclado
    switch (mir_keyboard_event_keysym(event))
    {
    // � Alternar pantalla completa (Ctrl + Alt + F)
    case XKB_KEY_f:
        if (mod)
        {
            toggle_fullscreen(window);
            return true;
        }
        break;



    // � Mover Foco (Ctrl + Alt + Izq/Der)
    case XKB_KEY_Left:
        if (mod)
        {
            std::cerr << "[DEBUG] Moviendo foco a la ventana anterior\n";
            tools.focus_prev_within_application();
            return true;
        }
        break;
    case XKB_KEY_Right:
        if (mod)
        {
            std::cerr << "[DEBUG] Moviendo foco a la siguiente ventana\n";
            tools.focus_next_within_application();
            return true;
        }
        break;

    // � Cerrar Ventana Activa (Ctrl + Alt + Q)
    case XKB_KEY_q:
    case XKB_KEY_Q:
        if (mod)
        {
            std::cerr << "[DEBUG] Cerrando ventana activa\n";
            tools.ask_client_to_close(window);
            return true;
        }
        break;

    default:
        return false;  // Dejamos pasar eventos de teclado normales
    }

    return false;
}


bool TilingWindowManagerPolicy::handle_touch_event(const MirTouchEvent* event)
{
    (void) event;

    return false;
}


bool TilingWindowManagerPolicy::handle_pointer_event(const MirPointerEvent* event)
{
    using namespace miral::toolkit;

    if (mir_pointer_event_action(event) != mir_pointer_action_button_down)
    {
        return false;
    }

    Point cursor{
        mir_pointer_event_axis_value(event, mir_pointer_axis_x),
        mir_pointer_event_axis_value(event, mir_pointer_axis_y)};


    miral::Window top_window;  
    int highest_layer = -1;

    tools.for_each_application([&](miral::ApplicationInfo& app_info)
    {
        for (auto const& window : app_info.windows())
        {
            auto& info = tools.info_for(window);

            if(info.name() == "Workspace Panel")
            {
                continue;
            }

            Rectangle rect = {info.window().top_left(), info.window().size()};

            if (cursor.x.as_int() >= rect.top_left.x.as_int() &&
                cursor.x.as_int() < rect.top_left.x.as_int() + rect.size.width.as_int() &&
                cursor.y.as_int() >= rect.top_left.y.as_int() &&
                cursor.y.as_int() < rect.top_left.y.as_int() + rect.size.height.as_int())
            {
                if (static_cast<int>(info.depth_layer()) > highest_layer)
                {
                    highest_layer = static_cast<int>(info.depth_layer());
                    top_window = window;
                }
            }
        }
    });

    if (top_window)
    {
        tools.select_active_window(top_window);
        return false; // No interceptamos el evento, dejamos que la app lo reciba
    }

    return false;
}



void TilingWindowManagerPolicy::handle_request_drag_and_drop(miral::WindowInfo& window_info)
{
    (void) window_info;
    // No soportamos drag & drop en tiling
}

void TilingWindowManagerPolicy::update_tiles(std::vector<Rectangle> const& outputs)
{
    (void) outputs;

    std::vector<miral::Window> tiled_windows;
    miral::Window fullscreen_window;

    int panel_height = 30;  // Altura fija del panel
    int screen_width = tools.active_output().size.width.as_int();
    int screen_height = tools.active_output().size.height.as_int() - panel_height;  // Restamos la altura del panel

    tools.for_each_application([&](miral::ApplicationInfo& app_info)
    {
        for (auto const& window : app_info.windows())
        {
            if (persistent_windows.count(window))  // Ignorar el panel
                continue;

            if (window_workspace_map[window] == active_workspace)
            {
                auto& info = tools.info_for(window);

                if (info.state() == mir_window_state_fullscreen)
                {
                    fullscreen_window = window;
                }
                else
                {
                    tiled_windows.push_back(window);
                }
            }
        }
    });

    if (fullscreen_window)
    {
        std::cerr << "[DEBUG] Modo pantalla completa activado\n";

        miral::WindowSpecification spec;
        spec.top_left() = {0, panel_height};  // Inicia debajo del panel
        spec.size() = {screen_width, screen_height};  // Ocupa toda la pantalla menos el panel
        spec.state() = mir_window_state_fullscreen;
        tools.modify_window(fullscreen_window, spec);

        for (auto& window : tiled_windows)
        {
            miral::WindowSpecification hide_spec;
            hide_spec.state() = mir_window_state_hidden;
            tools.modify_window(window, hide_spec);
        }
        return;
    }

    if (tiled_windows.empty()) return;

    int num_windows = tiled_windows.size();
    std::cerr << "[DEBUG] Recalculando mosaico en workspace " << active_workspace 
              << ", número de ventanas: " << num_windows << "\n";

    int columns = std::ceil(std::sqrt(num_windows));
    int rows = std::ceil(static_cast<float>(num_windows) / columns);

    int window_width = screen_width / columns;
    int window_height = screen_height / rows;  // Restamos la altura del panel

    for (int i = 0; i < num_windows; ++i)
    {
        int col = i % columns;
        int row = i / columns;

        miral::WindowSpecification spec;
        spec.top_left() = {col * window_width, panel_height + row * window_height};  // Inicia debajo del panel
        spec.size() = {window_width, window_height};
        spec.state() = mir_window_state_restored;

        tools.modify_window(tiled_windows[i], spec);
    }
}


void TilingWindowManagerPolicy::advise_delete_window(miral::WindowInfo const& window_info)
{
    std::cerr << "[DEBUG] Ventana eliminada: " << window_info.name() << ". Esperando a que desaparezca...\n";
    
    window_workspace_map.erase(window_info.window());

    dirty_tiles = true;  // Marcamos que necesitamos actualizar el tiling
}


void TilingWindowManagerPolicy::advise_end()
{
    if (dirty_tiles)
    {
        std::cerr << "[DEBUG] Recalculando mosaico al final del ciclo de eventos.\n";
        update_tiles({tools.active_output()});
        dirty_tiles = false;  // Restablecemos el flag
    }
}



void TilingWindowManagerPolicy::create_workspace(int id)
{
    if (workspaces.find(id) == workspaces.end())
    {
        workspaces[id] = tools.create_workspace();
    }
}


void TilingWindowManagerPolicy::switch_workspace(int id)
{
    if (workspaces.find(id) == workspaces.end())
    {
        std::cerr << "[DEBUG] Creando workspace " << id << "\n";
        create_workspace(id);
    }

    std::cerr << "[DEBUG] Cambiando al workspace " << id << "\n";

    active_workspace = id;
    saveWorkspaceFile(id);

    // Ocultar ventanas del workspace actual
    tools.for_each_application([&](miral::ApplicationInfo& app_info)
    {
        for (auto const& window : app_info.windows())
        {
            if (persistent_windows.count(window))  // No ocultar panel
                continue;

            if (window_workspace_map[window] != active_workspace)
            {
                miral::WindowSpecification spec;
                spec.state() = mir_window_state_hidden;
                tools.modify_window(window, spec);
            }
        }
    });

    bool has_windows = false;
    tools.for_each_application([&](miral::ApplicationInfo& app_info)
    {
        for (auto const& window : app_info.windows())
        {
            if (persistent_windows.count(window))  // No modificar panel
                continue;

            if (window_workspace_map[window] == active_workspace)
            {
                has_windows = true;
                miral::WindowSpecification spec;
                spec.state() = mir_window_state_restored;
                tools.modify_window(window, spec);
            }
        }
    });

    update_tiles({tools.active_output()});
    
}

void TilingWindowManagerPolicy::saveWorkspaceFile(int id)
{
    // ✍️ Guardar en el archivo temporal
    std::ofstream file(WORKSPACE_FILE);
    if (file.is_open())
    {
        file << id << std::endl;
        file.close();
    }
    else
    {
        std::cerr << "[ERROR] No se pudo escribir en " << WORKSPACE_FILE << "\n";
    }
}

void TilingWindowManagerPolicy::toggle_fullscreen(miral::Window window)
{
    auto& window_info = tools.info_for(window);
    miral::WindowSpecification spec;

    if (window_info.state() == mir_window_state_fullscreen)
    {
        std::cerr << "[DEBUG] Saliendo de pantalla completa\n";
        spec.state() = mir_window_state_restored;
        tools.modify_window(window, spec);
        update_tiles({tools.active_output()});
    }
    else
    {
        std::cerr << "[DEBUG] Activando pantalla completa\n";

        // � Salir del modo fullscreen en cualquier otra ventana del workspace
        tools.for_each_application([&](miral::ApplicationInfo& app_info)
        {
            for (auto const& w : app_info.windows())
            {
                if (window_workspace_map[w] == active_workspace)
                {
                    auto& info = tools.info_for(w);
                    if (info.state() == mir_window_state_fullscreen)
                    {
                        miral::WindowSpecification restore_spec;
                        restore_spec.state() = mir_window_state_restored;
                        tools.modify_window(w, restore_spec);
                    }
                }
            }
        });

        // � Poner la ventana activa en pantalla completa
        spec.state() = mir_window_state_fullscreen;
        tools.modify_window(window, spec);
        update_tiles({tools.active_output()});
    }
}
