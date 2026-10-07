#include "scop.h"

/* 4x4 column-major model transforms; rotations are counter-clockwise looking down the axis towards the origin. */

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

/* a * b: b is applied first, then a. */
t_mat4	mat4_mul(t_mat4 a, t_mat4 b)
{
	t_mat4	r;
	int		col;
	int		row;
	int		k;

	memset(&r, 0, sizeof(r));
	col = 0;
	while (col < 4)
	{
		row = 0;
		while (row < 4)
		{
			k = 0;
			while (k < 4)
			{
				r.m[col * 4 + row] += a.m[k * 4 + row] * b.m[col * 4 + k];
				k++;
			}
			row++;
		}
		col++;
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

t_mat4	mat4_scale(t_vec3 s)
{
	t_mat4	r;

	r = mat4_identity();
	r.m[0] = s.x;
	r.m[5] = s.y;
	r.m[10] = s.z;
	return (r);
}

t_mat4	mat4_rotation_x(float radians)
{
	t_mat4	r;
	float	c;
	float	s;

	c = cosf(radians);
	s = sinf(radians);
	r = mat4_identity();
	r.m[5] = c;
	r.m[6] = s;
	r.m[9] = -s;
	r.m[10] = c;
	return (r);
}

t_mat4	mat4_rotation_y(float radians)
{
	t_mat4	r;
	float	c;
	float	s;

	c = cosf(radians);
	s = sinf(radians);
	r = mat4_identity();
	r.m[0] = c;
	r.m[2] = -s;
	r.m[8] = s;
	r.m[10] = c;
	return (r);
}

t_mat4	mat4_rotation_z(float radians)
{
	t_mat4	r;
	float	c;
	float	s;

	c = cosf(radians);
	s = sinf(radians);
	r = mat4_identity();
	r.m[0] = c;
	r.m[1] = s;
	r.m[4] = -s;
	r.m[5] = c;
	return (r);
}
