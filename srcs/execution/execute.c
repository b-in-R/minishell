/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:31:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/28 17:17:30 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>

// Resolves the executable path then calls execve or exits on failure.
void	execute_command(t_cmd *cmd, t_expander *exp)
{
	char	*path;
	char	**clean_args;
	char	**my_env;

	if (!cmd->args || !cmd->args[0])
		error_exit(exp->pool, exp->my_env,
			"execute_command: missing command");
	clean_args = create_clean_args(exp->pool, cmd->args);
	if (!clean_args)
		error_exit(exp->pool, exp->my_env,
			"execute_command: create_clean_args");
	my_env = exp->my_env;
	path = find_command_path(exp, clean_args[0]);
	if (!path)
		command_not_found_exit(exp, clean_args[0]);
	if (execve(path, clean_args, my_env) == -1)
		exec_failure_exit(exp, clean_args[0]);
}

// Runs a builtin without forking by saving/restoring stdio.
void	only_builtin(t_cmd *cmd, t_expander *exp, t_fork *data)
{
	int	save_in;
	int	save_out;
	int	save_err;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	save_err = dup(STDERR_FILENO);
	if (save_in == -1 || save_out == -1 || save_err == -1)
		error_exit(exp->pool, exp->my_env, "only_builtin: dup fds");
	if (!pool_track_fd(exp->pool, save_in)
		|| !pool_track_fd(exp->pool, save_out)
		|| !pool_track_fd(exp->pool, save_err))
		error_exit(exp->pool, exp->my_env, "only_builtin: track fds");
	ft_memset(data->fd, -1, sizeof(data->fd));
	set_redirection(exp, cmd, 0, data->fd);
	data->status = execute_builtin(cmd, exp);
	if (dup2(save_in, STDIN_FILENO) == -1
		|| dup2(save_out, STDOUT_FILENO) == -1
		|| dup2(save_err, STDERR_FILENO) == -1)
		error_exit(exp->pool, exp->my_env, "only_builtin: restore fds");
	pool_close_one(exp->pool, save_in);
	pool_close_one(exp->pool, save_out);
	pool_close_one(exp->pool, save_err);
	exp->last_status = data->status;
}

// Child-side execution: set redirections then exec or run builtin.
void	execute_bis_2(t_cmd **cmd, t_expander *exp, t_fork *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	set_redirection(exp, *cmd, data->in_fd, data->fd);
	if (data->fd[0] != -1)
		pool_close_one(exp->pool, data->fd[0]);
	if (data->fd[1] != -1)
		pool_close_one(exp->pool, data->fd[1]);
	if (data->in_fd != 0)
		pool_close_one(exp->pool, data->in_fd);
	if (!(*cmd)->args || !(*cmd)->args[0])
	{
		pool_cleanup(exp->pool);
		exit(0);
	}
	if (is_builtin(*cmd))
	{
		data->status = execute_builtin(*cmd, exp);
		pool_cleanup(exp->pool);
		exit(data->status);
	}
	execute_command(*cmd, exp);
	error_exit(exp->pool, exp->my_env, "execute_bis: execve fail");
}

// Handles one pipeline stage: prepare pipe, fork, and advance.
void	execute_bis(t_cmd **cmd, t_expander *exp, t_fork *data, int *i)
{
	if ((*cmd)->next)
	{
		if (pipe(data->fd) == -1)
			error_exit(exp->pool, exp->my_env, "execute_bis: pipe");
		if (!pool_track_fd(exp->pool, data->fd[0])
			|| !pool_track_fd(exp->pool, data->fd[1]))
			error_exit(exp->pool, exp->my_env,
				"execute_bis: track pipe");
	}
	data->pid[*i] = fork();
	if (data->pid[*i] == -1)
		error_exit(exp->pool, exp->my_env, "execute_bis: fork");
	if (data->pid[*i] == 0)
		execute_bis_2(cmd, exp, data);
	else
	{
		cleanup_parent(exp->pool, *cmd, &data->in_fd, data->fd);
		*cmd = (*cmd)->next;
		(*i)++;
	}
}

// Orchestrates the full command pipeline, updating last status.
void	execute(t_cmd *cmd, t_expander *exp)
{
	t_fork	data;
	int		i;
	int		j;

	i = 0;
	j = 0;
	initialise_data(&data, cmd, exp);
	if (!cmd->next && is_builtin(cmd))
	{
		only_builtin(cmd, exp, &data);
		pool_free_one(exp->pool, data.pid);
		return ;
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (cmd)
	{
		data.fd[0] = -1;
		data.fd[1] = -1;
		execute_bis(&cmd, exp, &data, &i);
	}
	take_exit_code(&i, &j, &data);
	exp->last_status = data.last_status;
	setup_signals();
	pool_free_one(exp->pool, data.pid);
}
