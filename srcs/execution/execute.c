/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:31:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/07/15 15:39:09 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
void	execute_command(t_cmd *cmd, char **my_env)
{
	char	*path;

	path = find_command_path(my_env, cmd->args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd("\n", 2);
		exit(127);
	}
	execve(path, cmd->args, my_env);
	ft_putstr_fd("minishell: execve failed\n", 2);
	exit(126);
}

int	execute(t_cmd *cmds, char **av, t_expander *exp)
{
	t_cmd	*cmd;
	int		fd[2];
	int		in_fd = 0;
	pid_t	pid;

	cmd = cmds;
	(void)av;
	//if (av[1] && av[1][0] == 'd')
	//	print_cmds(cmds);

	/*
		setup_redirections:
			si pas de fork, mais redirections quand meme:
			voir pour sauvegarge et restauration de stdin et stdout.
			Si on ne restaure pas la sortie standard apres l'exec,
			toutes les prochaines commandes ecriront dans le fichier
			en question plutot que le terminal
			dans un fork pas necessaire vu que le process meurt a la
			fin de l'exec
	*/

	// si qu'1 cmd & is_builtin ok: pas de fork pour l'exec -- ?
	if (!cmd->next && is_builtin(cmd))
	{

		//test
	//	printf(YELL"just builtin - no fork\n"RST);

		fd[0] = -1;
		fd[1] = -1;
		setup_redirections(exp->my_env, cmd, in_fd, fd);
		execute_builtin(cmd, exp);
		return (1);
	}

	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)// si il y a encore une cmd & pipe/erreur
			error_exit(exp->my_env, "pipe");

		pid = fork();	/*-------------- FORK --------------*/
		if (pid == -1)
			error_exit(exp->my_env, "fork");

		if (pid == 0)// si OK
		{
			//
			setup_redirections(exp->my_env, cmd, in_fd, fd);
			if (is_builtin(cmd))
				execute_builtin(cmd, exp);
			else
				execute_command(cmd, exp->my_env);
		}
		else // autre erreur fork
		{
			cleanup_parent(cmd, &in_fd, fd);
			if (cmd->next)
			{
				close(fd[1]);
				in_fd = fd[0];
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
	return (1);
}

