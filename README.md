## **mirA: A Lightweight Tiling Window Manager for Mir**

mirA is a lightweight **tiling window manager** built on top of **Mir**. It provides efficient window management with **workspaces, keyboard-driven controls, and auto-tiling**. The goal of mirA is to offer a structured and intuitive window management experience while keeping the implementation simple.

You can see a video demonstration on [mirA_demo.mp4](mirA_demo.mp4).

### **Design Decisions**

##### **1. Grid-Based Tiling Layout**
mirA organizes windows in a **grid pattern** rather than dynamic stacking or floating layouts. This choice ensures that all open windows are neatly arranged without overlapping.

##### **2. Workspace Support**
mirA implements **workspaces**, allowing users to group applications into separate virtual desktops.
- Users can **switch between workspaces** using keyboard shortcuts.
- Windows can be **moved across workspaces**, allowing dynamic organization based on user needs.

##### **3. Fullscreen Support**
MirA includes **fullscreen mode**. When a window is set to fullscreen:
- All other windows are hidden.
- The active window occupies the entire screen.
- Exiting fullscreen restores the previous tiling layout.

##### **4. Minimalist Approach to System UI**
To simplify configuration and avoid complexity, mirA includes two **special-purpose applications**:
- **A panel application:** Acts as a lightweight system bar for essential functions.
- **A wallpaper application:** Manages the desktop background.

##### **5. No Drag & Drop or Window Resizing**
mirA **does not support** manual dragging or resizing of windows. Instead:
- Windows are arranged according to the **grid layout**.
- Users can move windows between workspaces, but manual adjustments to size and position are not required.

---

## **Future Improvements**
- [ ] **Better layout management**  
  _Improve how windows are tiled dynamically based on available space._
- [ ] **Floating window support**  
  _Allow certain windows to be free-floating outside the tiling layout._
- [ ] **Stack view support**  
  _Enable a mode where windows stack within a container._
- [ ] **Reorder windows**  
  _Allow moving windows within the same workspace._
- [ ] **Resize windows**  
  _Allow resize windows._
- [ ] **Support for 3rd party panels and wallpapers**  
  _Support applications like waybar or feh that implements panel bars and wallpapers_


---

## **Dependencies**
To build and run mirA, you need:
- **MirAL** (`miral`).
- **XKB Common** (`xkbcommon`).
- **GTK+3** (`gtk+-3.0`) for panel and wallpaper.
- **Cairo** (`cairo`) for wallpaper.

Install them on Ubuntu/Debian:
```sh
sudo add-apt-repository ppa:mir-team/release
sudo apt update
sudo apt install libmiral-dev mir-graphics-drivers-desktop
sudo apt install build-essential cmake pkg-config
sudo apt install libgtk-3-dev libcairo2-dev libxkbcommon-dev 
# Optional
sudo apt install bomber
```

## **Building mirA**
Clone the repository, compile and install the project:
```sh
git clone https://github.com/mariorht/mirA.git
cd mirA
mkdir build && cd build
cmake ..
make -j$(nproc)
make install
```

### **Running mirA**
Once installed, you can launch **mirA** with the wallpaper and panel:  
```sh
mirA --startup-app "mirA_panel" --startup-app "mirA_wallpaper /usr/share/mirA/assets/background.png"
```


## **Generating the `.deb` Package**
Youc can generate a **Debian package** after compiling:
```sh
cpack -G DEB
```
Then install it:
```sh
sudo apt install ./mirA-*.deb
```

---

## **Install from Releases**  

You can download the latest stable version of **mirA** from the releases page:  

👉 **[mirA Releases](https://github.com/mariorht/mirA/releases)**  

### **📦 Installing the `.deb` Package**  
1️⃣ **Download the latest `.deb` file** from the releases page.  
2️⃣ **Install it using `dpkg`**:  
   ```sh
   sudo apt install ./mirA-*.deb
   ```

## Uninstalling mirA
If installed via `.deb`, remove it with:
```sh
sudo apt remove mirA
```
If installed manually:
```sh
sudo make uninstall
```


---

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


---

## **Resources for building a compositor**
- [Getting Started with Mir](https://canonical-mir.readthedocs-hosted.com/stable/tutorial/getting-started-with-mir/)  
- [Writing Your First Wayland Compositor](https://canonical-mir.readthedocs-hosted.com/stable/tutorial/write-your-first-wayland-compositor/)  
- [Handling Keyboard Input](https://canonical-mir.readthedocs-hosted.com/stable/how-to/how-to-handle-keyboard-input/)  
- [Mir API Reference](https://canonical-mir.readthedocs-hosted.com/stable/api/library_root/#)  
- [Introducing the MirAL API](https://canonical-mir.readthedocs-hosted.com/stable/reference/introducing_the_miral_api/)  
- [Tiling Window Manager Example](https://canonical-mir.readthedocs-hosted.com/v2.18.3/api/program_listing_file_examples_example-server-lib_tiling_window_manager.h/)  
- [Mir Examples (Debian Sources)](https://sources.debian.org/src/mir/1.8.0%2Bdfsg1-18/examples/)  
- [Reddit Discussion on Tiling Window Managers](https://www.reddit.com/r/linux/comments/5t9f7a/for_those_curious_about_tiling_window_managers/)  
- [Mir GitHub Repository](https://github.com/canonical/mir)  
- [Mir PDF Documentation (v2.17.2)](https://canonical-mir.readthedocs-hosted.com/_/downloads/en/v2.17.2/pdf/)  
