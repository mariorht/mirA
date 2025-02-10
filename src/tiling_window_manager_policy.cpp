#include "tiling_window_manager_policy.h"
#include <miral/application_info.h>
#include <miral/window_info.h>
#include <miral/window_manager_tools.h>

#include <iostream> 
#include <cmath>
#include <fstream> 

using namespace miral;

TilingWindowManagerPolicy::TilingWindowManagerPolicy(miral::WindowManagerTools const& tools)
    : tools(tools)
{
    create_workspace(1);  // Default workspace 1
    saveWorkspaceFile(1);
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

    // If they are popup menus, they do not have a name, no need to handle them
    if(window_name == "")
        return;

    if (window_name == WORKSPACE_PROGRAM_NAME)  // Catch the panel
    {
        miral::WindowSpecification spec;
        spec.top_left() = {0, 0};
        spec.size() = {tools.active_output().size.width, HEIGHT_PANEL};
        spec.state() = mir_window_state_restored;
        spec.shell_chrome() = mir_shell_chrome_low; 
        tools.modify_window(window, spec);

        panel_window = window;  // Save panel for later reference
    }
    else if (window_name == WALLPAPER_PROGRAM_NAME)  // Catch the wallpaper
    {
        miral::WindowSpecification spec;
        spec.top_left() = {0, 0}; 
        spec.size() = {tools.active_output().size.width, tools.active_output().size.height};
        spec.state() = mir_window_state_restored;
        spec.shell_chrome() = mir_shell_chrome_low;
        tools.modify_window(window, spec);

        wallpaper_window = window;  // Save wallpaper for later reference
    }
    else
    {
        tools.add_tree_to_workspace(window, workspaces[active_workspace]);
    }

    dirty_tiles = true;

    if (window_info.can_be_active() && window != panel_window && window != wallpaper_window)  // No focus on panel or wallpaper
    {
        tools.select_active_window(window);
    }
}


void TilingWindowManagerPolicy::handle_modify_window(miral::WindowInfo& window_info, miral::WindowSpecification const& modifications)
{
    // If window is already hidden, do not modify it again
    // I'm not sure why handle_modify_window is called with the window already hidden
    if (window_info.state() == mir_window_state_hidden && modifications.state().value() == mir_window_state_restored)
        return;

    tools.modify_window(window_info, modifications);
}

void TilingWindowManagerPolicy::handle_request_move(miral::WindowInfo& window_info, MirInputEvent const* input_event)
{
    (void) window_info;
    (void) input_event;

    // Move windows is not supported
}

void TilingWindowManagerPolicy::handle_request_resize(miral::WindowInfo& window_info, MirInputEvent const* input_event, MirResizeEdge edge)
{
    (void) window_info;
    (void) input_event;
    (void) edge;

    // Resize windows is not supported
}

void TilingWindowManagerPolicy::handle_raise_window(miral::WindowInfo& window_info)
{
    if(window_info.window() == panel_window || window_info.window() == wallpaper_window)
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
        return false;
    }

    MirInputEventModifiers mods = mir_keyboard_event_modifiers(event);
    bool mod = (mods & mir_input_event_modifier_meta);
    bool ctrl = (mods & mir_input_event_modifier_ctrl_left);

    auto window = tools.active_window();

    // Worksapces
    if (mod)
    {
        int key = mir_keyboard_event_keysym(event);
      

        if (ctrl && key >= XKB_KEY_1 && key <= XKB_KEY_9) // Detectar Meta + Shift + Número
        {
            int workspace_id = key - XKB_KEY_1 + 1;
            if (window) // Evitar mover una ventana inexistente
                move_window_to_workspace(window, workspace_id);
            return true;
        }

        if (key >= XKB_KEY_1 && key <= XKB_KEY_9)
        {
            int workspace_id = key - XKB_KEY_1 + 1;
            switch_workspace(workspace_id);
            return true;
        }
    }

    // if not window active let the event pass
    if (!window)
    {
        return false; 
    }

    // Keyboards shortcuts for window management
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
            tools.focus_prev_within_application();
            return true;
        }
        break;
    case XKB_KEY_Right:
        if (mod)
        {
            tools.focus_next_within_application();
            return true;
        }
        break;

    case XKB_KEY_q:
    case XKB_KEY_Q:
        if (window == panel_window || window == wallpaper_window)
        {
            break;
        }
        if (mod)
        {
            tools.ask_client_to_close(window);
            return true;
        }
        break;

    default:
        return false;
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

    return false;
}



void TilingWindowManagerPolicy::handle_request_drag_and_drop(miral::WindowInfo& window_info)
{
    (void) window_info;
    // Drag and drop is not supported
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
        // If it doesn't have a name, it's a popup menu, do nothing with it
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
    if (window_info.window() == panel_window || window_info.window() == wallpaper_window)
    {
        return;
    }

    tools.for_each_workspace_containing(window_info.window(), [&](std::shared_ptr<Workspace> const& workspace)
    {
        tools.remove_tree_from_workspace(window_info.window(), workspace);
    });

    dirty_tiles = true;  // Tiling will be updated on the next frame
}


void TilingWindowManagerPolicy::advise_end()
{
    if (dirty_tiles)
    {
        update_tiles();
        dirty_tiles = false;
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


    // Hide all windows in the current workspace
    tools.for_each_window_in_workspace(workspaces[active_workspace], [&](Window const& window)
    {
        if (window == panel_window || window == wallpaper_window) // No hide the panel or the wallpaper
            return;

        miral::WindowSpecification spec;
        spec.state() = mir_window_state_hidden;
        tools.modify_window(window, spec);
    });

    active_workspace = id;
    saveWorkspaceFile(id);

    // Restore windows from the new workspace and select the first visible window
    miral::Window first_window;

    tools.for_each_window_in_workspace(workspaces[active_workspace], [&](Window const& window)
    {
        if (window == panel_window || window == wallpaper_window)  // No hide the panel or the wallpaper
            return;

        if (!first_window)
        {
            first_window = window;
        }

        miral::WindowSpecification spec;
        spec.state() = mir_window_state_restored;
        tools.modify_window(window, spec);
    });

    // Focus on the first window
    if (first_window)
        tools.select_active_window(first_window);

    dirty_tiles = true;
}


void TilingWindowManagerPolicy::saveWorkspaceFile(int id)
{
    // Save the workspace id to a file for the panel app
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
        spec.state() = mir_window_state_restored;
    else
    {
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
    dirty_tiles = true;
}


void TilingWindowManagerPolicy::move_window_to_workspace(miral::Window window, int workspace_id)
{
    if (!window || window == panel_window || window == wallpaper_window)
    {
        std::cerr << "[ERROR] Intento de mover una ventana inválida.\n";
        return;
    }

    if (workspaces.find(workspace_id) == workspaces.end())
        create_workspace(workspace_id);
    


    // Remove window from all workspaces
    tools.for_each_workspace_containing(window, [&](std::shared_ptr<Workspace> const& workspace)
    {
        tools.remove_tree_from_workspace(window, workspace);
    });

    // Add window to the new workspace
    tools.add_tree_to_workspace(window, workspaces[workspace_id]);

    // Hide the window in the current workspace
    miral::WindowSpecification spec;
    spec.state() = mir_window_state_hidden;
    tools.modify_window(window, spec);

    // If we are in the workspace we moved it to, restore it and give it focus
    if (workspace_id == active_workspace)
    {
        spec.state() = mir_window_state_restored;
        tools.modify_window(window, spec);
        tools.select_active_window(window);
    }

    dirty_tiles = true;
}
