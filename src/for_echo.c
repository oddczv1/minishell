/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   for_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huchoi <huchoi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 15:31:52 by huchoi            #+#    #+#             */
/*   Updated: 2021/04/26 19:36:57 by huchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void			delete_last_newline(char **cmd)
{
	int		idx;
	char	*str;

	idx = 0;
	while (cmd[idx])
		idx++;
	str = cmd[idx - 1];
	idx = 0;
	while (str[idx])
		idx++;
	if (str[idx - 1] == '\n')
		str[idx] = '\0';
}

int				is_newline(char *str)
{
	int idx;

	idx = 0;
	while (str[idx])
		idx++;
	if (str[idx - 1] == '\n')
		return (1);
	return (0);
}

void			my_putstr_fd(char *str, int fd)
{
	int idx;

	idx = 0;
	while (str[idx])
	{
		write(fd, &str[idx], 1);
		idx++;
	}
}

void			porcess_echo(t_data *data)
{
	int	idx;

	idx = 1;
	if (data->cmd[1] == NULL)
	{
		if (!(data->is_cflage))
			write(1, "\n", 1);
		return ;
	}
	while (!ft_strncmp(data->cmd[idx], "-n", 3))
		idx++;
	while (data->cmd[idx + 1] && data->cmd[idx])
	{
		my_putstr_fd(data->cmd[idx++], 1);
		ft_putstr_fd(" ", 1);
	}
	if (ft_strncmp(data->cmd[1], "-n", 3) != 0)
	{
		my_putstr_fd(data->cmd[idx], 1);
		write(1, "\n", 1);
	}
	else
		my_putstr_fd(data->cmd[idx], 1);
	recover_std(data);
}
