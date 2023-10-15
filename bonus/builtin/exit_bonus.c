/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 16:09:14 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/15 01:24:52 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/* The program has to exit with the last command exit 
   status before exit*/
void	exit_minishell(int nb, t_vars *vars)
{
	free_minishell(vars);
	exit(nb);
}

int	builtin_exit(char **params, t_vars *vars)
{
	unsigned char	nb;

	if (isatty(STDIN_FILENO) && !vars->is_forked)
		ft_dprintf(2, "exit\n");
	if (!*params)
	{
		nb = vars->state.status;
		exit_minishell(nb, vars);
	}
	if (!ft_is_number(*params))
	{
		ft_dprintf(STDERR_FILENO, \
		"minishell: exit: %s: numeric argument required\n", *params);
		exit_minishell(2, vars);
	}
	if (ft_lenmt((void **) params) > 1)
	{
		ft_dprintf(STDERR_FILENO, \
		"minishell: exit: too many arguments\n", *params);
		return (EXIT_FAILURE);
	}
	nb = ft_atoi(*params);
	exit_minishell(nb, vars);
	return (EXIT_SUCCESS);
}
