#include "scop.h"

static int	emit(t_obj *obj, const unsigned int *abc, unsigned int id)
{
	const t_vec3	ab = vec3_sub(obj->positions[abc[1]], obj->positions[abc[0]]);
	const t_vec3	ac = vec3_sub(obj->positions[abc[2]], obj->positions[abc[0]]);
	t_obj_triangle	*grown;

	if (!(vec3_length(vec3_cross(ab, ac)) > obj->min_area))
		return (1);
	grown = array_reserve(obj->triangles, &obj->triangle_cap, obj->triangle_count + 1, sizeof(t_obj_triangle));
	if (!grown)
		return (0);
	obj->triangles = grown;
	memcpy(obj->triangles[obj->triangle_count].corner, abc, 3 * sizeof(unsigned int));
	obj->triangles[obj->triangle_count++].face = id;
	return (1);
}

static int	fan(t_obj *obj, const t_obj_parser *p, size_t count, unsigned int id)
{
	unsigned int	abc[3];
	size_t			k;

	for (k = 1; k + 1 < count; k++)
	{
		abc[0] = p->polygon[p->ring[0]];
		abc[1] = p->polygon[p->ring[k]];
		abc[2] = p->polygon[p->ring[k + 1]];
		if (!emit(obj, abc, id))
			return (0);
	}
	return (1);
}

static t_vec3	newell_normal(const t_obj *obj, const unsigned int *poly, size_t n)
{
	t_vec3	sum;
	t_vec3	a;
	t_vec3	b;
	size_t	i;

	sum = vec3(0.0f, 0.0f, 0.0f);
	for (i = 0; i < n; i++)
	{
		a = obj->positions[poly[i]];
		b = obj->positions[poly[(i + 1) % n]];
		sum.x += (a.y - b.y) * (a.z + b.z);
		sum.y += (a.z - b.z) * (a.x + b.x);
		sum.z += (a.x - b.x) * (a.y + b.y);
	}
	return (sum);
}

static float	coordinate(t_vec3 a, int axis)
{
	if (axis == 0)
		return (a.x);
	if (axis == 1)
		return (a.y);
	return (a.z);
}

/* Drops the normal's dominant axis; u and v swap when it points away, so outlines turn counter-clockwise */
static void	project(const t_obj *obj, t_obj_parser *p, size_t n, t_vec3 normal)
{
	int		drop;
	int		u;
	int		v;
	size_t	i;

	drop = 2;
	if (fabsf(normal.x) >= fabsf(normal.y) && fabsf(normal.x) >= fabsf(normal.z))
		drop = 0;
	else if (fabsf(normal.y) >= fabsf(normal.z))
		drop = 1;
	u = (drop + 1) % 3;
	v = (drop + 2) % 3;
	if (coordinate(normal, drop) < 0.0f)
	{
		u = v;
		v = (drop + 1) % 3;
	}
	for (i = 0; i < n; i++)
	{
		p->flat[2 * i] = coordinate(obj->positions[p->polygon[i]], u);
		p->flat[2 * i + 1] = coordinate(obj->positions[p->polygon[i]], v);
	}
}

static float	turn(const float *a, const float *b, const float *c)
{
	return ((b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]));
}

static int	same_point(const float *a, const float *b)
{
	return (a[0] == b[0] && a[1] == b[1]);
}

/* An ear is a convex corner whose triangle has no other corner inside or on it */
static int	is_ear(const t_obj_parser *p, size_t count, size_t i)
{
	const float	*a = p->flat + 2 * p->ring[(i + count - 1) % count];
	const float	*b = p->flat + 2 * p->ring[i];
	const float	*c = p->flat + 2 * p->ring[(i + 1) % count];
	const float	*q;
	size_t		k;

	if (turn(a, b, c) <= 0.0f)
		return (0);
	for (k = 0; k < count; k++)
	{
		q = p->flat + 2 * p->ring[k];
		if (same_point(q, a) || same_point(q, b) || same_point(q, c))
			continue ;
		if (turn(a, b, q) >= 0.0f && turn(b, c, q) >= 0.0f && turn(c, a, q) >= 0.0f)
			return (0);
	}
	return (1);
}

static int	ear_clip(t_obj *obj, t_obj_parser *p, size_t count, unsigned int id)
{
	unsigned int	abc[3];
	size_t			i;
	size_t			misses;

	i = 0;
	misses = 0;
	while (count > 3 && misses < count)
	{
		misses++;
		if (is_ear(p, count, i))
		{
			abc[0] = p->polygon[p->ring[(i + count - 1) % count]];
			abc[1] = p->polygon[p->ring[i]];
			abc[2] = p->polygon[p->ring[(i + 1) % count]];
			if (!emit(obj, abc, id))
				return (0);
			memmove(p->ring + i, p->ring + i + 1, (count - i - 1) * sizeof(size_t));
			count--;
			misses = 0;
		}
		i = (i + 1) % count;
	}
	return (fan(obj, p, count, id));
}

int	obj_triangulate(t_obj *obj, t_obj_parser *p, size_t n, unsigned int id)
{
	size_t	*ring;
	float	*flat;
	t_vec3	normal;
	size_t	i;

	if (n == 3)
		return (emit(obj, p->polygon, id));
	ring = array_reserve(p->ring, &p->ring_cap, n, sizeof(size_t));
	if (!ring)
		return (0);
	p->ring = ring;
	flat = array_reserve(p->flat, &p->flat_cap, 2 * n, sizeof(float));
	if (!flat)
		return (0);
	p->flat = flat;
	for (i = 0; i < n; i++)
		p->ring[i] = i;
	normal = newell_normal(obj, p->polygon, n);
	if (!(vec3_length(normal) > obj->min_area))
		return (fan(obj, p, n, id));
	project(obj, p, n, normal);
	return (ear_clip(obj, p, n, id));
}
