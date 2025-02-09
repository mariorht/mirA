### Webs

https://canonical-mir.readthedocs-hosted.com/stable/tutorial/getting-started-with-mir/
https://canonical-mir.readthedocs-hosted.com/stable/tutorial/write-your-first-wayland-compositor/
https://canonical-mir.readthedocs-hosted.com/stable/how-to/how-to-handle-keyboard-input/
https://canonical-mir.readthedocs-hosted.com/stable/api/library_root/#
https://canonical-mir.readthedocs-hosted.com/stable/reference/introducing_the_miral_api/
https://canonical-mir.readthedocs-hosted.com/v2.18.3/api/program_listing_file_examples_example-server-lib_tiling_window_manager.h/
https://sources.debian.org/src/mir/1.8.0%2Bdfsg1-18/examples/
https://www.reddit.com/r/linux/comments/5t9f7a/for_those_curious_about_tiling_window_managers/
https://github.com/ubports/miral

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

Ejemplo de juego
sudo apt install bomber





### Va a ser un tiling window manager
Panel
sudo apt install libgtk-3-dev

Creamos un panel con una aplicación muy sencilla.
Sería mejor usar alguna aplicación como waybar, pero para simplificar la configuración. Mejor sería tener ese panel y dejar que se coloque como una ventana flotnte, pero haremos el truco.

### Lanzar
/mirA --startup-app panel


# **� Lista de Tareas y Mejoras para el Compositor**

## **� Problemas Actuales**
- [ ] **Cambiar de workspace falla si estoy en uno sin ventana**  
  _Solución parcial: Se actualiza `active_workspace`, pero hay que verificar que la interfaz responde correctamente._
- [ ] **Poner fondo de escritorio**  
  _Opciones: Usar `feh`, `swww` (Wayland) o integrar un fondo desde Mir._
- [ ] **Probar en máquina virtual a ver si abre Tint2**  
  _Comprobar compatibilidad y si recibe los eventos correctamente._

---

## **� Mejoras y Ampliaciones**
- [ ] **Mejor gestión del layout**  
  _Hacer que las ventanas se distribuyan de forma más eficiente según el espacio disponible._
- [ ] **Soporte para ventanas flotantes**  
  _Permitir que ciertas ventanas no sigan el tiling y puedan moverse libremente._
- [ ] **Soporte para vista de stack**  
  _Un modo donde las ventanas se apilen en un solo contenedor._
- [ ] **Posibilidad de reordenar ventanas**  
  _Mover ventanas dentro del mismo workspace sin cambiar su tamaño._

