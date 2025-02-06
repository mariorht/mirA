#include "compositor.h"

int main(int argc, char const* argv[])
{
    Compositor compositor(argc, argv);
    return compositor.run();
}
