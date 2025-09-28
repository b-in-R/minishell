/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:11:41 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/25 16:29:51 by albertooutu      ###   ########.fr       */
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
	cmd->delimiter = remove_outer_quotes(exp->pool, tok->next->value);
	cmd->expand_heredoc = (tok->next->quoted_type == NO_QUOTE);
}
