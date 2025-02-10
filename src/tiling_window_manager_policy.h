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

    void handle_request_drag_and_drop(miral::WindowInfo& window_info);
    void handle_request_move(miral::WindowInfo& window_info, MirInputEvent const* input_event) override;
    void handle_request_resize(miral::WindowInfo& window_info, MirInputEvent const* input_event, MirResizeEdge edge) override;

    auto confirm_inherited_move(miral::WindowInfo const& window_info, Displacement movement) -> Rectangle override;

    Rectangle confirm_placement_on_display(const miral::WindowInfo& window_info, MirWindowState new_state,
        Rectangle const& new_placement) override;

    void advise_end() override;
    void advise_delete_window(miral::WindowInfo const& window_info) override;

private:

    void update_tiles();

    miral::WindowManagerTools tools;

    bool dirty_tiles = false;  // Flag para marcar cuando necesitamos actualizar el tiling

    //Workspaces
    void create_workspace(int id);
    void switch_workspace(int id);
    void saveWorkspaceFile(int id);
    void toggle_fullscreen(miral::Window window);
    void move_window_to_workspace(miral::Window window, int workspace_id);


private:
    int active_workspace = 1;
    std::unordered_map<int, std::shared_ptr<miral::Workspace>> workspaces;
    miral::Window panel_window;
    miral::Window wallpaper_window;
    const std::string WORKSPACE_FILE = "/tmp/workspace_id";

};


#endif // TILING_WINDOW_MANAGER_H
