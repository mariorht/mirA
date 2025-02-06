cmake_minimum_required(VERSION 3.5)

project(mirA)

set(CMAKE_CXX_STANDARD 20)

include(FindPkgConfig)
pkg_check_modules(MIRAL miral REQUIRED)
pkg_check_modules(XKBCOMMON xkbcommon REQUIRED)

add_executable(mirA src/main.cpp)

target_include_directories(mirA PUBLIC SYSTEM ${MIRAL_INCLUDE_DIRS})
target_link_libraries(     mirA               ${MIRAL_LDFLAGS})
target_link_libraries(     mirA               ${XKBCOMMON_LIBRARIES})

