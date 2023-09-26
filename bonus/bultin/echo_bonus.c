/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 20:20:04 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/25 21:04:53 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	bultin_echo(char **params)
{
	int		i;
	char	*str;
	int		newline;

	i = 0;
	if (params[0] && ft_strncmp(params[0], "-n", 2) == 0)
	{
		i = 1;
		str = &params[0][2];
		while (*str)
		{
			if (*str++ != 'n')
				i = 0;
		}
	}
	newline = i;
	while (params[i])
	{
		printf("%s", params[i++]);
		if (params[i] != NULL)
			printf(" ");
	}
	if (newline == 0)
		printf("\n");
	return (0);
}
