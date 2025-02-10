#include "compositor.h"
#include "tiling_window_manager_policy.h"
#include "keybindings.h"
#include <miral/set_window_management_policy.h>
#include <miral/append_event_filter.h>
#include <iostream>
#include <sstream>
#include <iomanip>

Compositor::Compositor(int argc, char const* argv[])
    : runner(argc, argv)
{
}

int Compositor::run()
{
    std::function<void(std::vector<std::string> const&)> run_startup_apps =
    [&](std::vector<std::string> const& apps)
    {
        for (auto const& app : apps)
        {
            std::vector<std::string> command_args;
            std::istringstream iss(app);
            std::string arg;

            while (iss >> std::quoted(arg))
            {
                command_args.push_back(arg);
            }

            if (!command_args.empty())
                external_client_launcher.launch(command_args);
            else
                std::cerr << "[ERROR] No se pudo procesar la aplicación de inicio: " << app << "\n";
        }
    };

    return runner.run_with(
        {
           miral::set_window_management_policy<TilingWindowManagerPolicy>(),
           external_client_launcher,
           miral::AppendEventFilter(KeyBindings::builtin_keybinds(runner, external_client_launcher)),
           miral::ConfigurationOption{run_startup_apps, "startup-app", "App to run at startup (can be specified multiple times)"}
        });


}

