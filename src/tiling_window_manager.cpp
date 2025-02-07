#include "tiling_window_manager.h"
#include <miral/application_info.h>
#include <miral/window_info.h>
#include <miral/window_manager_tools.h>

using namespace miral;

TilingWindowManagerPolicy::TilingWindowManagerPolicy(miral::WindowManagerTools const& tools)
    : tools(tools)
{
}

auto TilingWindowManagerPolicy::place_new_window(
    miral::ApplicationInfo const& app_info,
    miral::WindowSpecification const& request_parameters) -> miral::WindowSpecification
{
    return request_parameters;
}

auto TilingWindowManagerPolicy::confirm_placement_on_display(
    miral::WindowInfo const& window_info, MirWindowState new_state, Rectangle const& new_placement) -> Rectangle
{
    return new_placement;
}

auto TilingWindowManagerPolicy::confirm_inherited_move(
    miral::WindowInfo const& window_info, Displacement movement) -> Rectangle
{
    return {window_info.window().top_left() + movement, window_info.window().size()};
}

void TilingWindowManagerPolicy::handle_window_ready(miral::WindowInfo& window_info)
{
    tiled_windows.push_back(window_info.window());
    update_tiles({tools.active_output()});

    // Asegurar que la ventana recibe el foco
    if (window_info.can_be_active())
    {
        tools.select_active_window(window_info.window());
    }
}

void TilingWindowManagerPolicy::handle_modify_window(miral::WindowInfo& window_info, miral::WindowSpecification const& modifications)
{
    update_tiles({tools.active_output()});
}

void TilingWindowManagerPolicy::handle_request_move(miral::WindowInfo& window_info, MirInputEvent const* input_event)
{
    // No permitimos mover ventanas en tiling
}

void TilingWindowManagerPolicy::handle_request_resize(miral::WindowInfo& window_info, MirInputEvent const* input_event, MirResizeEdge edge)
{
    // No permitimos redimensionar en tiling
}

void TilingWindowManagerPolicy::handle_raise_window(miral::WindowInfo& window_info)
{
    tools.select_active_window(window_info.window());
}

bool TilingWindowManagerPolicy::handle_keyboard_event(const MirKeyboardEvent* event)
{
    return false;
}

bool TilingWindowManagerPolicy::handle_touch_event(const MirTouchEvent* event)
{
    return false;
}

bool TilingWindowManagerPolicy::handle_pointer_event(const MirPointerEvent* event)
{
    return false;
}

void TilingWindowManagerPolicy::handle_request_drag_and_drop(miral::WindowInfo& window_info)
{
    // No soportamos drag & drop en tiling
}

void TilingWindowManagerPolicy::update_tiles(std::vector<Rectangle> const& outputs)
{
    if (tiled_windows.empty()) return;

    int num_windows = tiled_windows.size();
    int screen_width = outputs[0].size.width.as_int();
    int screen_height = outputs[0].size.height.as_int();
    int window_width = screen_width / num_windows;
    int window_height = screen_height;

    for (int i = 0; i < num_windows; ++i)
    {
        miral::WindowSpecification spec;
        spec.top_left() = {i * window_width, 0};
        spec.size() = {window_width, window_height};

        tools.modify_window(tiled_windows[i], spec);
    }
}
