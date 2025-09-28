/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:55:28 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/27 20:41:40 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>

/*
** Ferme le FD du pipe, nettoie le pool et termine le fils avec le statut donné.
*/
void	heredoc_child_exit(t_expander *exp, int write_fd, int status)
{
	pool_close_one(exp->pool, write_fd);
	pool_cleanup(exp->pool);
	exit(status);
}

/*
** Assure l'écriture totale d'un tampon en gérant les interruptions système.
*/
int	write_all(int fd, const char *buf, size_t len)
{
	size_t	total;
	ssize_t	written;

	total = 0;
	while (total < len)
	{
		written = write(fd, buf + total, len - total);
		if (written == -1)
		{
			if (errno == EINTR)
				continue ;
			return (0);
		}
		total += (size_t)written;
	}
	return (1);
}

/*
** Reconstitue une ligne en expandant les variables $ tout préservant le reste
*/
char	*expand_heredoc_line(const char *line, t_expander *exp)
{
	char		*expanded;
	int			i;
	t_expand	expand;

	i = 0;
	expanded = pool_strdup(exp->pool, "");
	if (!expanded)
		return (NULL);
	expand.pool = exp->pool;
	expand.exp = exp;
	expand.result = &expanded;
	while (line[i])
	{
		if (line[i] == '$')
		{
			if (!handle_dollar(&expand, line, &i))
				return (pool_free_one(exp->pool, expanded), NULL);
		}
		else if (!append_char(exp->pool, &expanded, line[i++]))
			return (pool_free_one(exp->pool, expanded), NULL);
	}
	return (expanded);
}

/*
** En cas d'échec du pool_track, libère la ligne et quitte proprement le fils.
*/
void	handle_pool_track_failure(char *line, t_expander *exp, int write_fd)
{
	if (line)
		free(line);
	perror("minishell: heredoc track");
	heredoc_child_exit(exp, write_fd, 1);
}

/*
** Écrit la ligne (éventuellement expansée) dans le pipe en signalant tout échec.
*/
int	push_line(int write_fd, char *line, t_expander *exp, t_cmd *cmd)
{
	char	*expanded;
	int		ok;

	ok = 1;
	if (cmd->expand_heredoc)
		expanded = expand_heredoc_line(line, exp);
	else
		expanded = pool_strdup(exp->pool, line);
	if (!expanded)
	{
		ft_putstr_fd("minishell: heredoc: allocation failed\n", 2);
		return (0);
	}
	if (!write_all(write_fd, expanded, ft_strlen(expanded))
		|| !write_all(write_fd, "\n", 1))
	{
		perror("minishell: heredoc write");
		ok = 0;
	}
	pool_free_one(exp->pool, expanded);
	return (ok);
}
