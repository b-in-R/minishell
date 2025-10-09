/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 18:58:01 by rabiner           #+#    #+#             */
/*   Updated: 2025/10/09 11:19:58 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# ifndef O_CLOEXEC
#  define O_CLOEXEC 0
# endif

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# ifndef PROMPT
#  define PROMPT "\001\033[32m\002minishell> \001\033[0m\002"
# endif

//# include <stddef.h>
//# include <sys/types.h>
//# include <stdlib.h>
//# include <unistd.h>
//# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
//# include <fcntl.h>
# include <sys/wait.h>// a voir si obligatoire
# include <errno.h>// a voir si obligatoire
//# include <limits.h> //

#endif