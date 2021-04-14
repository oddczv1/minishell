/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youngrch <youngrch@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 23:45:09 by youngrch          #+#    #+#             */
/*   Updated: 2021/04/09 23:45:12 by youngrch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void		ft_check_redirection(t_data *d)
{
	int i;

	i = -1;
	while (d->cmd[++i])
	{
		if (ft_memcmp(d->cmd[i], "<", 2) == 0)
		{
			d->fd[0] = open(d->cmd[i + 1], O_RDONLY);
			if (d->fd[0] < 0)
				ft_putstr_fd("err\n", 2);  //read err
			//d->fd[1] = dup(0);
			//dup2(d->fd[0], 0);
			d->cmd[i] = 0;	
			//int read_size;
			//char buffer[5000];
			//read_size = read(d->fd[0], buffer, 5000);
			//buffer[read_size] = 0;
			//close(d->fd[0]);
			break;
		}	
		else if (ft_memcmp(d->cmd[i], ">", 2) == 0)
		{
			d->fd[0] = open(d->cmd[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
			d->fd[1] = dup(1);
			dup2(d->fd[0], 1);
			d->cmd[i] = 0;
			break;
		}	
		else if (ft_memcmp(d->cmd[i], ">>", 3) == 0)
		{
			d->fd[0] = open(d->cmd[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
			d->fd[1] = dup(1);
			dup2(d->fd[0], 1);
			d->cmd[i] = 0;
			break;
		}	
	}
}

void		ft_check_env(t_data *d)
{
	int i;
	int j;
	int quote;

	i = -1;
	while (d->cmd[++i])
	{
		j = 0;
		quote = 1;
		while (d->cmd[i][j])
		{
			if (d->cmd[i][j] == '\'' && d->cmd[i][j - 1] != '\\')
				quote *= -1;
			if (d->cmd[i][j] == '$' && quote != -1)
			{
				d->num = i;
				ft_put_env(d, d->cmd[i], &j);
			}
			else
				j++;
		}
	}		
}


void		ft_check_word(char *str, int *i)
{
	if ((str[(*i) - 1] != '\\' && str[*i] == '\'') || 
		(str[(*i) - 2] == '\\' && str[(*i) - 1] == '\\' && str[*i] == '\''))
		ft_check_quote(str, i, 1);
	else if ((str[(*i) - 1] != '\\' && str[*i] == '\"') || 
		(str[(*i) - 2] == '\\' && str[(*i) - 1] == '\\' && str[*i] == '\"'))
		ft_check_quote(str, i, 2);
	else if (str[(*i) - 1] != '\\' && str[*i] == '>' && str[*i + 1] != '>')
		ft_check_redirection_one(str, i);
	else if (str[(*i) - 1] != '\\' && str[*i] == '<' && str[*i + 1] != '<')
		ft_check_redirection_one(str, i);
	else if (str[(*i) - 1] != '\\' && str[*i] == '>' && str[*i + 1] == '>')
		ft_check_redirection_two(str, i);
	else
		(*i)++;
}

void		ft_check_argv(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		while (ft_isspace(str[i]))
		{
			str[i] = '|';
			i++;
		}
		if (str[i] == '\0')
			break ;
		while(str[i] && !ft_isspace(str[i]))
			ft_check_word(str, &i);
	}		
}


void		ft_command(t_data *d, int *fd_std, int *fd_cmd, int pipe1)
{
	int status;
	pid_t pid;
	if (pipe1 == 1)
	{
	ft_putstr_fd(ft_itoa(fd_std[0]), 2);
	ft_putchar_fd( ',', 2);
	ft_putstr_fd(ft_itoa(fd_std[1]), 2);
	ft_putchar_fd( ',', 2);
	ft_putstr_fd(ft_itoa(fd_cmd[0]), 2);
	ft_putchar_fd( ',', 2);
	ft_putstr_fd(ft_itoa(fd_cmd[1]), 2);
	ft_putchar_fd( '\n', 2);
	}


	if ((pid = fork()) == 0)
	{
		if (pipe1 == 0)
		{
			dup2(fd_cmd[0], 0);
			close(fd_cmd[0]);
			close(fd_cmd[1]);
		}
		else
		{
			if (pipe1 == 1)
				close(fd_cmd[0]);
			else
			{
				dup2(fd_cmd[0], 0);
				close(fd_cmd[0]);
			}
			ft_putstr_fd(ft_itoa(pipe1), 2);
			ft_putchar_fd( '\n', 2);				
			dup2(fd_cmd[1], 1);
			close(fd_cmd[1]);
		}
		if (ft_memcmp(d->cmd[0], "ls", 3) == 0)
			execve("/bin/ls", d->cmd, NULL);
		if (ft_memcmp(d->cmd[0], "grep", 5) == 0)
			execve("/usr/bin/grep", d->cmd, NULL);
	}
	else
	{ 
		if (pipe1 == 0)
		{
			dup2(fd_std[0], 0);
			dup2(fd_std[1], 1);
			close(fd_cmd[0]);
			close(fd_cmd[1]);
		}
		else
		{
			close(fd_cmd[1]);
			//close(fd_cmd[0]);
		}
		//close(fd_cmd[0]);
		waitpid(pid, &status, 0);
	}
}
//ls -al | grep m | grep main
void        parse(t_data *d)
{
	int fd_std[2];
	int fd_cmd[2];
	//pipe(fd_std);

	int status;
	pid_t pid = 0;
	int pipe1;
    int i;
	int j;
	
	i = -1;
	d->cmds = ft_split_semi(d->str);
	if (!d->cmds)
		return ;
	while (d->cmds[++i])
	{			
		d->argv = ft_split_pipe(d->cmds[i]);
		if (d->argv[1] != NULL)
		{
			pipe1 = 0;
			if ((pid = fork()) == 0)
			{
				pipe(fd_cmd);
				fd_std[0] = dup(0);
				fd_std[1] = dup(1);
				j = -1;
				while (d->argv[++j])
				{
					ft_check_argv(d->argv[j]);
					d->cmd = ft_split_pipe(d->argv[j]);
					ft_check_env(d);
					ft_remove_mark(d);							
					ft_check_redirection(d);		
					if (d->argv[j + 1] != 0)
						pipe1 += 1;
					else
						pipe1 = 0;
					ft_command(d, fd_std, fd_cmd, pipe1);	
					//process(d);				
					ft_free(d->cmd);			
        		}			
			}
			else
				waitpid(pid, &status, 0);
		}
		else
		{
			j = -1;
			while (d->argv[++j])
			{
				ft_check_argv(d->argv[j]);
				d->cmd = ft_split_pipe(d->argv[j]);
				ft_check_env(d);
				ft_remove_mark(d);							
				ft_check_redirection(d);		
				pipe1 = 0;
				ft_command(d, fd_std, fd_cmd, pipe1);					
				ft_free(d->cmd);			
        	}
			ft_free(d->argv);
    	}
	}
	ft_free(d->cmds);
	return ;
}
