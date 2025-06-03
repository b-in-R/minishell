/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:31:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/03 18:47:50 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/*
	input expected:

		terminal entry:		ls -l | grep hello > out.txt
			
		struct:		t_cmd *cmd1;
						cmd1->args = {"ls", "-l", NULL};
						cmd1->infile = NULL;
						cmd1->outfile = NULL;
						cmd1->append = 0;
						cmd1->heredoc = 0;
						cmd1->delimiter = NULL;
						cmd1->next = cmd2;

					t_cmd *cmd2;
						cmd2->args = {"grep", "hello", NULL};
						cmd2->infile = NULL; (fd 0 pour stdin)
						cmd2->outfile = "out.txt";
						cmd2->apprend = 0;
						cmd2->heredoc = 0;
						cmd2->delimiter = NULL;
						cmd2->next = NULL;
*/



void	execute_command(t_cmd *cmd)
{
	char	*path;
	
	path = find_command_path(cmd->args[0]);
	if (!path)
		error_exit("execute_command: command not fould\n");// texte a voir selon bash?
	execve(path, cmd->args, NULL);
	error_exit("execute_command: execve failure\n");
}

void	execute(t_cmd *cmds)
{
	t_cmd	*cmd;
	int		fd[2];
	int		in_fd = 0;
	pid_t	pid;

	cmd = cmds;
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			error_exit("pipe");
		
		pid = fork();
		if (pid == -1)
			error_exit("fork");
			
		if (pid == 0)
		{
			setup_redirections(cmd, in_fd, fd);
			if (is_builtin(cmd))
				execute_builtin(cmd);
			else
				execute_command(cmd);				
		}
		else
		{
			cleanup_parent(cmd, &in_fd, fd);
			if (cmd->next)
			{
				close(fd[1]);
				in_fd =  fd[0];
			}
			else
			{
				close(fd[0]);
				close(fd[1]);
			}
			waitpid(pid, NULL, 0);
			cmd = cmd->next;
		}
	}
}
