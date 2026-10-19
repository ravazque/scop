# scop

## Description

scop is an introduction to GPU rendering: a small program written in C with OpenGL 4.1 that loads a 3D model from a `.obj` file and displays it in a window, in perspective. The model spins around its own vertical axis and can be rotated and moved along its three axes from the keyboard. Every face of the `.obj` gets its own subtle shade of gray, and one key fades a texture in and out over the model.

Apart from the window and the keyboard, handled by GLFW, everything is written from scratch: the `.obj` parser, the vector and matrix math, the shader loading, the BMP texture loader and the OpenGL function loader.

As a bonus, scop:

- triangulates concave and non-planar faces by ear clipping, so the original Utah teapot renders exactly like its re-exported version (`resources/extra/` adds a concave star prism and a crown with non-planar faces);
- maps the texture triplanar, so no face stretches it, with a key to compare against a single planar projection;
- has a soft diffuse light, wireframe and point drawing modes, an optional texture argument, a window size argument, and the frame rate in the window title.

## Instructions

### Dependencies

| Dependency | Version | Debian / Ubuntu | Arch |
|---|---|---|---|
| C compiler and GNU Make | C11 | `build-essential` | `base-devel` |
| `pkg-config` | any | `pkg-config` | `pkgconf` |
| GLFW 3 | 3.3 or newer, or built by `make` | `libglfw3-dev` | `glfw` |
| An OpenGL 4.1 driver | Mesa or a vendor driver | installed with the desktop | same |
| `valgrind` | optional, for `make valgrind` | `valgrind` | `valgrind` |

`make` uses the system GLFW when `pkg-config` finds it. Otherwise it downloads GLFW 3.5.1 into `lib/` with `curl` or `wget`, checks it against a pinned SHA-256 and builds it as a static library with `cmake`, without root; that build needs the X11 or Wayland development files, and `make` names whatever is missing. `make install` checks the compiler and `pkg-config` and makes sure GLFW is available.

### Build and run

```bash
make install                                      # checks the dependencies, builds GLFW when the system has none
make                                              # builds ./scop (-O2; O=0, 1, 3, s or g for another level)
make run                                          # ./scop resources/42.obj (ARGS="..." for another model)
make valgrind                                     # the same under valgrind, with the suppression files of docs/
make clean / fclean / re
./scop <model.obj> [texture.bmp] [width height]
```

The texture defaults to `resources/kittens.bmp` and the window to 1280x720, within 1280x720 (HD) and 3840x2160 (4K), also when it is resized. The program runs from the repository root, where `shaders/` is found. Model and texture names need a name before their extension: `.obj`, `model` or `model..obj` are refused. Every refusal prints one line on standard error and exits with status 1 before any window opens.

| Key | Action |
|---|---|
| `W` / `S`, `A` / `D`, `Q` / `E` | Rotation around the model's X, Y and Z axes |
| `←` / `→`, `↓` / `↑`, `F` / `R` | Movement along X, Y and Z (away / closer) |
| `T` | Texture on / off, with a smooth transition |
| `U` | Texture mapping: triplanar (default) or a single planar projection |
| `L` | Soft diffuse light on / off, with a smooth transition |
| `M` | Drawing mode: filled faces, wireframe, points |
| `Space` | Pause / resume the automatic rotation |
| `Backspace` | Back to the initial position |
| `H` | Frame rate and model path in the window title |
| `ESC` | Quit |

Rotation and movement keys act while held, at a speed that does not depend on the frame rate. Frames follow the monitor's refresh (vsync); when the driver ignores vsync, the loop is capped at 240 frames per second. The model is centered on its bounding box and scaled to fit the view, so it always turns around its own center. `Ctrl+C` in the terminal also closes the program cleanly.

### Valgrind

The window system, the toolkit that draws the window decorations and the OpenGL driver keep global state until the process exits. The two suppression files in `docs/` silence only that code, matched by library, never by a project function:

| File | Valgrind | Contents |
|---|---|---|
| `docs/valgrind.supp` | every version | Wayland, X11, GTK / libdecor, GLib, D-Bus, fontconfig, Mesa, NVIDIA, libstdc++ and the glibc loader |
| `docs/valgrind_recent.supp` | 3.22 or newer | Two NVIDIA start-up errors whose kinds (`ReallocZero`, `BadSize`) older versions do not know |

`make valgrind` always loads `docs/valgrind.supp` and adds `docs/valgrind_recent.supp` when the installed valgrind accepts it. `--keep-debuginfo=yes` is required: the Mesa driver is unloaded before the leak check, and without it its frames lose their library name. A clean run ends with every lost and reachable counter at 0 and `ERROR SUMMARY: 0 errors`, with the system GLFW or the static one.

## How it works

- **Loading** (`src/obj/`, `src/image/`): the model and the texture are read before the window opens, so a bad file fails without one. The `.obj` is parsed in two passes, vertices first, so a face may name any vertex; corners can be `v`, `v/vt`, `v//vn` or `v/vt/vn`, and negative indices count back from the face's line. Polygons are ear-clipped in the plane of their Newell normal and degenerate triangles are dropped. The BMP loader reads 24 and 32-bit files, uncompressed or with byte-aligned `BI_BITFIELDS` masks, bottom-up or top-down, with any header from `BITMAPINFOHEADER` to V5. Errors name the file, and the line for a `.obj`.
- **Mesh** (`src/gl/mesh.c`): every triangle corner gets its own vertex with its position, the face normal and a gray level per `.obj` face, taken from the golden-ratio sequence so neighboring faces differ. The model is centered on the bounding box of its triangles and scaled into the unit sphere, so any model turns around its center and fits the view.
- **Transforms** (`src/app/view.c`, `src/math/`): 4x4 column-major matrices, uploaded without transposition. The model matrix is a translation, then rotations around Z and X, then the rotation around the model's own Y axis that carries the automatic spin; the camera uses a look-at view and a perspective projection.
- **Shading** (`shaders/`): the texture is projected in model space, so it stays on the model while it turns, and keeps the image's proportions. The triplanar mapping blends a projection along each axis by the face normal, each one upright and unmirrored seen from outside. The texture, the mapping and the light switch through 0.8 s smoothstep fades instead of cutting.
- **OpenGL loader** (`include/gl_loader.h`, `src/gl/gl_loader.c`): a struct with one function pointer per OpenGL 4.1 core function the program calls, filled from a table of names with `glfwGetProcAddress` once the context exists and returned by `gl()`. No OpenGL header or library is needed to build.

### Shared with ft_newton

scop came after ft_newton, a rigid-body physics engine, and builds on parts of it:

- **The `.obj` reader** (`src/obj/`) is ft_newton's: two passes so a face may name a later vertex, corners as `v`, `v/vt`, `v//vn` or `v/vt/vn`, negative indices, ear clipping in the plane of the Newell normal, degenerate faces dropped and `file:line` errors. ft_newton then repairs the surface into a closed one (welding split vertices, closing holes), because its models are colliders that need a volume; scop only draws it, so open models like the Utah teapot load as they are.
- **A model**: `resources/42.obj`, the one scop shows by default, is also one of ft_newton's mesh colliders.
- **Small helpers**: reading a whole file (`src/util/file.c`) and growing an array by doubling (`src/util/array.c`).
- **The rendering basics**: column-major 4x4 matrices uploaded without a transpose, a look-at view and a perspective projection, a flat normal per triangle corner, GLSL shaders read from `shaders/`, a GLFW window whose size argument is checked against bounds and whose title shows the frame rate on `H`.
- **The way they are built and checked**: `make install` and `make help`, `make O=…` with a flags stamp that rebuilds the objects when the flags change, `make valgrind` with the same two suppression files and `--keep-debuginfo=yes`, and strict loading that refuses a bad file before any window opens.

They differ in what each one needs: scop draws with OpenGL 4.1 through its own function loader, follows the monitor's refresh and adds textures and lighting; ft_newton uses OpenGL 3.3 Core through GLAD and runs a fixed 120 Hz physics step behind a 30 frames per second cap.

## Resources

- [docs.gl](https://docs.gl/): OpenGL function reference.
- [OpenGL 4.1 core profile specification](https://registry.khronos.org/OpenGL/specs/gl/glspec41.core.pdf), Khronos.
- [LearnOpenGL](https://learnopengl.com/): buffers, shaders, transformations, textures and lighting.
- [GLFW documentation](https://www.glfw.org/docs/latest/): windows, contexts, input and `glfwGetProcAddress`.
- [Wavefront OBJ file format](https://paulbourke.net/dataformats/obj/), Paul Bourke.
- [BMP file format](https://en.wikipedia.org/wiki/BMP_file_format), Wikipedia.
- David Eberly, [Triangulation by Ear Clipping](https://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf), Geometric Tools.
- Filippo Tampieri, *Newell's Method for Computing the Plane Equation of a Polygon*, Graphics Gems III, 1992.
- Ryan Geiss, [Generating Complex Procedural Terrains Using the GPU](https://developer.nvidia.com/gpugems/gpugems3/part-i-geometry/chapter-1-generating-complex-procedural-terrains-using-gpu), GPU Gems 3, chapter 1: triplanar texturing.
- Texture: [Three innocent kittens](https://www.flickr.com/photos/38709274@N02/14582884739) by iDapinder, Public Domain Dedication (CC0 1.0), cropped to 768x768 and stored as `resources/kittens.bmp`.
