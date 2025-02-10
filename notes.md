## **mirA: A Lightweight Tiling Window Manager for Mir**

mirA is a lightweight **tiling window manager** built on top of **Mir**. It provides efficient window management with **workspaces, keyboard-driven controls, and auto-tiling**. The goal of mirA is to offer a structured and intuitive window management experience while keeping the implementation simple.

### **Design Decisions**

#### **1. Grid-Based Tiling Layout**
mirA organizes windows in a **grid pattern** rather than dynamic stacking or floating layouts. This choice ensures that all open windows are neatly arranged without overlapping.

#### **2. Workspace Support**
mirA implements **workspaces**, allowing users to group applications into separate virtual desktops.
- Users can **switch between workspaces** using keyboard shortcuts.
- Windows can be **moved across workspaces**, allowing dynamic organization based on user needs.

#### **3. Fullscreen Support**
MirA includes **fullscreen mode**. When a window is set to fullscreen:
- All other windows are hidden.
- The active window occupies the entire screen.
- Exiting fullscreen restores the previous tiling layout.

#### **4. Minimalist Approach to System UI**
To simplify configuration and avoid complexity, mirA includes two **special-purpose applications**:
- **A panel application:** Acts as a lightweight system bar for essential functions.
- **A wallpaper application:** Manages the desktop background without interfering with tiling behavior.

#### **5. No Drag & Drop or Window Resizing**
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
  _Allow moving windows within the same workspace without resizing._


---

## **Useful Links**
📖 **Official Mir Documentation:**  
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

## **Dependencies & Installation**
### **Install Mir and Build Tools**
```sh
sudo add-apt-repository ppa:mir-team/release
sudo apt update
sudo apt install libmiral-dev mir-graphics-drivers-desktop
sudo apt install build-essential cmake pkg-config
```

### **Install Required Packages**
```sh
sudo apt install xfce4-terminal
sudo apt install mir-demos mir-graphics-drivers-desktop
```

### **(Optional) Install a Game for Testing**
```sh
sudo apt install bomber
```

---

## **Components**
### **Tiling Window Manager**
This is the core of mirA, managing windows in a **tiling layout** with support for multiple workspaces.

### **Panel**
A lightweight **GTK3-based panel** that provides a simple workspace indicator.  
Install required dependencies:
```sh
sudo apt install libgtk-3-dev
```

### **Wallpaper**
A minimal **GTK3 + Cairo** application for setting a wallpaper.  
Install required dependencies:
```sh
sudo apt install libcairo2-dev libgtk-3-dev
```


