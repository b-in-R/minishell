/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool_strings.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:55:02 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/27 20:54:33 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "../../libft/libft.h"
#include "../../includes/mem_manager.h"

// Creates a substring using pool-managed memory.
char	*pool_substr(t_pool *p, const char *s, unsigned int start,
			size_t len)
{
	size_t	size;
	char	*sub;

	if (p == NULL || s == NULL)
		return (NULL);
	size = ft_strlen(s);
	if (start >= size)
		len = 0;
	else if (start + len > size)
		len = size - start;
	sub = pool_alloc(p, len + 1);
	if (sub == NULL)
		return (NULL);
	if (len > 0)
		ft_memcpy(sub, s + start, len);
	sub[len] = '\0';
	return (sub);
}

// Concatenates two strings into a new pool-tracked buffer.
char	*pool_strjoin(t_pool *p, const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*join;

	if (p == NULL)
		return (NULL);
	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	join = pool_alloc(p, len1 + len2 + 1);
	if (join == NULL)
		return (NULL);
	if (len1 > 0)
		ft_memcpy(join, s1, len1);
	if (len2 > 0)
		ft_memcpy(join + len1, s2, len2);
	join[len1 + len2] = '\0';
	return (join);
}
