/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 20:20:04 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/15 22:51:31 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
		printf("%s", params[i++]);
		if (params[i] != NULL)
			printf(" ");
	}
	if (nl && i > 0)
		printf("\n");
	return (0);
}
