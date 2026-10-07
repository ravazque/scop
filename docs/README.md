*This project has been created as part of the 42 curriculum by ravazque.*

# scop

## Description

scop is an introduction to **GPU rendering**: a small program written in **C** with **OpenGL 4.1** that loads a 3D model from a `.obj` file and displays it in a window, in perspective.

The model spins around its own vertical axis and can be rotated and moved along its three axes from the keyboard. Every face of the `.obj` gets its own subtle shade of gray, and one key fades a texture in and out over the model.

Apart from the window and the keyboard, handled by GLFW, everything is written from scratch: the `.obj` parser, the vector and matrix math, the shader loading, the BMP texture loader and the OpenGL function loader.

Beyond that, scop:

- triangulates concave and non-planar faces by ear clipping, and draws the original Utah teapot exactly like its re-exported version (`resources/extra/` adds a concave star prism and a crown with non-planar faces);
- maps the texture triplanar, so no face stretches it, with a key to compare against a single planar projection;
- has a soft diffuse light, wireframe and point drawing modes, an optional texture argument, and the frame rate in the window title.

## Instructions

### Requirements

- A C compiler, `make` and `pkg-config`.
- GLFW 3 (the only external library, used for the window and the input) and an OpenGL 4.1 driver.

`make` uses the system GLFW when `pkg-config` finds it. Otherwise it downloads GLFW 3.5.1 into `lib/` with `curl` or `wget`, checks it against a pinned SHA-256 and builds it as a static library with `cmake`, without root. That build needs the X11 (`libx11-dev`, `libxrandr-dev`, `libxinerama-dev`, `libxcursor-dev`, `libxi-dev`, `libxext-dev`) or Wayland (`libwayland-dev`, `libxkbcommon-dev`) development files, and `make` stops naming what is missing. `make fclean` keeps the downloaded sources, so a rebuild needs no network.

### Build and run

```bash
make                                # builds ./scop (-O2)
make O=0                            # optimisation level: 2 by default, or 0, 1, 3, s, g
./scop <model.obj> [texture.bmp] [width height]
                                    # texture: resources/kittens.bmp by default
                                    # window size: 1280x720 (HD) to 3840x2160 (4K), 1280x720 by default
make run                            # ./scop resources/42.obj (ARGS="..." to change it)
norminette src include              # the C sources follow the 42 Norm
make clean / fclean / re
```

Run it from the repository root, so `shaders/` is found. The model and texture names need a name before their extension: `.obj`, `model` or `model..obj` are refused. Resizing the window by its border stays within the same 1280x720 to 3840x2160 range.

| Key | Action |
|---|---|
| `W` / `S` | rotate around the model's X axis |
| `A` / `D` | rotate around the model's Y axis |
| `Q` / `E` | rotate around the model's Z axis |
| `←` / `→` | move along X |
| `↓` / `↑` | move along Y |
| `F` / `R` | move along Z, away / closer |
| `T` | apply / remove the texture, with a smooth transition |
| `U` | switch the texture mapping between triplanar (default, no stretching) and a single planar projection |
| `L` | turn a soft diffuse light on / off, with a smooth transition |
| `M` | cycle the drawing mode: filled faces, wireframe, points |
| `Space` | pause / resume the automatic rotation |
| `Backspace` | back to the initial position |
| `H` | show / hide the frame rate and the model path in the window title |
| `ESC` | quit |

Rotation and movement keys act while held, at a speed that does not depend on the frame rate. The model is centered on its bounding box and scaled to fit the view, so it always turns around its own center.

The window title is `scop` by default and `scop  /  FPS:<rate> | '<model path>' |` while `H` has it shown, with the path exactly as given on the command line. `Ctrl+C` in the terminal also closes the program cleanly.

### Valgrind

```bash
make valgrind    # accepts ARGS too
```

The window system, the toolkit that draws the window decorations and the OpenGL driver allocate global state that they keep until the process exits. Two suppression files in `docs/` silence only that code, matched by library, never by a project function:

| File | Valgrind | Contents |
|---|---|---|
| [valgrind.supp](valgrind.supp) | every version | Wayland, X11, GTK / libdecor, GLib, D-Bus, fontconfig, Mesa, NVIDIA, libstdc++ and the glibc loader |
| [valgrind_recent.supp](valgrind_recent.supp) | 3.22 or newer | Two NVIDIA start-up errors whose kinds (`ReallocZero`, `BadSize`) older versions do not know |

`make valgrind` always loads the first file and adds the second only when the installed valgrind accepts it. `--keep-debuginfo=yes` is required: the Mesa driver is unloaded before the leak check, and without it its frames lose their library name. A clean run ends with every lost and reachable counter at 0 and `ERROR SUMMARY: 0 errors`, with the system GLFW or the static one. No entry matches GLFW itself: it frees its own state, and its frames sit under every callback of the program, so matching them would also hide the program's own leaks.

Without the suppression files, a Wayland run reports no definitely lost block. On X11, libX11 keeps the locale (Xlc), input method and resource-quark caches it builds during `glfwInit` until the process ends, and valgrind reports them as definitely lost; none of them comes from the program.

## How it works

- **Loading** (`src/obj/`, `src/image/`): the model and the texture are read before the window opens, so a bad file fails without one. The `.obj` is parsed in two passes, vertices first, so a face may name any vertex; corners can be `v`, `v/vt`, `v//vn` or `v/vt/vn`, and negative indices count back from the face's line. Polygons are ear-clipped in the plane of their Newell normal and degenerate triangles are dropped. The BMP loader reads 24 and 32-bit files, uncompressed or with byte-aligned `BI_BITFIELDS` masks, bottom-up or top-down, with any header from `BITMAPINFOHEADER` to V5. Errors name the file, and the line for a `.obj`.
- **Mesh** (`src/gl/mesh.c`): every triangle corner gets its own vertex with its position, the face normal and a gray level per `.obj` face, taken from the golden-ratio sequence so neighboring faces differ. The model is centered on the bounding box of its triangles and scaled into the unit sphere, so any model turns around its center and fits the view.
- **Transforms** (`src/app/view.c`, `src/math/`): 4x4 column-major matrices, uploaded without transposition. The model matrix is a translation, then rotations around Z and X, then the rotation around the model's own Y axis that carries the automatic spin; the camera uses a look-at view and a perspective projection.
- **Shading** (`shaders/`): the texture is projected in model space, so it stays on the model while it turns, and keeps the image's proportions. The triplanar mapping blends a projection along each axis by the face normal, each one upright and unmirrored seen from outside. The texture, the mapping and the light switch through 0.8 s smoothstep fades instead of cutting.
- **OpenGL loader** (`include/gl_loader.h`, `src/gl/gl_loader.c`): a struct with one function pointer per OpenGL 4.1 core function the program calls, filled with `glfwGetProcAddress` once the context exists and returned by `gl()`. No OpenGL header or library is needed to build.

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

### Use of AI

An AI assistant was used as a support tool, under the author's direction and review: to generate and review this README, to write and run tests (generated `.obj` and BMP files, rendered-frame checks, valgrind runs), to migrate the build setup from previous projects (Makefile, GLFW build, valgrind suppressions), and as a coding aid on parts of the implementation (the `.obj` parser, the BMP loader and the shaders).
