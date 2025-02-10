cmake_minimum_required(VERSION 3.5)

### mirA ###
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
    src/tiling_window_manager_policy.cpp
    src/keybindings.cpp
)

target_include_directories(mirA PUBLIC src ${MIRAL_INCLUDE_DIRS})
target_link_libraries(mirA ${MIRAL_LDFLAGS} ${XKBCOMMON_LIBRARIES})


### panel ###
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(mirA_panel panel/panel_workspace.cpp)
target_include_directories(mirA_panel PRIVATE ${GTK3_INCLUDE_DIRS})
target_link_libraries(mirA_panel PRIVATE ${GTK3_LIBRARIES} pthread)
target_compile_options(mirA_panel PRIVATE ${GTK3_CFLAGS_OTHER})


### wallpaper ###
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(CAIRO REQUIRED cairo)

add_executable(mirA_wallpaper wallpaper/wallpaper.cpp)
target_include_directories(mirA_wallpaper PRIVATE ${GTK3_INCLUDE_DIRS}     ${CAIRO_INCLUDE_DIRS})
target_link_libraries(mirA_wallpaper PRIVATE ${GTK3_LIBRARIES} ${CAIRO_LIBRARIES} pthread)
target_compile_options(mirA_wallpaper PRIVATE ${GTK3_CFLAGS_OTHER} ${CAIRO_CFLAGS_OTHER})


# Install binaries on /usr/local/bin
install(TARGETS mirA_panel mirA_wallpaper mirA
    RUNTIME DESTINATION /usr/local/bin
)

# Assets
install(DIRECTORY assets/ DESTINATION /usr/share/mirA/assets
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# Uninstall
add_custom_target(uninstall
    COMMAND xargs rm < install_manifest.txt
    COMMAND rm -rf /usr/share/mirA/assets
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Desinstalando archivos y la carpeta assets..."
)



# Package .deb
# Package information
set(CPACK_PACKAGE_NAME "mirA")
set(CPACK_PACKAGE_VENDOR "Mario Roos Hoefgeest Toribio")
set(CPACK_PACKAGE_DESCRIPTION "Gestor de ventanas Mir con tiling")
set(CPACK_PACKAGE_VERSION "0.2")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/mariorht/mirA")
set(CPACK_PACKAGE_MAINTAINER "Mario Roos <marioroos2502@gmail.com>")
set(CPACK_PACKAGE_CONTACT "marioroos2502@gmail.com")

# Define package type
set(CPACK_GENERATOR "DEB")

# Package dependencies (excluding 'bomber')
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libmiral-dev, mir-graphics-drivers-desktop, xfce4-terminal, libgtk-3-dev, libcairo2-dev, libxkbcommon-dev")

# Bomber optional dependency
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "bomber")


# Package priority and architecture
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")

# Include CPack
include(CPack)



