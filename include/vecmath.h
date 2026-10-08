#ifndef VECMATH_H
# define VECMATH_H

# define SCOP_PI	3.14159265358979323846f

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

/* Column-major, as OpenGL reads it: element (column c, row r) is m[c * 4 + r] */
typedef struct s_mat4
{
	float	m[16];
}	t_mat4;

t_vec3	vec3(float x, float y, float z);
t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_sub(t_vec3 a, t_vec3 b);
t_vec3	vec3_scale(t_vec3 a, float s);
float	vec3_dot(t_vec3 a, t_vec3 b);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
float	vec3_length(t_vec3 a);
t_vec3	vec3_normalize(t_vec3 a);
t_vec3	vec3_min(t_vec3 a, t_vec3 b);
t_vec3	vec3_max(t_vec3 a, t_vec3 b);

t_mat4	mat4_identity(void);
t_mat4	mat4_mul(t_mat4 a, t_mat4 b);
t_mat4	mat4_translation(t_vec3 t);
t_mat4	mat4_rotation_x(float radians);
t_mat4	mat4_rotation_y(float radians);
t_mat4	mat4_rotation_z(float radians);
t_mat4	mat4_perspective(float fovy, float aspect, float near_p, float far_p);
t_mat4	mat4_look_at(t_vec3 eye, t_vec3 target, t_vec3 up);

#endif
