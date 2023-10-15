/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_bonus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 20:50:05 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/14 23:17:19 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	builtin_cd(char **param)
{
	char	*dir;
	char	*pwd;
	char	*old_pwd;
	int		i;

	if (ft_lenmt((void **) param) > 1)
	{
		ft_dprintf(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (ft_lenmt((void **) param) == 0 || ft_strcmp(param[0], "~") == 0)
		dir = get_env("HOME");
	else
		dir = param[0];
	if (dir[0] == '/')
	{
		i = 0;
		while (dir[i] == '/')
			i++;
		i--;
		if (chdir(&dir[i]) != 0)
		{
			print_perror("cd", dir);
			return (1);
		}
	}
	if (chdir(dir) != 0)
	{
		print_perror("cd", dir);
		return (1);
	}
	old_pwd = get_env("PWD");
	if (old_pwd && *old_pwd)
		add_env("OLDPWD", old_pwd);
	pwd = getcwd(NULL, 0);
	if (pwd && *pwd)
	{
		add_env("PWD", pwd);
		free(pwd);
	}
	return (EXIT_SUCCESS);
}
