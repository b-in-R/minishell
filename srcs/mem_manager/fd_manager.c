/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_manager.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 21:01:50 by rabiner           #+#    #+#             */
/*   Updated: 2025/10/08 12:51:30 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mem_manager.h"

// Registers an already opened file descriptor for later cleanup.
int	pool_track_fd(t_pool *p, int fd)
{
	t_fdnode	*node;

	node = NULL;
	if (p == NULL || fd < 0)
		return (0);
	node = (t_fdnode *)malloc(sizeof(t_fdnode));
	if (node == NULL)
		return (0);
	node->fd = fd;
	node->next = p->fhead;
	p->fhead = node;
	return (1);
}

// Closes a tracked descriptor and removes it from the pool list.
int	pool_close_one(t_pool *p, int fd)
{
	t_fdnode	*cur;
	t_fdnode	*prev;
	int			rc;

	cur = NULL;
	prev = NULL;
	rc = 0;
	if (p == NULL || fd < 0)
		return (0);
	cur = p->fhead;
	while (cur != NULL)
	{
		if (cur->fd == fd)
		{
			pool_detach_fd_node(p, prev, cur);
			rc = close(cur->fd);
			free(cur);
			(void)rc;
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

// Opens a file and tracks its descriptor inside the provided pool.
int	pool_open(t_pool *p, const char *path, int flags, int mode)
{
	int	fd;
	int	ok;

	fd = -1;
	ok = 0;
	if (p == NULL || path == NULL)
		return (-1);
	fd = open(path, flags, mode);
	if (fd < 0)
		return (-1);
	ok = pool_track_fd(p, fd);
	if (!ok)
	{
		close(fd);
		return (-1);
	}
	return (fd);
}

// Opens a file, tracks it, with O_CLOEXEC.
int	pool_open_coe(t_pool *p, const char *path, int flags, int mode)
{
	int	fd;
	int	co_flags;

	co_flags = flags | O_CLOEXEC;
	fd = pool_open(p, path, co_flags, mode);
	if (fd < 0)
		return (-1);
	return (fd);
}

// Closes every descriptor tracked in the pool and clears the list.
void	pool_close_all(t_pool *p)
{
	t_fdnode	*cur;
	t_fdnode	*next;
	int			rc;

	cur = NULL;
	next = NULL;
	rc = 0;
	if (p == NULL)
		return ;
	cur = p->fhead;
	while (cur != NULL)
	{
		next = cur->next;
		rc = close(cur->fd);
		(void)rc;
		free(cur);
		cur = next;
	}
	p->fhead = NULL;
}
