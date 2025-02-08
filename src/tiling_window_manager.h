#ifndef TILING_WINDOW_MANAGER_H
#define TILING_WINDOW_MANAGER_H

#include <miral/application.h>
#include <miral/window_management_policy.h>
#include <miral/window_manager_tools.h>
#include <miral/internal_client.h>
#include <miral/toolkit_event.h>
#include <mir/geometry/displacement.h>

#include <vector>
#include <map>
#include <set>

using namespace mir::geometry;

class TilingWindowManagerPolicy : public miral::WindowManagementPolicy
{
public:
    explicit TilingWindowManagerPolicy(miral::WindowManagerTools const& tools);

    auto place_new_window(
        miral::ApplicationInfo const& app_info,
        miral::WindowSpecification const& request_parameters) -> miral::WindowSpecification override;

    void handle_window_ready(miral::WindowInfo& window_info) override;
    void handle_modify_window(miral::WindowInfo& window_info, miral::WindowSpecification const& modifications) override;
    bool handle_keyboard_event(MirKeyboardEvent const* event) override;
    bool handle_touch_event(MirTouchEvent const* event) override;
    bool handle_pointer_event(MirPointerEvent const* event) override;
    void handle_raise_window(miral::WindowInfo& window_info) override;

    void handle_request_drag_and_drop(miral::WindowInfo& window_info) override;
    void handle_request_move(miral::WindowInfo& window_info, MirInputEvent const* input_event) override;
    void handle_request_resize(miral::WindowInfo& window_info, MirInputEvent const* input_event, MirResizeEdge edge) override;

    auto confirm_inherited_move(miral::WindowInfo const& window_info, Displacement movement) -> Rectangle override;

    Rectangle confirm_placement_on_display(const miral::WindowInfo& window_info, MirWindowState new_state,
        Rectangle const& new_placement) override;

    void advise_end() override;
    void advise_delete_window(miral::WindowInfo const& window_info) override;

private:
    static const int modifier_mask =
        mir_input_event_modifier_alt |
        mir_input_event_modifier_shift |
        mir_input_event_modifier_sym |
        mir_input_event_modifier_ctrl |
        mir_input_event_modifier_meta;

    void click(Point cursor);
    void resize(Point cursor);
    void drag(Point cursor);
    void toggle(MirWindowState state);

    miral::Application application_under(Point position);

    void update_tiles(std::vector<Rectangle> const& outputs);
    void update_surfaces(miral::ApplicationInfo& info, Rectangle const& old_tile, Rectangle const& new_tile);

    miral::WindowManagerTools tools;

    bool dirty_tiles = false;  // Flag para marcar cuando necesitamos actualizar el tiling

    //Workspaces
    void create_workspace(int id);
    void switch_workspace(int id);
    void saveWorkspaceFile(int id);
    void toggle_fullscreen(miral::Window window);


private:
    int active_workspace = 1;
    std::map<int, std::shared_ptr<miral::Workspace>> workspaces;
    std::set<miral::Window> persistent_windows;
    miral::Window panel_window;
    const std::string WORKSPACE_FILE = "/tmp/workspace_id";

};


#endif // TILING_WINDOW_MANAGER_H
