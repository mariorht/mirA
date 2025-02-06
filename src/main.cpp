#include <vector>  
#include <functional> 

#include <miral/runner.h>
#include <miral/configuration_option.h>
#include <miral/external_client.h>
#include <miral/minimal_window_manager.h>
#include <miral/set_window_management_policy.h>



using namespace miral;

int main(int argc, char const* argv[])
{
    MirRunner runner{argc, argv};
   
    miral::ExternalClientLauncher external_client_launcher;

    // Convertir la lambda a std::function<>
    std::function<void(std::vector<std::string> const&)> run_startup_apps =
        [&](std::vector<std::string> const& apps)
        {
            for (auto const& app : apps)
            {
                external_client_launcher.launch(std::vector<std::string>{app});
            }
        };

    return runner.run_with(
        {
            set_window_management_policy<MinimalWindowManager>(),
            external_client_launcher,
            miral::ConfigurationOption{run_startup_apps, "startup-app", "App to run at startup (can be specified multiple times)"},
        });
}
