/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_1_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 20:11:22 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/13 21:42:31 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	free_env(void *env)
{
	t_env	*var;

	var = env;
	free(var->key);
	if (var->value != NULL)
		free(var->value);
	free(var);
}

void	free_all_envs(void)
{
	ft_lstclear(&g_vars.env, free_env);
}
