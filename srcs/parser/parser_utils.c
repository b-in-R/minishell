/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:11:41 by albertooutu       #+#    #+#             */
/*   Updated: 2025/10/09 16:35:34 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_redir_in(t_expander *exp, t_cmd *cmd, char *value)
{
	if (cmd->infile)
		pool_free_one(exp->pool, cmd->infile);
	cmd->infile = pool_strdup(exp->pool, value);
}

void	handle_redir_out(t_expander *exp, t_cmd *cmd, char *value, int type)
{
	if (cmd->outfile)
		pool_free_one(exp->pool, cmd->outfile);
	cmd->outfile = pool_strdup(exp->pool, value);
	cmd->append = (type == REDIR_APPEND);
}

void	handle_heredoc(t_expander *exp, t_cmd *cmd, t_token *tok)
{
	if (cmd->delimiter)
		pool_free_one(exp->pool, cmd->delimiter);
	cmd->heredoc = 1;
	cmd->delimiter = remove_outer_quotes(exp->pool, tok->next->value, cmd);
	cmd->expand_heredoc = (tok->next->quoted_type == NO_QUOTE);
}
