#include "keybindings.h"

std::function<bool(MirEvent const*)> KeyBindings::builtin_keybinds(miral::MirRunner& runner, miral::ExternalClientLauncher& external_client_launcher)
{
    return [&](MirEvent const* event) -> bool
    {
        using namespace miral::toolkit;  // <-- Importamos el espacio de nombres correcto

        if (mir_event_get_type(event) != mir_event_type_input)
            return false;

        MirInputEvent const* input_event = mir_event_get_input_event(event);
        if (mir_input_event_get_type(input_event) != mir_input_event_type_key)
            return false;

        MirKeyboardEvent const* kev = mir_input_event_get_keyboard_event(input_event);
        if (mir_keyboard_event_action(kev) != mir_keyboard_action_down)
            return false;

        MirInputEventModifiers mods = mir_keyboard_event_modifiers(kev);
        if (!(mods && mir_input_event_modifier_meta))
            return false;

        switch (mir_keyboard_event_keysym(kev))
        {
        case XKB_KEY_BackSpace:
            runner.stop();
            return true;

        case XKB_KEY_t:
        case XKB_KEY_T:
            external_client_launcher.launch(std::string("xfce4-terminal"));
            return true;

        case XKB_KEY_g:
        case XKB_KEY_G:
            external_client_launcher.launch(std::string("gedit"));
            return true;

        default:
            return false;
        }

        return false;
    };
}
