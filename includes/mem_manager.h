/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_manager.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 22:39:40 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/28 19:56:11 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEM_MANAGER_H
# define MEM_MANAGER_H

# include "includes.h"
# include "../libft/libft.h"

// Struct suivi d'allocations
typedef struct s_memnode
{
	void				*ptr;
	struct s_memnode	*next;
}	t_memnode;

// Struct suivi de fd
typedef struct s_fdnode
{
	int				fd;
	struct s_fdnode	*next;
}	t_fdnode;

// Struct head allocs et fd
typedef struct s_pool
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
char	*pool_strdup(t_pool *p, const char *s);
char	*pool_substr(t_pool *p, const char *s, unsigned int start, size_t len);
char	*pool_strjoin(t_pool *p, const char *s1, const char *s2);

// fd_manager.c
int		pool_track_fd(t_pool *p, int fd);
int		pool_close_one(t_pool *p, int fd);
int		pool_open(t_pool *p, const char *path, int flags, int mode);
int		pool_open_coe(t_pool *p, const char *path, int flags, int mode);
void	pool_close_all(t_pool *p);
void	pool_detach_fd_node(t_pool *p, t_fdnode *prev, t_fdnode *cur);

#endif
