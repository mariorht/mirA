#ifndef KEYBINDINGS_H
#define KEYBINDINGS_H

#include <functional>
#include <miral/toolkit_event.h>
#include <miral/runner.h>
#include <miral/external_client.h>

class KeyBindings
{
public:
    static std::function<bool(MirEvent const*)> builtin_keybinds(miral::MirRunner& runner, miral::ExternalClientLauncher& external_client_launcher);
};

#endif // KEYBINDINGS_H
