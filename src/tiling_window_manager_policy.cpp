#include "tiling_window_manager_policy.h"
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
    std::cerr << "[DEBUG] Ventana lista: " << window_info.name() << "\n";

    auto window = window_info.window();
    std::string window_name = window_info.name();

    //Si son menús emerjentes no tienen nombre, no hace falta hacer nada con ellos
    if(window_name == "")
        return;

    if (window_name == "Workspace Panel")  // Detectamos el panel
    {
        miral::WindowSpecification spec;
        spec.top_left() = {0, 0};  // Arriba, esquina izquierda
        spec.size() = {tools.active_output().size.width, 30};  // Todo el ancho, altura de 30px
        spec.state() = mir_window_state_restored;
        spec.shell_chrome() = mir_shell_chrome_low; 
        tools.modify_window(window, spec);

        panel_window = window;  // Guardamos la referencia del panel
    }
    else
    {
        tools.add_tree_to_workspace(window, workspaces[active_workspace]);
    }

    update_tiles();

    if (window_info.can_be_active() && window != panel_window)  // ✅ Evita que el panel tome foco
    {
        tools.select_active_window(window);
    }
}


void TilingWindowManagerPolicy::handle_modify_window(miral::WindowInfo& window_info, miral::WindowSpecification const& modifications)
{
    std::cerr << "[DEBUG] Modificando ventana: " << window_info.name() 
              << " | Estado actual: " << window_info.state() << "\n";

    // Si la ventana ya está oculta, no modificarla nuevamente
    if (window_info.state() == mir_window_state_hidden && modifications.state().is_set() 
        && modifications.state().value() == mir_window_state_restored)
    {
        std::cerr << "[DEBUG] Evitando restaurar ventana oculta en cambio de workspace: " 
                  << window_info.name() << "\n";
        return;
    }

    tools.modify_window(window_info, modifications);
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
    if(window_info.window() == panel_window)
    {
        return;
    }
    tools.select_active_window(window_info.window());
}

bool TilingWindowManagerPolicy::handle_keyboard_event(const MirKeyboardEvent* event)
{
    using namespace miral::toolkit;

    if (mir_keyboard_event_action(event) != mir_keyboard_action_down)
    {
        return false; // Ignorar eventos de teclado que no sean pulsaciones
    }

    MirInputEventModifiers mods = mir_keyboard_event_modifiers(event);
    bool mod = (mods & mir_input_event_modifier_meta);

    auto window = tools.active_window(); // Puede ser nulo si no hay ventanas

    // � Permitir cambio de workspace aunque no haya ventanas activas
    if (mod)
    {
        int key = mir_keyboard_event_keysym(event);

        if (key >= XKB_KEY_1 && key <= XKB_KEY_9)
        {
            int workspace_id = key - XKB_KEY_1 + 1;
            switch_workspace(workspace_id);
            return true;
        }
    }

    // � Manejar atajos de teclado aunque no haya ventana activa
    if (!window)
    {
        return false; // Sin ventana activa, dejamos pasar eventos normales
    }

    // � Resto de atajos de teclado solo si hay ventana activa
    switch (mir_keyboard_event_keysym(event))
    {
    case XKB_KEY_f:
        if (mod)
        {
            toggle_fullscreen(window);
            return true;
        }
        break;

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

    case XKB_KEY_q:
    case XKB_KEY_Q:
        if (window == panel_window)
        {
            break;
        }
        if (mod)
        {
            std::cerr << "[DEBUG] Cerrando ventana activa\n";
            tools.ask_client_to_close(window);
            return true;
        }
        break;

    default:
        return false; // Dejamos pasar eventos normales
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

    miral::Window detected_window =tools.window_at(cursor);
    if (detected_window)
        tools.select_active_window(detected_window);

    return false; // Permitir que otros manejadores de eventos procesen el evento
}



void TilingWindowManagerPolicy::handle_request_drag_and_drop(miral::WindowInfo& window_info)
{
    (void) window_info;
    // No soportamos drag & drop en tiling
}

void TilingWindowManagerPolicy::update_tiles()
{
    std::vector<miral::Window> tiled_windows;
    miral::Window fullscreen_window;

    int panel_height = 30;
    int screen_width = tools.active_output().size.width.as_int();
    int screen_height = tools.active_output().size.height.as_int() - panel_height;

    tools.for_each_window_in_workspace(workspaces[active_workspace], [&](Window const& window)
    {
        // Si no tiene nombre es un menú emergente, no hacer nada con él
        if(tools.info_for(window).name() == "")
            return;

        auto& info = tools.info_for(window);

        if (info.state() == mir_window_state_fullscreen)
        {
            fullscreen_window = window;
        }
        else
        {
            tiled_windows.push_back(window);
            std::cerr << "[DEBUG] Ventana en mosaico: " << info.name() << "\n";
        }
    });

    if (fullscreen_window)
    {
        miral::WindowSpecification spec;
        spec.top_left() = {0, panel_height};
        spec.size() = {screen_width, screen_height};
        spec.state() = mir_window_state_fullscreen;
        tools.modify_window(fullscreen_window, spec);
        return;
    }

    if (tiled_windows.empty()) 
        return;

    int num_windows = tiled_windows.size();
    int columns = std::ceil(std::sqrt(num_windows));
    int rows = std::ceil(static_cast<float>(num_windows) / columns);

    int window_width = screen_width / columns;
    int window_height = screen_height / rows;

    for (int i = 0; i < num_windows; ++i)
    {
        miral::WindowSpecification spec;
        spec.top_left() = { (i % columns) * window_width, panel_height + (i / columns) * window_height };
        spec.size() = { window_width, window_height };
        spec.state() = mir_window_state_restored;

        tools.modify_window(tiled_windows[i], spec);
    }
}

void TilingWindowManagerPolicy::advise_delete_window(miral::WindowInfo const& window_info)
{
    if(window_info.window() == panel_window)
    {
        return;
    }

    std::cerr << "[DEBUG] Ventana eliminada: " << window_info.name() << ". Esperando a que desaparezca...\n";
    
    tools.for_each_workspace_containing(window_info.window(), [&](std::shared_ptr<Workspace> const& workspace)
    {
        tools.remove_tree_from_workspace(window_info.window(), workspace);
    });

    dirty_tiles = true;  // Marcamos que necesitamos actualizar el tiling
}


void TilingWindowManagerPolicy::advise_end()
{
    if (dirty_tiles)
    {
        std::cerr << "[DEBUG] Recalculando mosaico al final del ciclo de eventos.\n";
        update_tiles();
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
        create_workspace(id);
    }

    std::cerr << "[DEBUG] Cambiando al workspace " << id << "\n";

    // Ocultar todas las ventanas del workspace actual
    tools.for_each_window_in_workspace(workspaces[active_workspace], [&](Window const& window)
    {
        if (window == panel_window)  // No ocultar el panel
            return;
        
        miral::WindowSpecification spec;
        spec.state() = mir_window_state_hidden;
        tools.modify_window(window, spec);
        std::cerr << "[DEBUG] Ocultando ventana " << tools.info_for(window).name() << "\n";
        
    });

    active_workspace = id;
    saveWorkspaceFile(id);

    // Mostrar las ventanas del nuevo workspace
    tools.for_each_window_in_workspace(workspaces[active_workspace], [&](Window const& window)
    {
        if (window == panel_window)  // No ocultar el panel
            return;
        
        miral::WindowSpecification spec;
        spec.state() = mir_window_state_restored;
        tools.modify_window(window, spec);
        std::cerr << "[DEBUG] Mostrando ventana " << tools.info_for(window).name() << "\n";
        
    });


    update_tiles();

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
        std::cerr << "[DEBUG] Exiting fullscreen mode\n";
        spec.state() = mir_window_state_restored;
    }
    else
    {
        std::cerr << "[DEBUG] Entering fullscreen mode\n";

        tools.for_each_window_in_workspace(workspaces[active_workspace], [&](Window const& w)
        {
            if (w != window)
            {
                miral::WindowSpecification hide_spec;
                hide_spec.state() = mir_window_state_hidden;
                tools.modify_window(w, hide_spec);
            }
        });

        spec.state() = mir_window_state_fullscreen;
    }

    tools.modify_window(window, spec);
    update_tiles();
}
