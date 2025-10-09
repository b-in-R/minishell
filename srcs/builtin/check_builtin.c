/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:09:00 by rabiner           #+#    #+#             */
/*   Updated: 2025/10/09 17:18:34 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_digits(char *args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (!ft_isdigit(args[i]))
			return (0);
		i++;
	}
	return (1);
}

// Check args to give the right exit code
static int	ft_exit(t_cmd *cmd, t_expander *exp)
{
	int	i;

	i = 0;
	printf("exit\n");
	printf("exp->double:\t%i\n", exp->double_quotes);//
	if (cmd->args[1] && !check_digits(cmd->args[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n",
		cmd->args[1]);
		return (2);
	}
	else if(exp->double_quotes > 0)
	{
		printf("minishell: exit: %s: numeric argument required\n",
		cmd->args[1]);
		return (2);
	}
	else if (cmd->args[1] && cmd->args[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	else if (cmd->args[1])
	{
		i = ft_atoi(cmd->args[1]);
		return (i);
	}
	return (0);
}

int	execute_builtin(t_cmd *cmd, t_expander *exp)
{
	int	i;

	i = 0;
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (ft_echo(cmd->args));
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (ft_cd(exp, cmd->args));
	if (!ft_strcmp(cmd->args[0], "pwd"))
		return (ft_pwd(exp->my_env, cmd->args));
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(exp, cmd->args));
	if (!ft_strcmp(cmd->args[0], "unset"))
		return (ft_unset(exp, cmd->args));
	if (!ft_strcmp(cmd->args[0], "env"))
		return (print_env(exp->my_env));
	if (!ft_strcmp(cmd->args[0], "exit"))
	{
		//print_cmds(cmd);//
		i = ft_exit(cmd, exp);
		pool_cleanup(exp->pool);// voir emplacement, free si 1?
		if (i != 1)
		{
			exit(i);
		}
	}
	return (1);
}

int	is_builtin(t_cmd *cmd)
{
	char	*name;

	if (!cmd->args || !cmd->args[0])
		return (0);
	name = cmd->args[0];
	if (!ft_strcmp(name, "echo"))
		return (1);
	if (!ft_strcmp(name, "cd"))
		return (1);
	if (!ft_strcmp(name, "pwd"))
		return (1);
	if (!ft_strcmp(name, "export"))
		return (1);
	if (!ft_strcmp(name, "unset"))
		return (1);
	if (!ft_strcmp(name, "env"))
		return (1);
	if (!ft_strcmp(name, "exit"))
		return (1);
	return (0);
}
