/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 22:11:59 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/20 08:05:14 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../libft/libft.h"
#include "../../includes/mem_manager.h"

// Allocates `n` bytes and registers the pointer in the pool list.
void	*pool_alloc(t_pool *p, size_t n)
{
	void		*mem;
	t_memnode	*node;

	mem = NULL;
	node = NULL;
	if (p == NULL || n == 0)
		return (NULL);
	mem = malloc(n);
	if (mem == NULL)
		return (NULL);
	node = (t_memnode *)malloc(sizeof(t_memnode));
	if (node == NULL)
	{
		free(mem);
		return (NULL);
	}
	node->ptr = mem;
	node->next = p->mhead;
	p->mhead = node;
	return (mem);
}

// Adds an externally allocated pointer to the pool tracking.
int	pool_track(t_pool *p, void *ptr)
{
	t_memnode	*node;

	node = NULL;
	if (p == NULL || ptr == NULL)
		return (0);
	node = (t_memnode *)malloc(sizeof(t_memnode));
	if (node == NULL)
		return (0);
	node->ptr = ptr;
	node->next = p->mhead;
	p->mhead = node;
	return (1);
}

// Frees a specific tracked pointer and removes it from the list.
void	pool_free_one(t_pool *p, void *ptr)
{
	t_memnode	*cur;
	t_memnode	*prev;

	cur = NULL;
	prev = NULL;
	if (p == NULL || ptr == NULL)
		return ;
	cur = p->mhead;
	while (cur != NULL)
	{
		if (cur->ptr == ptr)
		{
			if (prev != NULL)
				prev->next = cur->next;
			else
				p->mhead = cur->next;
			free(cur->ptr);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

// Frees all pointers tracked by the pool.
void	pool_clear(t_pool *p)
{
	t_memnode	*cur;
	t_memnode	*next;

	cur = NULL;
	next = NULL;
	if (p == NULL)
		return ;
	cur = p->mhead;
	while (cur != NULL)
	{
		next = cur->next;
		free(cur->ptr);
		free(cur);
		cur = next;
	}
	p->mhead = NULL;
}

// Duplicates a C-string while recording it in the given pool.
char	*pool_strdup_sys(t_pool *p, const char *s)
{
	size_t	len;
	char	*dup;

	if (p == NULL || s == NULL)
		return (NULL);
	len = ft_strlen(s) + 1;
	dup = (char *)pool_alloc(p, len);
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, s, len);
	return (dup);
}
