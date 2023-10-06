/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:44:45 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/04 21:50:00 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	print_env_node(void *node)
{
	t_env	*env;

	env = (t_env *) node;
	if (env->value != NULL)
		printf("%s=%s\n", env->key, env->value);
}

int	builtin_env(char **params)
{
	if (params != NULL)
		ft_lstiter(g_env, print_env_node);
	return (EXIT_SUCCESS);
}
