*This project has been created as part of the 42 curriculum by ravazque.*

# scop

## Description

scop is an introduction to **GPU rendering**: a small program written in **C** with **OpenGL** that loads a 3D model from a `.obj` file and displays it in a window with perspective projection.

The object can be rotated and translated around its three main axes, with the origin at its center. Its faces are shaded in distinct tones of gray, and a dedicated key toggles a texture on and off with a smooth transition between both views.

Apart from window and event management, everything is implemented from scratch: the `.obj` parser, the matrix math, the shader loading and the texture loading.

## Instructions

### Requirements

- A C compiler, `make` and `pkg-config`.
- GLFW 3 (the only external library, used for the window and the input) and an OpenGL 4.1 driver.

`make` uses the system GLFW when `pkg-config` finds it. Otherwise it downloads GLFW 3.5.1 into `lib/` with `curl` or `wget`, checks it against a pinned SHA-256 and builds it as a static library with `cmake`, without root. That build needs the X11 (`libx11-dev`, `libxrandr-dev`, `libxinerama-dev`, `libxcursor-dev`, `libxi-dev`, `libxext-dev`) or Wayland (`libwayland-dev`, `libxkbcommon-dev`) development files, and `make` stops naming what is missing. `make fclean` keeps the downloaded sources, so a rebuild needs no network.

### Build and run

```bash
make                                # builds ./scop (-O0)
make O=2                            # optimisation level: 0 by default, or 1, 2, 3, s, g
./scop <model.obj> [width height]   # window size: 1280x720 (HD) to 3840x2160 (4K), 1280x720 by default
make run                            # ./scop resources/42.obj (ARGS="..." to change it)
make clean / fclean / re
```

Run it from the repository root, so `shaders/` is found. Resizing the window by its border stays within the same 1280x720 to 3840x2160 range.

| Key | Action |
|---|---|
| `ESC` | quit |
| `H` | show / hide the frame rate and the model path in the window title |

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
