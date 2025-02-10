# mirA - Tiling Window Manager for Mir

**mirA** is a lightweight tiling window manager built for the **Mir** display server. It provides an efficient window management experience with keyboard-driven controls, workspaces, and automatic tiling.

## 🚀 Features
- **Tiling Window Management**: Windows are automatically arranged for better usability.
- **Workspaces**: Easily switch and move windows between virtual desktops.
- **Keyboard Shortcuts**: Navigate, manage, and launch applications quickly.
- **Persistent Panel & Wallpaper**: Custom panel and wallpaper system integrated.
- **Debian Package Support**: Easily install and manage with `.deb` packaging.

## 🛠 Dependencies
To build and run mirA, you need:
- **MirAL** (`miral`)
- **GTK+3** (`gtk+-3.0`)
- **Cairo** (`cairo`)
- **XKB Common** (`xkbcommon`)

Install them on Ubuntu/Debian:
```sh
sudo apt install miral-dev libgtk-3-dev libcairo2-dev libxkbcommon-dev
```

## 🔧 Building mirA
Clone the repository and compile the project:
```sh
git clone https://github.com/mariorht/mirA.git
cd mirA
mkdir build && cd build
cmake ..
make -j$(nproc)
make install
```

## 📦 Generating the `.deb` Package
To generate a **Debian package** for easy installation:
```sh
cpack -G DEB
```
Then install it:
```sh
sudo dpkg -i mirA-1.0.0-Linux.deb
```

## 🖥️ Running mirA
To launch mirA with the **panel and wallpaper**:
```sh
mirA --startup-app mirA_panel --startup-app "mirA_wallpaper /usr/share/mirA/assets/background.png"
```

## ⌨️ Keyboard Shortcuts
| Shortcut | Action |
|----------|--------|
| **Meta + 1-9** | Switch workspace |
| **Meta + Ctrl + 1-9** | Move window to workspace |
| **Meta + Left/Right** | Switch focus between windows |
| **Meta + F** | Toggle fullscreen |
| **Meta + Q** | Close active window |
| **Meta + T** | Launch terminal (xfce4-terminal) |
| **Meta + B** | Launch bomber game |
| **Meta + Backspace** | Exit mirA |

## 📂 File Structure
```
mirA/
├── assets/            # Default assets (wallpapers, icons, etc.)
├── panel/             # Panel workspace application
├── wallpaper/         # Wallpaper rendering application
├── src/               # Main compositor and window manager logic
├── CMakeLists.txt     # Build configuration
└── README.md          # This file
```

## 📜 Uninstalling mirA
If installed via `.deb`, remove it with:
```sh
sudo dpkg -r mirA
```
If installed manually:
```sh
sudo make uninstall
```

