/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rabiner_inputs.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:11:34 by rabiner           #+#    #+#             */
/*   Updated: 2025/05/20 14:29:20 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RABINER_INPUTS_H
# define RABINER_INPUTS_H

# include "minishell.h"


typedef enum t_build
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}			s_build;

#endif