/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:09:00 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/17 16:53:51 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// renvoie sur la bonne fonction builtin
int	execute_builtin(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (ft_echo(cmd->args));
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (ft_cd(cmd->args));
	if (!ft_strcmp(cmd->args[0], "pwd"))
		return (ft_pwd(cmd->args));
	if (!ft_strcmp(cmd->args[0], "export"))
		return (ft_export(cmd->args));
	if (!ft_strcmp(cmd->args[0], "unset"))
		return (ft_unset(cmd->args));
	if (!ft_strcmp(cmd->args[0], "env"))
		return (ft_env(cmd->args));
	if (!ft_strcmp(cmd->args[0], "exit"))
		exit(0);// voir si free etc avant
	return (1);
}

// Verifie si la commande entree est une builtin
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