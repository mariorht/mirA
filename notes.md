### Webs

https://canonical-mir.readthedocs-hosted.com/stable/tutorial/getting-started-with-mir/
https://canonical-mir.readthedocs-hosted.com/stable/tutorial/write-your-first-wayland-compositor/

### My system:
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


### Dependencies:

sudo add-apt-repository ppa:mir-team/release
sudo apt update
sudo apt install libmiral-dev mir-graphics-drivers-desktop
sudo apt install build-essential cmake pkg-config
sudo apt install xfce4-terminal
sudo apt install mir-demos mir-graphics-drivers-desktop
