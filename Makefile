NAME		= scop

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -std=c11 -O2 -g3
CPPFLAGS	= -Iinclude
LDLIBS		= -lm

SRCDIR		= src
OBJDIR		= obj

SRCS		= main.c \
			  app/app.c app/args.c app/window.c app/input.c app/hud.c app/draw.c \
			  gl/gl_loader.c gl/shader.c gl/mesh.c \
			  math/vec3.c math/mat4.c math/projection.c

OBJS		= $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS		= $(OBJS:.o=.d)

ARGS		= resources/42.obj

# GLFW: the system copy found by pkg-config, otherwise a static cmake build of the sources in lib/ (never downloaded).
GLFW_VERSION	= 3.5.1
GLFW_SRC		= lib/glfw-$(GLFW_VERSION)
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
GLFW_WAYLAND	= $(shell pkg-config --exists wayland-client wayland-cursor \
					wayland-egl xkbcommon 2>/dev/null \
					&& command -v wayland-scanner >/dev/null && echo ON || echo OFF)
endif

VALGRIND		= valgrind
SUPP			= docs/valgrind.supp
SUPP_RECENT		= docs/valgrind_recent.supp
# Expanded only by `make valgrind`: the recent file is added when this valgrind accepts it.
SUPPFLAGS		= --suppressions=$(SUPP) \
				  $(shell $(VALGRIND) --suppressions=$(SUPP_RECENT) true >/dev/null 2>&1 \
					&& echo --suppressions=$(SUPP_RECENT))
VALGRINDFLAGS	= --leak-check=full --show-leak-kinds=all --track-origins=yes --keep-debuginfo=yes $(SUPPFLAGS)


all: $(NAME)

$(NAME): $(OBJS) $(GLFW_DEP)
	$(CC) $(OBJS) $(GLFW_LIBS) $(LDLIBS) -o $@

# Order-only on GLFW, so a missing one is reported before any source is compiled.
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(GLFW_DEP)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(GLFW_CFLAGS) -MMD -MP -c $< -o $@

$(GLFW_DEP):
	@test -d $(GLFW_SRC) || { echo "GLFW 3 not found: install it (libglfw3-dev," \
		"glfw, ...) or place the GLFW $(GLFW_VERSION) sources in $(GLFW_SRC)" >&2; exit 1; }
	@command -v cmake >/dev/null || { echo "cmake is required to build $(GLFW_SRC)" >&2; exit 1; }
	@command -v pkg-config >/dev/null || { echo "pkg-config is required to link $(GLFW_SRC)" >&2; exit 1; }
	cmake -S $(GLFW_SRC) -B $(GLFW_BUILD) -DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INSTALL_PREFIX=$(abspath $(GLFW_PREFIX)) -DCMAKE_INSTALL_LIBDIR=lib \
		-DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF \
		-DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_WAYLAND=$(GLFW_WAYLAND)
	cmake --build $(GLFW_BUILD) --parallel
	cmake --install $(GLFW_BUILD)

run: all
	./$(NAME) $(ARGS)

valgrind: all
	$(VALGRIND) $(VALGRINDFLAGS) ./$(NAME) $(ARGS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME) $(GLFW_BUILD) $(GLFW_PREFIX)

re: fclean all

.PHONY: all run valgrind clean fclean re

-include $(DEPS)
