/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:31:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/07/29 14:30:31 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_cmds(t_cmd *cmd)
{
	int	count;
	
	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

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
	printf(BLUE"execve ok\n"RST);
	execve(path, cmd->args, my_env);
	ft_putstr_fd(RED"minishell: execve failed\n"RST, 2);
	exit(126);
}

//////////////////////////////////////////////////////

void	execute(t_cmd *cmds, t_expander *exp)
{
	t_cmd	*cmd;
	int		fd[2];
	int		in_fd = 0;
	pid_t	*pid;
	int		i = 0;
	int		j = 0;
	int		status = 0;
	int		last_status = 0;

	cmd = cmds;
	
	pid = malloc(sizeof(pid_t) * count_cmds(cmd));

	if (!pid)
		error_exit(exp->my_env, "execute: malloc pid fail\n");

	// si 1 cmd & is_buitin
	if (!cmd->next && is_builtin(cmd))
	{
		fd[0] = -1;
		fd[1] = -1;
		setup_redirections(exp->my_env, cmd, in_fd, fd);
		status = execute_builtin(cmd,exp);
		//return (status);
	}

	// si + d'1 cmd
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			error_exit(exp->my_env, "execute: pipe");
		
		pid[i] = fork();
		if (pid[i] == -1)
			error_exit(exp->my_env, "fork");
		
		if (pid[i] == 0)
		{
			setup_redirections(exp->my_env, cmd, in_fd, fd);
			if (is_builtin(cmd))
			{
				int	status = execute_builtin(cmd, exp);
				exit(status);
			}
			else
				execute_command(cmd, exp->my_env);
		}

		else
		{
			cleanup_parent(cmd, in_fd, fd);
			cmd = cmd->next;
			i++;
		}
	}
	while (j < i)
	{
		if (pid[j] > 0)
		{
			waitpid(pid[j], &status, 0);
			if (j == i - 1)// attendre tous les process enfants
			{
				if (WIFEXITED(status))// si le pross s'est termine avec exit
					last_status = WEXITSTATUS(status);// renvoi signal
				else if (WIFSIGNALED(status))// si le pross a ete tue par un signal (SIGINT par ex)
					last_status = 128 + WTERMSIG(status);// renvoi 128 + signal
			}
		}
		j++;
	}
	
	//
	printf(YELL"last_status:\t%i\n"RST, last_status);
	//
	free(pid);
	//return (last_status);// voir pour transmettre info last_status
}

///////////////////////////////////////////////////////////////////////////

/*
void	initialise_data(t_fork *data, t_cmd *cmd)
{
	data->fd[0] = 0;
	data->fd[1] = 0;
	data->in_fd = 0;
	data->pid = malloc(sizeof(pid_t) * count_cmds(cmd));
	data->status = 0;
	data->last_status = 0;
}

void	only_builtin(t_cmd *cmd, t_expander *exp, t_fork *data)
{
	data->fd[0] = -1;
	data->fd[1] = -1;
	setup_redirections(exp->my_env, cmd, 0, data->fd);
	data->status = execute_builtin(cmd, exp);
	//return (status);// a voir comment return status
}


void	execute_bis(t_cmd **cmd, t_expander *exp, t_fork *data, int *i)
{
	if ((*cmd)->next && pipe(data->fd) == -1)
		error_exit(exp->my_env, "execute_bis: pipe");
	data->pid[*i] = fork();		
	if (data->pid[*i] == -1)
		error_exit(exp->my_env, "execute bis: fork");
		
	if (data->pid[*i] == 0)
	{
		setup_redirections(exp->my_env, *cmd, data->in_fd, data->fd);
		if (is_builtin(*cmd))
		{
			data->status = execute_builtin(*cmd, exp);
			exit(data->status);
		}
		else
			execute_command(*cmd, exp->my_env);
	}
	else
	{
		cleanup_parent(*cmd, data);
		*cmd = (*cmd)->next;
		i++;
	}
}


void	take_exit_code(int *i, int *j, t_fork *data)
{
	int	stat;

	stat = data->status;
	while (*j < *i)
	{
		if (data->pid[*j] > 0)
		{
			waitpid(data->pid[*j], &stat, 0);
			if (*j == *i - 1)
			{
				if (WIFEXITED(data->status))
					data->last_status = WEXITSTATUS(data->status);
				else if (WIFSIGNALED(data->status))
					data->last_status = 128 + WTERMSIG(data->status);
			}
		}
		j++;
	}
}

int	execute(t_cmd *cmd, t_expander *exp)
{
	t_fork	data;
	int		i;
	int		j;

	initialise_data(&data, cmd);
	i = 0;
	j = 0;
	if (!data.pid)
		error_exit(exp->my_env, "execute: malloc pid fail\n");

	//
	printf(RED"i avant execute_bis: %i\n"RST, i);
	printf(RED"j avant take_exit_code: %i\n"RST, j);
	//
		
	if (!cmd->next && is_builtin(cmd))
		only_builtin(cmd, exp, &data);// voir return status

	while (cmd)
		execute_bis(&cmd, exp, &data, &i);
	
	//
	printf(RED"i apres execute_bis: %i\n"RST, i);
	printf(RED"j apres take_exit_code: %i\n"RST, j);
	//
	
	take_exit_code(&i, &j, &data);
	
	//
	printf(YELL"last_status:\t%i\n"RST, data.last_status);
	//
	
	free(data.pid);
	return (data.last_status);
}
*/