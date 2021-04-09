/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huchoi <huchoi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 14:56:36 by youngrch          #+#    #+#             */
/*   Updated: 2021/04/09 14:51:39 by huchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"

# define huchoi 111

# include <unistd.h>

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <sys/types.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>

typedef struct		s_data{
	int		argc;
	char	**cmds;//if call the func, output is stored this variable.
	char	**argv;//split by semiclone
	char	**cmd;
	char	**envp;
	char	**export;
	int		ret;
	char	*str;
	int		fd[2];
	
}					t_data;

int					ft_read_str(t_data *d, char *buf);
void        		parse(t_data *d);
#endif
