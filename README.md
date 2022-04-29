# DH2323 Computer Graphics and Interaction - Rendering track
This is a base repository for the rendering track of DH2323 Computer Graphics and Interaction.

The repository is meant to provide a simple API to be able to write pixels to a window. Ultimately, we want to make it 
easy for both students and TAs in DH2323 to get started with the labs, so that more time can be put into what the 
students are meant to learn. The purpose of the labs is to get familiar with the fundamentals of ray-tracing and 
rasterization, the purpose is not to spend time on linking libraries and building boilerplate code to set up a 
window or a graphics context.

## Dependencies
- CMake (Minimum version 3.2)

## Build
```
git clone --recurse-submodules https://github.com/raaavioli/Rendering-fundamentals
```
```
cd Rendering-fundamentals && mkdir build && cd build && cmake ..
```
### MacOS + Linux
CMake will generate a Makefile.
```
make
```
Which produces a binary ```bin/DH2323_Rendering```.

### Windows
When building, CMake will generate a visual studio solution ```DH2323_Rendering.sln```.

- Open ```build/DH2323_Rendering.sln```
- Find _DH2323\_Rendering_ in the Solution Explorer
- Right click -> Set as Startup Project
- Build -> Build Solution

### Operating systems
The repo has currently been verified to work on:
- Windows 10 with VS2022 and VS2019
- MacOS Big Sur (M1-chip)

More operating systems will be added as they are verified, and the goal is to support a wide range of operating systems so 
that students can run from any environment.

## Creating Scenes
To use the tool, one creates a class deriving from [_Scene_](https://github.com/raaavioli/Rendering-fundamentals/tree/main/labscenes) (see [scene.h](https://github.com/raaavioli/Rendering-fundamentals/tree/main/include/scene.h), [example_scene.h](https://github.com/raaavioli/Rendering-fundamentals/tree/main/labscenes/example_scene.h) and [example_scene.cpp](https://github.com/raaavioli/Rendering-fundamentals/tree/main/labscenes/example_scene.cpp)).
The created Scene subclass needs to be included into main.cpp, and added to the list of scenes at the top of the main()-function.

This way, all Scenes are selectable from the graphical settings-panel when running the program. The general approach 
for solving a lab assignment is to create a Scene, and implement **Update**, **Draw** and **DrawGUI** to output imagery 
in line with the assignment specification.

## Assignment
Lab document: https://www.overleaf.com/read/kqpqdndnpcgd
