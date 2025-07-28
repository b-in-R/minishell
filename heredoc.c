/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:17:29 by albertooutu       #+#    #+#             */
/*   Updated: 2025/07/08 16:57:26 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Child process - reads heredoc input
void	heredoc_child_process(int pipe_fd[2], char *delimiter)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (!line)
			exit(0);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			exit(0);
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	exit(0);
}

/* Parent process - handles child and sets up file descriptor
* Close write end in parent, cmd->in_fd = -1; // Invalidate fd
* cmd->in_fd = pipe_fd[0]; // Set read fd
*/
int	heredoc_parent_process(pid_t pid, int pipe_fd[2], t_cmd *cmd)
{
	int	status;

	waitpid(pid, &status, 0);
	close(pipe_fd[1]);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(1, "\n", 1);
		close(pipe_fd[0]);
		cmd->in_fd = -1;
		return (0);
	}
	cmd->in_fd = pipe_fd[0];
	return (1);
}

// Process single heredoc
int	process_single_heredoc(t_cmd *cmd)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 0);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 0);
	if (pid == 0)
	{
		heredoc_child_process(pipe_fd, cmd->delimiter);
	}
	else
	{
		return (heredoc_parent_process(pid, pipe_fd, cmd));
	}
	return (1);
}

// Main function - iterate through commands
/*
*	Lit l'entrée du user ligne par ligne jusqu'à que le délimiteur soit atteint
* 	Gére les quotes dans le délimiteur (ex : "<< 'EOF'")
*   Gere les signaux en cas dinterruption (Ctrl-C) pendant lecture de l'heredoc
*   Appel par le main avant l'exécution des commandes et aprés le parsing.
*	Parcours chaque t_cmd dans ta liste.
*	Si détecte que cmd->heredoc == 1, alors on:
*		- Lit l’entrée utilisateur ligne par ligne jusqu’au délimiteur.
*		- On stockes ça dans un pipe temporaire.
*		- On ouvre ce pipe/fichier en lecture, et on mets son fd dans cmd->in_fd.
*	Comem ca lors de l’exécution (execute()), le shell saura que l’entrée (stdin)
*	de cette commande ne doit pas venir du clavier, mais de ce contenu temporaire
*/
int	handle_heredocs(t_cmd *cmds)
{
	while (cmds)
	{
		if (cmds->heredoc && cmds->delimiter)
		{
			if (!process_single_heredoc(cmds))
				return (0);
		}
		cmds = cmds->next;
	}
	return (1);
}
