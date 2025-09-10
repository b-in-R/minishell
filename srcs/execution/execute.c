/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:31:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/10 12:23:16 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Creates a copy of the arguments array with outer quotes removed.
 * This is necessary because execve needs the actual command arguments
 * without the shell quotes, but we keep the original args intact
 * for other uses (like echo which needs to preserve inner quotes).
 *
 * Example:
 *		Input:  ["echo", "'hello'", NULL]
 * 		Output: ["echo", "hello", NULL]
 *
 * Returns: A newly allocated array with cleaned arguments.
*/
static char	**create_clean_args(char **args)
{
	char	**clean_args;
	int		i;

	i = 0;
	while (args[i])
		i++;
	clean_args = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (args[i])
	{
		clean_args[i] = remove_outer_quotes(args[i]);
		i++;
	}
	clean_args[i] = NULL;
	return (clean_args);
}

void	execute_command(t_cmd *cmd, char **my_env)
{
	char	*path;
	char	**clean_args;

	clean_args = create_clean_args(cmd->args);
	path = find_command_path(my_env, clean_args[0]);
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

void	only_builtin(t_cmd *cmd, t_expander *exp, t_fork *data)
{
	int	save_in;
	int	save_out;
	int	save_err;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	save_err = dup(STDERR_FILENO);
	if (save_in == -1 || save_out == -1 || save_err == -1)
		error_exit(exp->my_env, "only_builtin: dup save std fds");
	data->fd[0] = -1;
	data->fd[1] = -1;
	set_redirection(exp->my_env, cmd, 0, data->fd);
	data->status = execute_builtin(cmd, exp);
	if (dup2(save_in, STDIN_FILENO) == -1
		|| dup2(save_out, STDOUT_FILENO) == -1
		|| dup2(save_err, STDERR_FILENO) == -1)
		error_exit(exp->my_env, "only_builtin: dup2 restore std fds");
	close(save_in);
	close(save_out);
	close(save_err);
	exp->last_status = data->status;
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
		only_builtin(cmd, exp, &data);
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
	exp->last_status = data.last_status;
	free(data.pid);
}
