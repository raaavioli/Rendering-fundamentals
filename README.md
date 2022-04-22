# DH2323 Computer Graphics and Interaction - Rendering track
This is a base repository for the rendering track of DH2323 Computer Graphics and Interaction.

The repository is meant to provide a simple API to be able to write pixels to a window. Ultimately, we want to make it 
easy for both students and TAs in DH2323 to get started with the labs, so that more time can be put into what the 
students are meant to learn. The purpose of the labs are to get familiar with the fundamentals of ray-tracing and 
rasterization, while the purpose is not to learn about linking libraries and building boilerplate code to set up a 
window or a graphics context.

## Dependencies
- CMake (Minimum version 3.2)

## Build
> git clone --recurse-submodules https://github.com/raaavioli/Rendering-fundamentals
> cd Rendering-fundamentals && mkdir build && cd build
> cmake ..

### Operating systems
The repo has currently been verified to work on:
- Windows 10 with Visual Studio 2022

More operating systems will be added as they are verified, and the goal is to support a wide range of operating system so 
that students can run from any environment.

### Windows
When building, CMake will generate a visual studio solution "DH2323\_Rendering.sln".

- Find _DH2323\_Rendering_ in the Solution Explorer
- Right click -> Set as Startup Project
- Build -> Build Solution
