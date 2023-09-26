/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 16:09:14 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/25 22:20:23 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/* The program has to exit with the last command exit 
   status before exit*/
void	exit_minishell(int nb)
{
	exit(nb);
}

int	bultin_exit(char **params)
{
	unsigned char	nb;

	ft_dprintf(2, "exit\n");
	if (!*params)
	{
		// Necessário rever e entender como vamos pegar a saída do último comando
		nb = EXIT_SUCCESS;
		exit_minishell(nb);
	}
	if (!ft_is_number(*params))
	{
		ft_dprintf(STDERR_FILENO, \
		"minishell: exit: %s: numeric argument required\n", *params);
		exit_minishell(255);
	}
	if (ft_lenmt((void **) params) > 1)
	{
		ft_dprintf(STDERR_FILENO, \
		"minishell: exit: too many arguments\n", *params);
		return (EXIT_FAILURE);
	}
	nb = ft_atoi(*params);
	exit_minishell(nb);
	return (EXIT_SUCCESS);
}