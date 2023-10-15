#include "minishell_bonus.h"

char *join_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*cmd_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (return_mem_alloc_error());
	cmd_path = join_1st_and_cleanup(&tmp, cmd);
	if (!cmd_path)
		return (return_mem_alloc_error());
	return cmd_path;
}

char *get_cmd_path(char *cmd)
{
	char	*cmd_path;
	char	*path;
	char	**dir;
	struct stat	file_info;
	int		i;

	path = get_env("PATH");
	dir = ft_split(path, ':');
	i = 0;
	while (dir[i])
	{
		cmd_path = join_path(dir[i], cmd);
		if (!cmd_path)
			break ;
		if (access(cmd_path, F_OK) == 0)
		{
			if (stat(cmd_path, &file_info) == 0)
			{
				if (!S_ISDIR(file_info.st_mode) && access(cmd_path, X_OK) == 0)
					break ;
			}
			else
			{
				perror("");
				free(cmd_path);
				cmd_path = NULL;
				break ;
			}
		}
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	i = 0;
	while (dir[i])
		free(dir[i++]);
	free(dir);
	return (cmd_path);
}

char	*cmd_path_routine(char *cmd, t_token_type token, t_vars *vars)
{
	char		*cmd_path;
	struct stat	file_info;

	if (ft_strlen(cmd) == 0 && token == TOKEN_COMMAND_NAME_QUOTES)
		return (return_cmd_not_found(cmd, vars));
	else if (ft_strlen(cmd) == 0 && token == TOKEN_COMMAND_NAME_UNQUOTES)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (stat(cmd, &file_info) == 0)
			{
				if (S_ISDIR(file_info.st_mode))
					return (return_is_dir_error_cmd(cmd, vars));
				if (access(cmd, X_OK) == 0)
					cmd_path = ft_strdup(cmd);
				if (access(cmd, X_OK) == -1)
					return (return_permission_denied(cmd, vars));
			}
			else
			{
				perror("");
				return (NULL);
			}
		}
		else
			return (return_no_such_cmd(cmd, vars));
	}
	else
		cmd_path = get_cmd_path(cmd);
	if (!cmd_path)
		return (return_cmd_not_found(cmd, vars));
	return (cmd_path);
}
