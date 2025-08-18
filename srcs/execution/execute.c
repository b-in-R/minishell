/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:31:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/18 10:43:52 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"



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
	ft_putstr_fd(RED"minishell: execve failed\n"RST, 2);
	exit(126);
}

void only_builtin(t_cmd *cmd, t_expander *exp, t_fork *data)
{
    int save_in  = dup(STDIN_FILENO);
    int save_out = dup(STDOUT_FILENO);
    int save_err = dup(STDERR_FILENO);
    if (save_in == -1 || save_out == -1 || save_err == -1)
        error_exit(exp->my_env, "only_builtin: dup save std fds");
    data->fd[0] = -1;
    data->fd[1] = -1;
    set_redirection(exp->my_env, cmd, /*in_fd=*/0, data->fd);

    // exécute dans parent, redirigé
    data->status = execute_builtin(cmd, exp);

    // restauration fd ftd
    if (dup2(save_in,  STDIN_FILENO)  == -1
    	|| dup2(save_out, STDOUT_FILENO) == -1
    	|| dup2(save_err, STDERR_FILENO) == -1)
        error_exit(exp->my_env, "only_builtin: dup2 restore std fds");

    close(save_in);
    close(save_out);
    close(save_err);
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
		set_redirection(exp->my_env, *cmd, data->in_fd, data->fd);
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
		cleanup_parent(*cmd, &data->in_fd, data->fd);
		*cmd = (*cmd)->next;
		(*i)++;
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
				if (WIFEXITED(stat))
					data->last_status = WEXITSTATUS(stat);
				else if (WIFSIGNALED(stat))
					data->last_status = 128 + WTERMSIG(stat);
			}
		}
		(*j)++;
	}
}

void	execute(t_cmd *cmd, t_expander *exp)
{
	t_fork	data;
	int		i;
	int		j;

	initialise_data(&data, cmd);
	i = 0;
	j = 0;

	if (!data.pid)
		error_exit(exp->my_env, "execute: malloc pid fail\n");
	if (!cmd->next && is_builtin(cmd))
	{
		only_builtin(cmd, exp, &data);// voir return status
		free(data.pid);
		return ;
	}
	while (cmd)
	{
		data.fd[0] = -1;
		data.fd[1] = -1;
		execute_bis(&cmd, exp, &data, &i);
	}
	take_exit_code(&i, &j, &data);

	// voir pour remplacer data.last_status par exp->last_status
	exp->last_status = data.last_status;

	free(data.pid);
}
