#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <miral/minimal_window_manager.h>

class CustomWindowManager : public miral::MinimalWindowManager
{
public:
    using MinimalWindowManager::MinimalWindowManager;
};

#endif // WINDOW_MANAGER_H
