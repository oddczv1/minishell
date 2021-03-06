/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huchoi <huchoi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 23:45:09 by youngrch          #+#    #+#             */
/*   Updated: 2021/04/26 13:12:59 by huchoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void		ft_check_word(t_data *d, char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!ft_check_escape_num(str, i) && str[i] == '\'')
			ft_check_quote(d, str, &i, 1);
		else if (!ft_check_escape_num(str, i) && str[i] == '\"')
			ft_check_quote(d, str, &i, 2);
		else if (str[(i) - 1] != '\\' && str[i] == '>' && str[i + 1] != '>')
			ft_check_redirection_one(str, &i);
		else if (str[(i) - 1] != '\\' && str[i] == '<' && str[i + 1] != '<')
			ft_check_redirection_one(str, &i);
		else if (str[(i) - 1] != '\\' && str[i] == '>' && str[i + 1] == '>')
			ft_check_redirection_two(str, &i);
		else if (str[(i) - 1] != '\\' && str[i] == '<' && str[i + 1] == '<')
			ft_check_redirection_two(str, &i);
		else
			(i)++;
	}
}

void		ft_check_semi(t_data *d)
{
	int i;

	i = -1;
	if (g_t.str != 0)
	{
		while (g_t.str[++i])
		{
			if (g_t.str[i] == ';' && g_t.str[i + 1] == ';')
			{
				ft_putstr_fd("bash: syntax error near unexpected token\n", 2);
				g_t.status = 258;
				d->enable = 1;
			}
		}
	}
}

void		ft_check_split(t_data *d, int idx)
{
	d->flag = 0;
	ft_check_word(d, d->argv[idx]);
	d->cmd = ft_split(d->argv[idx], ' ');
	ft_get_cmdlen(d);
	ft_check_env(d);
	if (!d->enable)
		ft_remove_mark(d);
	if (g_t.err == 1)
		d->enable = 1;
	ft_check_redirection(d);
	ft_check_upper(d);
}

void		parse(t_data *d)
{
	d->p_i = -1;
	d->enable = 0;
	ft_check_semi(d);
	d->cmds = ft_split(g_t.str, ';');
	if (!d->cmds)
		return ;
	while (d->cmds[++d->p_i])
	{
		ft_check_pipe(d, d->cmds[d->p_i]);
		d->argv = ft_split(d->cmds[d->p_i], '|');
		if (d->argv[1] != NULL && !d->enable)
			is_pipe(d);
		else
			non_pipe(d);
		ft_free(d->argv);
	}
	ft_free(d->cmds);
	return ;
}
