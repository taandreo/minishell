/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 18:53:19 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/25 20:51:05 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	bultin_pwd(char **params)
{
	char	*pwd;

	if (params == NULL)
		return (EXIT_FAILURE);
	pwd = getcwd(NULL, 0);
	if (pwd != NULL)
	{
		printf("%s\n", pwd);
		free(pwd);
	}
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
