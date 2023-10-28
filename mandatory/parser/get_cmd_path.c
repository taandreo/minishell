/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 22:34:17 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:42:57 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_local_cmd(char *cmd, t_vars *vars)
{
	struct stat	file_info;

	if (access(cmd, F_OK) == 0)
	{
		if (stat(cmd, &file_info) == 0)
		{
			if (S_ISDIR(file_info.st_mode))
				return (return_is_dir_error_cmd(cmd, vars));
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
			if (access(cmd, X_OK) == -1)
				return (return_permission_denied(cmd, vars));
		}
		else
		{
			perror("");
			return (NULL);
		}
	}
	return (return_no_such_cmd(cmd, vars));
}

char	*cmd_path_routine(char *cmd, t_token_type token, t_vars *vars)
{
	char		*cmd_path;

	if (ft_strlen(cmd) == 0 && token == TOKEN_COMMAND_NAME_QUOTES)
		return (return_cmd_not_found(cmd, vars));
	else if (ft_strlen(cmd) == 0 && token == TOKEN_COMMAND_NAME_UNQUOTES)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		cmd_path = check_local_cmd(cmd, vars);
		if (!cmd_path)
			return (NULL);
	}
	else
		cmd_path = get_cmd_path(cmd, vars);
	if (!cmd_path && !vars->got_cmd_path)
		return (return_cmd_not_found(cmd, vars));
	return (cmd_path);
}
