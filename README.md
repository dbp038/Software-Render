# Software-Render
## About the project
A basic software renderer application just for the purpose of acquiring a better insight on how a GPU render pipeline works from within. It features a programmable render pipeline (vertex input, vertex shader, geometry shader, pixel shader) in C++ like syntax; an example scene is provided inside the src/game/ folder.

![alt text][app-screenshot]

All the work is handled by the CPU, no Direct3D or OpenGL involved (even the process of painting the window is directly made with Win32 API), it should work fine with any recent CPU, but keep in mind that this project isn't designed for performance. It's programmed to use only one thread, but there is an experimental branch with a multi-threaded aproach that improves FPS.

## Acknowledgements
* [ChiliTomatoNoodle](https://www.youtube.com/user/ChiliTomatoNoodle) - best teacher 11/10 ([source](https://github.com/planetchili/3D_Fundamentals))
* [Eigen](https://gitlab.com/libeigen/eigen)
* [OBJ_Loader](https://github.com/Bly7/OBJ-Loader)
* [STB](https://github.com/nothings/stb) - texture loading using stb_image
* [DirectXTK](https://github.com/microsoft/DirectXTK) - used modified versions of Keyboard and Mouse classes
* [Skull Model](https://free3d.com/3d-model/skull-v3--785914.html)


<!-- MARKDOWN LINKS & IMAGES -->
[app-screenshot]: images/skull_anim.gif
