/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:31:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/09 11:21:13 by rabiner          ###   ########.fr       */
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

void	only_builtin(t_cmd *cmd, t_expander *exp, t_fork *data)
{
	int	save_in;
	int	save_out;
	int	save_err;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	save_err = dup(STDERR_FILENO);
	if (save_in == -1 || save_out == -1 || save_err == -1)
		error_exit(exp->my_env, "only_builtin: dup save std fds");// texte
	data->fd[0] = -1;
	data->fd[1] = -1;
	set_redirection(exp->my_env, cmd, 0, data->fd);
	data->status = execute_builtin(cmd, exp);
	if (dup2(save_in, STDIN_FILENO) == -1
		|| dup2(save_out, STDOUT_FILENO) == -1
		|| dup2(save_err, STDERR_FILENO) == -1)
		error_exit(exp->my_env, "only_builtin: dup2 restore std fds");// texte
	close(save_in);
	close(save_out);
	close(save_err);
	exp->last_status = data->status;
}

void	execute_bis_2(t_cmd **cmd, t_expander *exp, t_fork *data)
{
		signal(SIGINT, SIG_DFL);//
		signal(SIGQUIT, SIG_DFL);//
		set_redirection(exp->my_env, *cmd, data->in_fd, data->fd);
		if (data->fd[0] != -1)//
			close(data->fd[0]);//
		if (data->fd[1] != -1)//
			close(data->fd[1]);//
		if (data->in_fd != 0)//
			close(data->in_fd);//
		if (is_builtin(*cmd))
		{
			data->status = execute_builtin(*cmd, exp);
			exit(data->status);
		}
		else
		{
			execute_command(*cmd, exp->my_env);
			error_exit(exp->my_env, "execute_bis: execve fail");
		}
}

	
void	execute_bis(t_cmd **cmd, t_expander *exp, t_fork *data, int *i)
{
	if ((*cmd)->next && pipe(data->fd) == -1)
		error_exit(exp->my_env, "execute_bis: pipe");
	data->pid[*i] = fork();
	if (data->pid[*i] == -1)
		error_exit(exp->my_env, "execute bis: fork");
	
	if (data->pid[*i] == 0)
		execute_bis_2(cmd, exp, data);
	else
	{
		cleanup_parent(*cmd, &data->in_fd, data->fd);
		*cmd = (*cmd)->next;
		(*i)++;
	}
}

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
		free(data.pid);
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
	free(data.pid);
}
