/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 20:20:04 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/18 23:24:23 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_n(char **params)
{
	int		i;
	t_bool	brk;
	char	*str;

	brk = false;
	i = 0;
	while (params[i] && ft_strncmp(params[i], "-n", 2) == 0)
	{
		str = &params[i][2];
		while (*str)
		{
			if (*str++ != 'n')
			{
				brk = true;
				break ;
			}
		}
		if (brk)
			break ;
		i++;
	}
	return (i);
}

int	builtin_echo(char **params)
{
	int		i;
	t_bool	nl;

	nl = true;
	i = check_n(params);
	if (i > 0)
		nl = false;
	while (params[i])
	{
		write(STDOUT_FILENO, params[i], ft_strlen(params[i]));
		i++;
		if (params[i] != NULL)
			printf(" ");
	}
	if (nl)
		printf("\n");
	return (0);
}
