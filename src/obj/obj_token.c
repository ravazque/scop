/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Line helpers of the .obj parser: tokens, errors and joined lines */

static int	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f');
}

/* Next blank-separated token, NUL-terminated in place; NULL at the end */
char	*obj_token(char **cursor)
{
	char	*s;
	char	*start;

	s = *cursor;
	while (is_blank(*s))
		s++;
	if (*s == '\0')
	{
		*cursor = s;
		return (NULL);
	}
	start = s;
	while (*s && !is_blank(*s))
		s++;
	if (*s)
		*s++ = '\0';
	*cursor = s;
	return (start);
}

/* 0, after "Error: path:line: message 'token'" (no line 0, no NULL token) */
int	obj_error(const t_obj_parser *p, size_t line, const char *message,
		const char *token)
{
	fprintf(stderr, "Error: %s", p->path);
	if (line)
		fprintf(stderr, ":%zu", line);
	fprintf(stderr, ": %s", message);
	if (token)
		fprintf(stderr, " '%s'", token);
	fprintf(stderr, "\n");
	return (0);
}

/* A backslash at the end of a line joins it with the next one */
void	obj_join_lines(char *s)
{
	while (*s)
	{
		if (s[0] == '\\' && s[1] == '\n')
			memset(s, ' ', 2);
		else if (s[0] == '\\' && s[1] == '\r' && s[2] == '\n')
			memset(s, ' ', 3);
		s++;
	}
}
