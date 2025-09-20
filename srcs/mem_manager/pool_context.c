/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool_context.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:10:52 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/19 16:10:52 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mem_manager.h"

static t_pool	*g_pool_ctx = NULL;

// Stores the pool pointer used by the *_ctx convenience macros.
void	pool_set_context(t_pool *pool)
{
	g_pool_ctx = pool;
}

// Retrieves the current context pool (may be NULL if unset).
t_pool	*pool_get_context(void)
{
	return (g_pool_ctx);
}

// Applies pool_cleanup() on the globally stored context.
void	pool_cleanup_ctx(void)
{
	pool_cleanup(pool_get_context());
}
