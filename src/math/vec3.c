#include "scop.h"

/* 3D vectors: positions, directions and normals. */

t_vec3	vec3(float x, float y, float z)
{
	return ((t_vec3){x, y, z});
}

t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}

t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});
}

t_vec3	vec3_scale(t_vec3 a, float s)
{
	return ((t_vec3){a.x * s, a.y * s, a.z * s});
}

float	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x});
}

float	vec3_length(t_vec3 a)
{
	return (sqrtf(vec3_dot(a, a)));
}

t_vec3	vec3_normalize(t_vec3 a)
{
	float	len;

	len = vec3_length(a);
	if (len > 0.0f)
		return (vec3_scale(a, 1.0f / len));
	return ((t_vec3){0.0f, 0.0f, 0.0f});
}
