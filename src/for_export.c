/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   for_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huchoi <huchoi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 15:07:50 by huchoi            #+#    #+#             */
/*   Updated: 2021/04/24 15:17:56 by huchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		is_valid(t_data *data)
{
	if (!check_head_valid(data))
		return (0);
	else if (!check_other_valid(data))
		return (0);
	else
		return (1);
}

void	print_value(char *p)
{
	write(1, "\"", 1);
	ft_putstr_fd(p + 1, 1);
	write(1, "\"", 1);
	ft_putstr_fd("\n", 1);
}

void	process_print(t_data *data)
{
	int		idx;
	char	*p;
	int		count;

	if (data->cmd[1] == NULL)
	{
		idx = 0;
		while (data->env[idx])
		{
			ft_putstr_fd("declare -x ", 1);
			count = 0;
			while (data->env[idx][count])
			{
				write(1, &data->env[idx][count], 1);
				if (data->env[idx][count++] == '=')
					break ;
			}
			if ((p = ft_strchr(data->env[idx], '=')) != NULL)
				print_value(p);
			idx++;
		}
	}
	recover_std(data);
}

void	process_export(t_data *data)
{
	if (data->cmd[1] && is_valid(data))
		add_env(data, data->cmd[1]);
	else if (!data->cmd[1])
		process_print(data);
	else
	{
		ft_putstr_fd("bash: export: `", 2);
		ft_putstr_fd(data->cmd[1], 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		data->flag = 1;
		g_t.status = 1;
	}
	get_paths(data);
	recover_std(data);
}
