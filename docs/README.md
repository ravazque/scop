*This project has been created as part of the 42 curriculum by ravazque.*

# scop

## Description

scop is an introduction to **GPU rendering**: a small program written in **C** with **OpenGL** that loads a 3D model from a `.obj` file and displays it in a window with perspective projection.

The object can be rotated and translated around its three main axes, with the origin at its center. Its faces are shaded in distinct tones of gray, and a dedicated key toggles a texture on and off with a smooth transition between both views.

Apart from window and event management, everything is implemented from scratch: the `.obj` parser, the matrix math, the shader loading and the texture loading.
