#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <functional>
#include <vector>
#include <miral/runner.h>
#include <miral/configuration_option.h>
#include <miral/external_client.h>
#include <miral/set_window_management_policy.h>
#include <miral/append_event_filter.h> 

class Compositor
{
public:
    Compositor(int argc, char const* argv[]);
    int run();

private:
    miral::MirRunner runner;
    miral::ExternalClientLauncher external_client_launcher;
};

#endif // COMPOSITOR_H
