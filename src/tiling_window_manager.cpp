#include "tiling_window_manager.h"
#include <miral/application_info.h>
#include <miral/window_info.h>
#include <miral/window_manager_tools.h>

#include <iostream> 
#include <cmath>

using namespace miral;

TilingWindowManagerPolicy::TilingWindowManagerPolicy(miral::WindowManagerTools const& tools)
    : tools(tools)
{
    create_workspace(1);  // Inicializamos con un workspace por defecto
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

// void TilingWindowManagerPolicy::handle_window_ready(miral::WindowInfo& window_info)
// {
//     update_tiles({tools.active_output()});

//     // Asegurar que la ventana recibe el foco
//     if (window_info.can_be_active())
//     {
//         tools.select_active_window(window_info.window());
//     }
// }

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
    bool alt = mods & mir_input_event_modifier_alt;
    bool ctrl = mods & mir_input_event_modifier_ctrl;
    bool shift = mods & mir_input_event_modifier_shift;
    bool super = mods & mir_input_event_modifier_meta;


    if (super && ctrl)  // ⊞ Win + Ctrl + Número para cambiar de workspace
    {
        switch (mir_keyboard_event_keysym(event))
        {
        case XKB_KEY_1: switch_workspace(1); return true;
        case XKB_KEY_2: switch_workspace(2); return true;
        case XKB_KEY_3: switch_workspace(3); return true;
        case XKB_KEY_4: switch_workspace(4); return true;
        case XKB_KEY_5: switch_workspace(5); return true;
        }
    }

    if (super && shift)  // ⊞ Win + Shift + Número para mover ventana al workspace
    {
        auto window = tools.active_window();
        if (!window) return false;

        auto& window_info = tools.info_for(window);

        switch (mir_keyboard_event_keysym(event))
        {
        case XKB_KEY_1: move_window_to_workspace(window_info, 1); return true;
        case XKB_KEY_2: move_window_to_workspace(window_info, 2); return true;
        case XKB_KEY_3: move_window_to_workspace(window_info, 3); return true;
        case XKB_KEY_4: move_window_to_workspace(window_info, 4); return true;
        case XKB_KEY_5: move_window_to_workspace(window_info, 5); return true;
        }
    }

    switch (mir_keyboard_event_keysym(event))
    {
    // 🔄 Mover el foco (Alt + Tab → Siguiente | Alt + Shift + Tab → Anterior)
    case XKB_KEY_Left:
        if (alt && ctrl)
        {
            std::cerr << "[DEBUG] Moviendo foco a la ventana anterior\n";
            tools.focus_prev_within_application();
        }
        return true;
    case XKB_KEY_Right:
        if (alt && ctrl)
        {
            std::cerr << "[DEBUG] Moviendo foco a la siguiente ventana\n";
            tools.focus_next_within_application();
        }
        return true;

    // 🔴 Cerrar Ventana Activa (Alt + Q)
    case XKB_KEY_q:
    case XKB_KEY_Q:
        if (alt)
        {
            auto window = tools.active_window();
            if (window)
            {
                std::cerr << "[DEBUG] Cerrando ventana activa\n";
                tools.ask_client_to_close(window);
            }
            return true;
        }
        break;

    // 🔼🔽 Mover Ventanas en el Stack (Alt + Flechas)
    // case XKB_KEY_Up:
    //     if (alt)
    //     {
    //         auto window = tools.active_window();
    //         if (window)
    //         {
    //             std::cerr << "[DEBUG] Moviendo ventana arriba en el orden de apilamiento\n";
    //             tools.raise_tree(window);
    //         }
    //         return true;
    //     }
    //     break;

    // case XKB_KEY_Down:
    //     if (alt)
    //     {
    //         auto window = tools.active_window();
    //         if (window)
    //         {
    //             std::cerr << "[DEBUG] Bajando ventana en el orden de apilamiento (simulado)\n";
    //             // No hay una función directa para bajar una ventana, pero podríamos hacer swap con otra ventana
    //         }
    //         return true;
    //     }
    //     break;



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
        return true;
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
    std::vector<miral::Window> tiled_windows;

    // Obtener todas las ventanas activas
    tools.for_each_application([&](miral::ApplicationInfo& app_info)
    {
        for (auto const& window : app_info.windows())
        {
            tiled_windows.push_back(window);
        }
    });

    if (tiled_windows.empty()) return;

    int num_windows = tiled_windows.size();

    std::cerr << "[DEBUG] Recalculando mosaico, número de ventanas activas: " << num_windows << "\n";


    int screen_width = outputs[0].size.width.as_int();
    int screen_height = outputs[0].size.height.as_int();

    // Calcular el número de columnas y filas para la cuadrícula
    int columns = std::ceil(std::sqrt(num_windows));  // Cantidad de columnas basada en raíz cuadrada
    int rows = std::ceil(static_cast<float>(num_windows) / columns);  // Número de filas necesarias

    int window_width = screen_width / columns;
    int window_height = screen_height / rows;

    for (int i = 0; i < num_windows; ++i)
    {
        int col = i % columns;  // Determinar en qué columna se encuentra la ventana
        int row = i / columns;  // Determinar en qué fila se encuentra la ventana

        miral::WindowSpecification spec;
        spec.top_left() = {col * window_width, row * window_height};
        spec.size() = {window_width, window_height};

        tools.modify_window(tiled_windows[i], spec);
    }
}



// void TilingWindowManagerPolicy::advise_delete_window(miral::WindowInfo const& window_info)
// {
//     std::cerr << "[DEBUG] Ventana eliminada: " << window_info.name() << ". Esperando a que desaparezca...\n";

//     dirty_tiles = true;  // Marcamos que necesitamos actualizar el tiling
// }


// void TilingWindowManagerPolicy::advise_end()
// {
//     if (dirty_tiles)
//     {
//         std::cerr << "[DEBUG] Recalculando mosaico al final del ciclo de eventos.\n";
//         update_tiles({tools.active_output()});
//         dirty_tiles = false;  // Restablecemos el flag
//     }
// }



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
        create_workspace(id);

    std::cerr << "[DEBUG] Cambiando al workspace " << id << "\n";

    // Ocultar ventanas del workspace actual
    tools.for_each_application([&](miral::ApplicationInfo& app_info)
    {
        for (auto const& window : app_info.windows())
        {
            if (window_workspace_map[window] == active_workspace)
            {
                miral::WindowSpecification spec;
                spec.state() = mir_window_state_hidden;
                tools.modify_window(window, spec);
            }
        }
    });

    active_workspace = id;

    // Restaurar ventanas del nuevo workspace
    tools.for_each_application([&](miral::ApplicationInfo& app_info)
    {
        for (auto const& window : app_info.windows())
        {
            if (window_workspace_map[window] == active_workspace)
            {
                miral::WindowSpecification spec;
                spec.state() = mir_window_state_restored;
                tools.modify_window(window, spec);
            }
        }
    });
}

void TilingWindowManagerPolicy::move_window_to_workspace(miral::WindowInfo& window_info, int workspace_id)
{
    auto window = window_info.window();
    std::cerr << "[DEBUG] Moviendo ventana al workspace " << workspace_id << "\n";

    // Asegurar que el workspace existe
    if (workspaces.find(workspace_id) == workspaces.end())
        create_workspace(workspace_id);

    // Ocultar la ventana en el workspace actual
    miral::WindowSpecification spec;
    spec.state() = mir_window_state_hidden;
    tools.modify_window(window, spec);

    // Asignar ventana al nuevo workspace
    window_workspace_map[window] = workspace_id;
    tools.add_tree_to_workspace(window, workspaces[workspace_id]);

    // Si estamos en el workspace al que movimos la ventana, restaurarla
    if (workspace_id == active_workspace)
    {
        spec.state() = mir_window_state_restored;
        tools.modify_window(window, spec);
    }
}

void TilingWindowManagerPolicy::handle_window_ready(miral::WindowInfo& window_info)
{
    auto window = window_info.window();
    window_workspace_map[window] = active_workspace;
    tools.add_tree_to_workspace(window, workspaces[active_workspace]);
}


void TilingWindowManagerPolicy::advise_delete_window(miral::WindowInfo const& window_info)
{
    window_workspace_map.erase(window_info.window());
}


void TilingWindowManagerPolicy::advise_end()
{
    // No es necesario actualizar mosaico, solo manejamos workspaces
}