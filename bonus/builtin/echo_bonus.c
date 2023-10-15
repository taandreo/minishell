/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 20:20:04 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/04 21:53:07 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	builtin_echo(char **params)
{
	int		i;
	int		j;
	char	*str;
	int		newline;

	i = 0;
	j = 0;
	if (params[0] && ft_strncmp(params[0], "-n", 2) == 0)
	{
		while (params[j] && ft_strncmp(params[j], "-n", 2) == 0)
		{
			i = 1;
			str = &params[j][2];
			while (*str)
			{
				if (*str++ != 'n')
					i = 0;
			}
			if (i == 0)
				break ;
			j++;
		}
	}
	newline = i;
	while (params[j])
	{
		printf("%s", params[j++]);
		if (params[i] != NULL)
			printf(" ");
	}
	if (newline == 0 && j > 0)
		printf("\n");
	return (0);
}
