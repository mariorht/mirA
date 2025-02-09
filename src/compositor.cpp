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

            while (iss >> std::quoted(arg))  // Extraer los argumentos correctamente
            {
                command_args.push_back(arg);
            }

            if (!command_args.empty())
            {
                std::cerr << "[DEBUG] Ejecutando aplicación de inicio: " << command_args[0] << "\n";
                for (size_t i = 1; i < command_args.size(); ++i)
                {
                    std::cerr << "[DEBUG]  → Argumento: " << command_args[i] << "\n";
                }
                
                external_client_launcher.launch(command_args);
            }
            else
            {
                std::cerr << "[ERROR] No se pudo procesar la aplicación de inicio: " << app << "\n";
            }
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

