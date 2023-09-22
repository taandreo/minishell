/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 20:50:05 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/21 18:25:47 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	bultin_cd(char **param)
{
	char	*dir;

	if (ft_lenmt((void **) param) == 0 || ft_strcmp(param[0], "~") == 0)
		dir = lookup_variable("HOME");
	else 
		dir = param[0];
	if (chdir(dir) != 0)
	{
		print_perror("cd", dir);
		free(dir);
		return (1);
	}
}