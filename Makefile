NAME		= scop

CC			= cc

# Optimisation level, -O2 by default: `make O=0` (or 1, 3, s, g). Objects are rebuilt whenever the compile flags change.
O			= 2

CFLAGS		= -Wall -Wextra -Werror -std=c11 -O$(O) -g3
CPPFLAGS	= -Iinclude
LDLIBS		= -lm

SRCDIR		= src
OBJDIR		= obj
FLAGS_STAMP	= $(OBJDIR)/.flags

SRCS		= main.c \
			  app/app.c app/args.c app/window.c app/input.c app/view.c app/fade.c app/hud.c app/draw.c \
			  gl/gl_loader.c gl/shader.c gl/mesh.c gl/texture.c \
			  math/vec3.c math/mat4.c math/projection.c \
			  obj/obj_load.c obj/obj_face.c obj/obj_triangulate.c \
			  image/bmp.c \
			  util/file.c util/array.c

OBJS		= $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS		= $(OBJS:.o=.d)

ARGS		= resources/42.obj

# GLFW: the system copy found by pkg-config, otherwise the pinned release downloaded to lib/ and built static with cmake.
GLFW_VERSION	= 3.5.1
GLFW_URL		= https://github.com/glfw/glfw/archive/refs/tags/$(GLFW_VERSION).tar.gz
GLFW_SHA256		= 5234f4f29473e9a06bc7847d8371858dd135d38466eeeaa652fdc9f8f9ff0c20
GLFW_SRC		= lib/glfw-$(GLFW_VERSION)
GLFW_TMP		= lib/.download
GLFW_BUILD		= lib/build
GLFW_PREFIX		= lib/glfw

ifeq ($(shell pkg-config --exists glfw3 2>/dev/null && echo yes),yes)
GLFW_CFLAGS		:= $(shell pkg-config --cflags glfw3)
GLFW_LIBS		:= $(shell pkg-config --libs glfw3)
GLFW_DEP		:=
else
GLFW_CFLAGS		:= -I$(GLFW_SRC)/include
GLFW_LIBS		= $(shell PKG_CONFIG_PATH=$(GLFW_PREFIX)/lib/pkgconfig pkg-config --static --libs glfw3)
GLFW_DEP		:= $(GLFW_PREFIX)/lib/libglfw3.a
GLFW_X11		= $(shell pkg-config --exists x11 xrandr xinerama xcursor xi xext 2>/dev/null && echo ON || echo OFF)
GLFW_WAYLAND	= $(shell pkg-config --exists wayland-client wayland-cursor \
					wayland-egl xkbcommon 2>/dev/null \
					&& command -v wayland-scanner >/dev/null && echo ON || echo OFF)
endif

VALGRIND		= valgrind
SUPP			= docs/valgrind.supp
SUPP_RECENT		= docs/valgrind_recent.supp
# Expanded only by `make valgrind`: the recent file is added when this valgrind accepts it.
SUPPFLAGS		= --suppressions=$(SUPP) \
				  $(shell $(VALGRIND) --suppressions=$(SUPP_RECENT) true >/dev/null 2>&1 && echo --suppressions=$(SUPP_RECENT))
VALGRINDFLAGS	= --leak-check=full --show-leak-kinds=all --track-origins=yes --keep-debuginfo=yes $(SUPPFLAGS)


all: $(NAME)

$(NAME): $(OBJS) $(GLFW_DEP)
	$(CC) $(OBJS) $(GLFW_LIBS) $(LDLIBS) -o $@

# Order-only on GLFW, so a missing one is reported before any source is compiled.
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(FLAGS_STAMP) | $(GLFW_DEP)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(GLFW_CFLAGS) -MMD -MP -c $< -o $@

# Rewritten only when the flags differ from the last build, so its date is the last flag change.
$(FLAGS_STAMP): FORCE
	@mkdir -p $(OBJDIR)
	@echo '$(CC) $(CFLAGS) $(CPPFLAGS) $(GLFW_CFLAGS)' | cmp -s - $@ || echo '$(CC) $(CFLAGS) $(CPPFLAGS) $(GLFW_CFLAGS)' > $@

FORCE:

# Downloaded and checked in a temporary folder, then moved in place: a failed download is never built.
$(GLFW_SRC):
	@command -v curl >/dev/null || command -v wget >/dev/null || { echo "GLFW 3 not found, and curl or wget is required to download GLFW $(GLFW_VERSION)" >&2; exit 1; }
	@echo "GLFW 3 not found: downloading GLFW $(GLFW_VERSION) into lib/"
	@rm -rf $(GLFW_TMP) && mkdir -p $(GLFW_TMP)
	@if command -v curl >/dev/null; then curl -fsSL $(GLFW_URL) -o $(GLFW_TMP)/glfw.tar.gz; else wget -q $(GLFW_URL) -O $(GLFW_TMP)/glfw.tar.gz; fi
	@echo "$(GLFW_SHA256)  $(GLFW_TMP)/glfw.tar.gz" | sha256sum -c --status || { echo "GLFW $(GLFW_VERSION): checksum mismatch, download discarded" >&2; exit 1; }
	@tar -xzf $(GLFW_TMP)/glfw.tar.gz -C $(GLFW_TMP)
	@mv $(GLFW_TMP)/glfw-$(GLFW_VERSION) $@
	@rm -rf $(GLFW_TMP)

$(GLFW_DEP): | $(GLFW_SRC)
	@command -v cmake >/dev/null || { echo "cmake is required to build GLFW $(GLFW_VERSION)" >&2; exit 1; }
	@command -v pkg-config >/dev/null || { echo "pkg-config is required to link GLFW $(GLFW_VERSION)" >&2; exit 1; }
	@test "$(GLFW_X11)$(GLFW_WAYLAND)" != OFFOFF || { echo "GLFW needs the X11 (libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxext-dev) or Wayland (libwayland-dev libxkbcommon-dev) development files" >&2; exit 1; }
	cmake -S $(GLFW_SRC) -B $(GLFW_BUILD) -DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INSTALL_PREFIX=$(abspath $(GLFW_PREFIX)) -DCMAKE_INSTALL_LIBDIR=lib \
		-DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF \
		-DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_X11=$(GLFW_X11) -DGLFW_BUILD_WAYLAND=$(GLFW_WAYLAND)
	cmake --build $(GLFW_BUILD) --parallel
	cmake --install $(GLFW_BUILD)

run: all
	./$(NAME) $(ARGS)

valgrind: all
	$(VALGRIND) $(VALGRINDFLAGS) ./$(NAME) $(ARGS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME) $(GLFW_BUILD) $(GLFW_PREFIX) $(GLFW_TMP)

re: fclean all

.PHONY: all run valgrind clean fclean re FORCE

-include $(DEPS)
