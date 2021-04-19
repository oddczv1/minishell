#include "../minishell.h"

void get_paths(t_data *data)
{
	char env_path[1000];
	int idx  = 0;
	while (data->env[idx])
	{
		if (ft_strncmp(data->env[idx], "PATH=", 5) == 0)
		{
			char *p = ft_strchr(data->env[idx], '=');
			ft_strlcpy(env_path, p + 1, 1000);
		}
		idx++;
	}
	data->paths = ft_split(env_path, ':');
}

int	execfile_in_path(t_data *data, char *path)
{
	DIR *dir_ptr = NULL; 
    struct dirent *file = NULL; 
	int ret = 0;
	int size= ft_strlen(data->cmd[0]);
    if((dir_ptr = opendir(path)) == NULL) 
    { 
        //fprintf(stderr, "%s directory 정보를 읽을 수 없습니다.\n", path); 
        return 0;//return 0 이 맞을듯...? 
    }
    while((file = readdir(dir_ptr)) != NULL)
    {
		if (!ft_strncmp(file->d_name, data->cmd[0], size + 1) && (int)ft_strlen(file->d_name) == size)
		{
			ret = 1;
			break;
		}
    }
    closedir(dir_ptr);
    return ret; 
}

int get_exec_dir_file(t_data *data)
{
	int idx = 0;
	while (data->paths[idx])
	{
		if (execfile_in_path(data, data->paths[idx]) == 1)
		{
			ft_strlcpy(data->exec_dir, data->paths[idx], 300);
			char *temp = ft_strjoin(data->exec_dir, "/");
			ft_strlcpy(data->exec_dir, temp, 300);
			free(temp);
			temp = ft_strjoin(data->exec_dir, data->cmd[0]);
			ft_strlcpy(data->exec_file, temp, 300);
			free(temp);
			break;
		}
		idx++;
	}
	if (data->paths[idx] == NULL)
	{
		//에러처리는 이 함수의 반환값을 받는 함수에서 진행.
		return (0);
	}
	return (1);
}
