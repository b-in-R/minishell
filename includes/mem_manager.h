/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_manager.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 22:39:40 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/20 19:10:52 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEM_MANAGER_H
# define MEM_MANAGER_H

# include <stddef.h>// c'est  pour quoi
# include <sys/types.h>// c'est pour quoi

// Struct suivi de malloc - free et fds
typedef struct	s_memnode
{
	void				*ptr;
	struct s_memnode	*next;
}	t_memnode;

// Struct suivi de fd
typedef struct	s_fdnode
{
	int				fd;
	struct s_fdnode	*next;
}	t_fdnode;

// Struct head malloc - free et fd
typedef struct	s_pool
{
	t_memnode	*mhead;
	t_fdnode	*fhead;
}	t_pool;

// mem_manager.c
void	pool_init(t_pool *p);
void	*pool_alloc(t_pool *p, size_t n);
int		pool_track(t_pool *p, void *ptr);
void	pool_free_one(t_pool *p, void *ptr);
void	pool_clear(t_pool *p);
void	pool_cleanup(t_pool *p);
char	*pool_strdup_sys(t_pool *p, const char *s);
char	*pool_substr_sys(t_pool *p, const char *s, unsigned int start, size_t len);
char	*pool_strjoin_sys(t_pool *p, const char *s1, const char *s2);
void	pool_detach_fd_node(t_pool *p, t_fdnode *prev, t_fdnode *cur);

# define pool_alloc_ctx(pool, size) pool_alloc((pool), (size))
# define pool_track_ctx(pool, ptr) pool_track((pool), (ptr))
# define pool_free_ctx(pool, ptr) pool_free_one((pool), (ptr))
# define pool_open_ctx(pool, path, flags, mode) \
	pool_open((pool), (path), (flags), (mode))
# define pool_open_coe_ctx(pool, path, flags, mode) \
	pool_open_coe((pool), (path), (flags), (mode))
# define pool_close_ctx(pool, fd) pool_close_one((pool), (fd))
# define pool_close_all_ctx(pool) pool_close_all((pool))
# define pool_cleanup_ctx(pool) pool_cleanup((pool))
# define pool_strdup_ctx(pool, str) pool_strdup_sys((pool), (str))
# define pool_substr_ctx(pool, str, start, len) \
	pool_substr_sys((pool), (str), (start), (len))
# define pool_strjoin_ctx(pool, s1, s2) \
	pool_strjoin_sys((pool), (s1), (s2))

// fd_manager.c
int		pool_track_fd(t_pool *p, int fd);
int		pool_close_one(t_pool *p, int fd);
int		pool_open(t_pool *p, const char *path, int flags, int mode);
int		pool_open_coe(t_pool *p, const char *path, int flags, int mode);
void	pool_close_all(t_pool *p);

#endif
