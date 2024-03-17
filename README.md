# 3D Engine
A simple 3D engine programmed using SDL2 and the C language.

## How to use it
Open the project using Visual Studio 2022 and link the SDL2 libraries.
If you try to run the project, it will print and error message: "Could not load mesh". This happens because it is trying to load an object called "ship.obj" which is not included.
Put any .obj file in the project folder and change the name of the file the program is trying to load.
The .obj file must only use triangles, it should not store vector normals or texture information.
