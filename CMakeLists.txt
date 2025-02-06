cmake_minimum_required(VERSION 3.5)

project(mirA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror=return-type")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror=return-type")

include(FindPkgConfig)
pkg_check_modules(MIRAL miral REQUIRED)
pkg_check_modules(XKBCOMMON xkbcommon REQUIRED)

add_executable(mirA 
    src/main.cpp
    src/compositor.cpp
    src/tiling_window_manager.cpp
    src/keybindings.cpp
)

target_include_directories(mirA PUBLIC src ${MIRAL_INCLUDE_DIRS})
target_link_libraries(mirA ${MIRAL_LDFLAGS} ${XKBCOMMON_LIBRARIES})

