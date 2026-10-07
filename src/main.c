#include "scop.h"

int	main(int argc, char **argv)
{
	t_app	app;

	if (!app_init(&app, argc, argv))
	{
		app_destroy(&app);
		return (1);
	}
	app_run(&app);
	app_destroy(&app);
	return (0);
}
