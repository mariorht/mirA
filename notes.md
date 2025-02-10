# **mirA - Tiling Window Manager for Mir**  

mirA is a lightweight **tiling window manager** built on top of **Mir**. It provides efficient window management with **workspaces, keyboard-driven controls, and auto-tiling**.  

---

## **🌍 Useful Links**
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

---

## **💻 System Information**
```
        #####           developer@asus-mario-popos 
       #######          -------------------------- 
       ##O#O##          OS: Pop!_OS 20.04 LTS x86_64 
       #######          Host: ROG Zephyrus G15 GA502IV_GA502IV 1.0 
     ###########        Kernel: 6.0.12-76060012-generic 
    #############       Uptime: 1 hour, 52 mins 
   ###############      Packages: 3112 (dpkg), 33 (flatpak), 13 (snap) 
   ################     Shell: fish 3.1.0 
  #################     Resolution: 1920x1080, 1920x1080, 1920x1080 
#####################   DE: GNOME 
#####################   WM: Mutter 
  #################     WM Theme: Pop 
                        Theme: Pop-dark [GTK2/3] 
                        Icons: Tela-dark [GTK2/3] 
                        Terminal: gnome-terminal 
                        CPU: AMD Ryzen 9 4900HS with Radeon Graphics (16) @ 3.000GHz 
                        GPU: AMD ATI 05:00.0 Renoir 
                        GPU: NVIDIA GeForce RTX 2060 
                        Memory: 7698MiB / 15415MiB 
```                                   

---

## **📦 Dependencies & Installation**
### **1️⃣ Install Mir and Build Tools**
```sh
sudo add-apt-repository ppa:mir-team/release
sudo apt update
sudo apt install libmiral-dev mir-graphics-drivers-desktop
sudo apt install build-essential cmake pkg-config
```

### **2️⃣ Install Required Packages**
```sh
sudo apt install xfce4-terminal
sudo apt install mir-demos mir-graphics-drivers-desktop
```

### **3️⃣ (Optional) Install a Game for Testing**
```sh
sudo apt install bomber
```

---

## **🛠 Components**
### **Tiling Window Manager**
This is the core of mirA, managing windows in a **tiling layout** with support for multiple workspaces.

### **Panel**
A lightweight **GTK3-based panel** that provides a simple workspace indicator.  
📦 Install required dependencies:
```sh
sudo apt install libgtk-3-dev
```

### **Wallpaper**
A minimal **GTK3 + Cairo** application for setting a wallpaper.  
📦 Install required dependencies:
```sh
sudo apt install libcairo2-dev libgtk-3-dev
```

---

## **📜 Roadmap & Improvements**
- [ ] **Better layout management**  
  _Improve how windows are tiled dynamically based on available space._
- [ ] **Floating window support**  
  _Allow certain windows to be free-floating outside the tiling layout._
- [ ] **Stack view support**  
  _Enable a mode where windows stack within a container._
- [ ] **Reorder windows**  
  _Allow moving windows within the same workspace without resizing._

