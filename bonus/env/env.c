/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 16:09:55 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/21 17:49:58 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_list *g_env;

void	add_env(char *key, char *value)
{
	t_env *var;
	t_list *node;

	var = ft_calloc(1, sizeof(t_env));
	var->key = ft_strdup(key);
	var->value = ft_strdup(value);
	node = ft_lstnew(var);
	ft_lstadd_back(&g_env, node);
}

void	print_env_node(void *node)
{
	t_env *var;
	
	var = (t_env *) node;
	printf("%s=%s\n", var->key, var->value);
}

void	print_env()
{
	ft_lstiter(g_env, print_env_node);
}

void	init_env(char **envp)
{
	size_t	i;
	char	*value;
	char	*key;

	i = 0;
	g_env = NULL;
	while(envp[i])
	{
		value = ft_strchr(envp[i], '=');
		value++;
		key = ft_strndup(envp[i], value - envp[i]);
		add_env(key, value);
	}
}