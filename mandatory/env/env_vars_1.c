/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 20:11:22 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:39:33 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
