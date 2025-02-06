#include "compositor.h"
#include "window_manager.h"
#include "keybindings.h"
#include <miral/set_window_management_policy.h>
#include <miral/append_event_filter.h>

Compositor::Compositor(int argc, char const* argv[])
    : runner(argc, argv)
{
}

int Compositor::run()
{
    return runner.run_with({
        miral::set_window_management_policy<CustomWindowManager>(),
        external_client_launcher,
        miral::AppendEventFilter(KeyBindings::builtin_keybinds(runner, external_client_launcher))
    });
}
