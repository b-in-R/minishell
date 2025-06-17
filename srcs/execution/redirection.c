/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:19:42 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/17 17:00:38 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// configure stdin stdout avant execution d'une commande selon pipe ou cmd unique
void	setup_redirections(t_cmd *cmd, int in_fd, int pipe_fd[2])
{
	int	fd_in;
	int	flags;
	int	fd_out;
	
	// redirection d'entree (par defaut in_fd = 0)
	if (cmd->infile)
	{
		fd_in = open(cmd->infile, O_RDONLY);
		if (fd_in < 0)
			error_exit("setup_redirections: fd < 0");
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	else if (in_fd != 0) // si !fichier mais pipe
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	
	// redirecition de sortie, si la cmd a "> fichier" ou "">> fichier"
	if (cmd->outfile)
	{
		flags = O_CREAT | O_WRONLY;
		if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd_out = open(cmd->outfile, flags, 0644);
		if (fd_out < 0)
			error_exit("setup_redirections: fd_out < 0");
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	else if (cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);

	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

