#include "scop.h"

/* Faces to triangles with the face's winding, ear-clipped in the plane of the Newell normal: concave and non-planar too. */

/* Twice the signed area of a, b, c in the 2D projection: positive when they turn counter-clockwise. */
static float	turn(const float *a, const float *b, const float *c)
{
	return ((b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]));
}

/* Appends a triangle, unless it has no area. */
static int	emit(t_obj *obj, unsigned int a, unsigned int b, unsigned int c, unsigned int id)
{
	t_obj_triangle	*grown;
	t_vec3			n;

	n = vec3_cross(vec3_sub(obj->positions[b], obj->positions[a]), vec3_sub(obj->positions[c], obj->positions[a]));
	if (!(vec3_length(n) > obj->min_area))
		return (1);
	grown = array_reserve(obj->triangles, &obj->triangle_cap, obj->triangle_count + 1, sizeof(t_obj_triangle));
	if (!grown)
		return (0);
	obj->triangles = grown;
	obj->triangles[obj->triangle_count++] = (t_obj_triangle){{a, b, c}, id};
	return (1);
}

/* Newell's method: the polygon normal, sized by its area, robust to concave and non-planar outlines. */
static t_vec3	newell_normal(const t_obj *obj, const unsigned int *poly, size_t n)
{
	t_vec3	sum;
	t_vec3	a;
	t_vec3	b;
	size_t	i;

	sum = vec3(0.0f, 0.0f, 0.0f);
	i = 0;
	while (i < n)
	{
		a = obj->positions[poly[i]];
		b = obj->positions[poly[(i + 1) % n]];
		sum = vec3_add(sum, vec3((a.y - b.y) * (a.z + b.z), (a.z - b.z) * (a.x + b.x), (a.x - b.x) * (a.y + b.y)));
		i++;
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

/* Drops the dominant axis of the normal, mirrored when needed so the outline turns counter-clockwise. */
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
	i = 0;
	while (i < n)
	{
		p->flat[2 * i] = coordinate(obj->positions[p->polygon[i]], u);
		p->flat[2 * i + 1] = coordinate(obj->positions[p->polygon[i]], v);
		i++;
	}
}

static int	same_point(const float *a, const float *b)
{
	return (a[0] == b[0] && a[1] == b[1]);
}

/* Corner i of the ring is an ear: convex, with no other corner inside or on its triangle. */
static int	is_ear(const t_obj_parser *p, size_t count, size_t i)
{
	const float	*a = p->flat + 2 * p->ring[(i + count - 1) % count];
	const float	*b = p->flat + 2 * p->ring[i];
	const float	*c = p->flat + 2 * p->ring[(i + 1) % count];
	const float	*q;
	size_t		k;

	if (turn(a, b, c) <= 0.0f)
		return (0);
	k = 0;
	while (k < count)
	{
		q = p->flat + 2 * p->ring[k++];
		if (same_point(q, a) || same_point(q, b) || same_point(q, c))
			continue ;
		if (turn(a, b, q) >= 0.0f && turn(b, c, q) >= 0.0f && turn(c, a, q) >= 0.0f)
			return (0);
	}
	return (1);
}

/* Fan over what is left of the ring: the fallback for outlines that cross themselves. */
static int	fan(t_obj *obj, const t_obj_parser *p, size_t count, unsigned int id)
{
	size_t	k;

	k = 1;
	while (k + 1 < count)
	{
		if (!emit(obj, p->polygon[p->ring[0]], p->polygon[p->ring[k]], p->polygon[p->ring[k + 1]], id))
			return (0);
		k++;
	}
	return (1);
}

static int	ear_clip(t_obj *obj, t_obj_parser *p, size_t count, unsigned int id)
{
	size_t	i;
	size_t	misses;

	i = 0;
	misses = 0;
	while (count > 3 && misses < count)
	{
		if (!is_ear(p, count, i))
		{
			i = (i + 1) % count;
			misses++;
			continue ;
		}
		if (!emit(obj, p->polygon[p->ring[(i + count - 1) % count]], p->polygon[p->ring[i]], p->polygon[p->ring[(i + 1) % count]], id))
			return (0);
		memmove(p->ring + i, p->ring + i + 1, (count - i - 1) * sizeof(size_t));
		count--;
		i %= count;
		misses = 0;
	}
	return (fan(obj, p, count, id));
}

int	obj_triangulate(t_obj *obj, t_obj_parser *p, size_t n, unsigned int id)
{
	t_vec3	normal;
	size_t	*ring;
	float	*flat;
	size_t	i;

	if (n == 3)
		return (emit(obj, p->polygon[0], p->polygon[1], p->polygon[2], id));
	ring = array_reserve(p->ring, &p->ring_cap, n, sizeof(size_t));
	if (!ring)
		return (0);
	p->ring = ring;
	flat = array_reserve(p->flat, &p->flat_cap, 2 * n, sizeof(float));
	if (!flat)
		return (0);
	p->flat = flat;
	i = 0;
	while (i < n)
	{
		p->ring[i] = i;
		i++;
	}
	normal = newell_normal(obj, p->polygon, n);
	if (!(vec3_length(normal) > obj->min_area))
		return (fan(obj, p, n, id));
	project(obj, p, n, normal);
	return (ear_clip(obj, p, n, id));
}
