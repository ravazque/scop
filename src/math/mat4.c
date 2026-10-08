#include "scop.h"

t_mat4	mat4_identity(void)
{
	t_mat4	r;

	memset(&r, 0, sizeof(r));
	r.m[0] = 1.0f;
	r.m[5] = 1.0f;
	r.m[10] = 1.0f;
	r.m[15] = 1.0f;
	return (r);
}

/* a * b applies b first, then a; element i sits at column i / 4, row i % 4 */
t_mat4	mat4_mul(t_mat4 a, t_mat4 b)
{
	t_mat4	r;
	int		i;
	int		k;

	memset(&r, 0, sizeof(r));
	for (i = 0; i < 16; i++)
	{
		for (k = 0; k < 4; k++)
			r.m[i] += a.m[k * 4 + i % 4] * b.m[(i / 4) * 4 + k];
	}
	return (r);
}

t_mat4	mat4_translation(t_vec3 t)
{
	t_mat4	r;

	r = mat4_identity();
	r.m[12] = t.x;
	r.m[13] = t.y;
	r.m[14] = t.z;
	return (r);
}

/* Rotations turn counter-clockwise when looking down the axis towards the origin */
t_mat4	mat4_rotation_x(float radians)
{
	const float	c = cosf(radians);
	const float	s = sinf(radians);
	t_mat4		r;

	r = mat4_identity();
	r.m[5] = c;
	r.m[6] = s;
	r.m[9] = -s;
	r.m[10] = c;
	return (r);
}

t_mat4	mat4_rotation_y(float radians)
{
	const float	c = cosf(radians);
	const float	s = sinf(radians);
	t_mat4		r;

	r = mat4_identity();
	r.m[0] = c;
	r.m[2] = -s;
	r.m[8] = s;
	r.m[10] = c;
	return (r);
}

t_mat4	mat4_rotation_z(float radians)
{
	const float	c = cosf(radians);
	const float	s = sinf(radians);
	t_mat4		r;

	r = mat4_identity();
	r.m[0] = c;
	r.m[1] = s;
	r.m[4] = -s;
	r.m[5] = c;
	return (r);
}
