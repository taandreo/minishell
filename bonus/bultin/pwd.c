/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 18:53:19 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/19 20:38:47 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	bultin_pwd(char **params)
{
	char *pwd;
	if (params == NULL)
		return (1);
	pwd = getcwd(NULL, 0);
	if (pwd != NULL)
		printf("%s\n", pwd);
	else 
		return (1);
	return (0);
}