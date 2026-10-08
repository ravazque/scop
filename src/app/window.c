#include "scop.h"

static void	glfw_error_callback(int code, const char *description)
{
	fprintf(stderr, "GLFW error %d: %s\n", code, description);
}

int	window_init(t_app *app)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return (fprintf(stderr, "Error: cannot initialize GLFW\n"), 0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	app->window = glfwCreateWindow(app->width, app->height, WIN_TITLE, NULL, NULL);
	if (!app->window)
	{
		fprintf(stderr, "Error: cannot create an OpenGL 4.1 core window\n");
		glfwTerminate();
		return (0);
	}
	glfwSetWindowSizeLimits(app->window, WIN_WIDTH_MIN, WIN_HEIGHT_MIN, WIN_WIDTH_MAX, WIN_HEIGHT_MAX);
	glfwMakeContextCurrent(app->window);
	glfwSwapInterval(1);
	if (!gl_load())
		return (window_destroy(app), 0);
	return (1);
}

/* Safe at any stage of window_init: glfwTerminate does nothing before glfwInit */
void	window_destroy(t_app *app)
{
	if (app->window)
		glfwDestroyWindow(app->window);
	app->window = NULL;
	glfwTerminate();
}

static void	set_title(t_app *app)
{
	char	title[HUD_TITLE_SIZE];

	if (!app->hud.visible)
	{
		glfwSetWindowTitle(app->window, WIN_TITLE);
		return ;
	}
	snprintf(title, sizeof(title), HUD_FORMAT, app->hud.fps, app->obj_path);
	glfwSetWindowTitle(app->window, title);
}

void	hud_toggle(t_app *app)
{
	app->hud.visible = !app->hud.visible;
	set_title(app);
}

/* Frames are counted over each refresh period: an exact rate and few title rewrites */
void	hud_update(t_app *app, float dt)
{
	t_hud	*h;

	h = &app->hud;
	h->frames++;
	h->elapsed += dt;
	if (h->elapsed < HUD_REFRESH_SECONDS)
		return ;
	h->fps = (int)lroundf((float)h->frames / h->elapsed);
	h->frames = 0;
	h->elapsed = 0.0f;
	if (h->visible)
		set_title(app);
}
