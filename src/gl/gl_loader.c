#include "scop.h"

/* Fills every pointer of GL_FUNCTIONS from the current context; GLFW resolves the names. */

GL_FUNCTIONS(GL_DEFINE)

/* Needs a current context. Reports every missing function, not only the first. */
int	gl_load_functions(void)
{
	int	ok;

	ok = 1;
	GL_FUNCTIONS(GL_LOAD)
	return (ok);
}
