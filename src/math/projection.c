/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Camera matrices: world -> eye (look-at), eye -> clip (perspective) */

/* Right-handed eye space looking down -Z, depth to [-1, 1] as in OpenGL */
t_mat4	mat4_perspective(float fovy, float aspect, float near_p, float far_p)
{
	t_mat4	r;
	float	f;

	memset(&r, 0, sizeof(r));
	f = 1.0f / tanf(fovy * 0.5f);
	r.m[0] = f / aspect;
	r.m[5] = f;
	r.m[10] = (far_p + near_p) / (near_p - far_p);
	r.m[11] = -1.0f;
	r.m[14] = (2.0f * far_p * near_p) / (near_p - far_p);
	return (r);
}

t_mat4	mat4_look_at(t_vec3 eye, t_vec3 target, t_vec3 up)
{
	t_mat4	r;
	t_vec3	f;
	t_vec3	s;
	t_vec3	u;

	f = vec3_normalize(vec3_sub(target, eye));
	s = vec3_normalize(vec3_cross(f, up));
	u = vec3_cross(s, f);
	r = mat4_identity();
	r.m[0] = s.x;
	r.m[4] = s.y;
	r.m[8] = s.z;
	r.m[1] = u.x;
	r.m[5] = u.y;
	r.m[9] = u.z;
	r.m[2] = -f.x;
	r.m[6] = -f.y;
	r.m[10] = -f.z;
	r.m[12] = -vec3_dot(s, eye);
	r.m[13] = -vec3_dot(u, eye);
	r.m[14] = vec3_dot(f, eye);
	return (r);
}
