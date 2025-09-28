/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_manager_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 08:01:45 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/28 19:27:54 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mem_manager.h"

// Initialises the allocation/FD tracking lists inside the pool.
void	pool_init(t_pool *p)
{
	if (p == NULL)
		return ;
	p->mhead = NULL;
	p->fhead = NULL;
}

// closes tracked fds then clears allocations.
void	pool_cleanup(t_pool *p)
{
	if (p == NULL)
		return ;
	pool_close_all(p);
	pool_clear(p);
}

// Updates links when removing a fd node from the pool list.
void	pool_detach_fd_node(t_pool *p, t_fdnode *prev, t_fdnode *cur)
{
	if (p == NULL || cur == NULL)
		return ;
	if (prev != NULL)
		prev->next = cur->next;
	else
		p->fhead = cur->next;
}
