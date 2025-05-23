/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:11:41 by albertooutu       #+#    #+#             */
/*   Updated: 2025/05/23 13:12:13 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
/*
*	Parser:
*	Reads the list of t_token to build structured t_cmd elements
*	(complete commands with arguments, redirections, pipes, etc.).
*/
