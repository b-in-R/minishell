/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:13:10 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/23 00:16:14 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"

/*
* Checks if the character c is a valid character for an environment variable.
* A valid character is alphanumeric (a-z, A-Z, 0-9) or an underscore (_).
*/
int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}
