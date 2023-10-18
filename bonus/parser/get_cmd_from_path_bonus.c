/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_from_path_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:59:40 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/16 00:05:15 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*join_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*cmd_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (return_mem_alloc_error());
	cmd_path = join_1st_and_cleanup(&tmp, cmd);
	if (!cmd_path)
		return (return_mem_alloc_error());
	return (cmd_path);
}

static t_bool	cmd_exist(char *cmd_path, t_bool *error, t_vars *vars)
{
	struct stat	file_info;

	*error = false;
	if (access(cmd_path, F_OK) == 0)
	{
		if (stat(cmd_path, &file_info) == 0)
		{
			if (!S_ISDIR(file_info.st_mode) && access(cmd_path, X_OK) == 0)
				return (true);
			else if (!S_ISDIR(file_info.st_mode)
				&& access(cmd_path, X_OK) == -1)
			{
				return_permission_denied(cmd_path, vars);
				*error = true;
				vars->got_cmd_path = true;
			}
		}
		else
		{
			perror("");
			*error = true;
		}
	}
	return (false);
}

static char	*find_path(char **dir, char *cmd, t_vars *vars)
{
	char	*cmd_path;
	int		i;
	t_bool	error;

	i = 0;
	cmd_path = NULL;
	while (dir[i])
	{
		cmd_path = join_path(dir[i], cmd);
		if (!cmd_path)
			break ;
		if (cmd_exist(cmd_path, &error, vars))
			break ;
		else
		{
			free(cmd_path);
			cmd_path = NULL;
			if (error)
				break ;
		}
		i++;
	}
	return (cmd_path);
}

char	*get_cmd_path(char *cmd, t_vars *vars)
{
	char		*cmd_path;
	char		*path;
	char		**dir;
	int			i;

	path = get_env("PATH");
	if (!path)
		return (NULL);
	dir = ft_split(path, ':');
	i = 0;
	cmd_path = find_path(dir, cmd, vars);
	while (dir[i])
		free(dir[i++]);
	free(dir);
	return (cmd_path);
}
