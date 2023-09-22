/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 16:09:55 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/21 22:33:17 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_list *g_env;

t_list	*search_env(char *key)
{
	t_list	*node;
	t_env	*env;

	node = g_env;
	while(node)
	{
		env = node->content;
		if (ft_strcmp(env->key, key) == 0)
			return (node);
		node = node->next;
	}
	return (NULL);
}

void	add_env(char *key, char *value)
{
	t_env	*env;
	t_list	*node;

	node = NULL;
	node = search_env(key);
	if (node != NULL)
	{
		env = node->content;
		free(env->value);
		env->value = ft_strdup(value);
		return ;
	}
	env = ft_calloc(1, sizeof(t_env));
	env->key = ft_strdup(key);
	env->value = ft_strdup(value);
	node = ft_lstnew(env);
	ft_lstadd_back(&g_env, node);
}

void	free_env(void *env)
{
	t_env	*var;

	var = env;
	free(var->key);
	free(var->value);
	free(var);
}

void	remove_env(char *key)
{
	t_list	*node;
	t_list	*prev;
	t_env	*env;

	node = g_env;
	prev = NULL;
	while(node)
	{
		env = node->content;
		if (ft_strcmp(env->key, key) == 0)
		{
			if (node == g_env)
				g_env = node->next;
			else
				prev->next = node->next;
			ft_lstdelone(node, free_env);
		}
		prev = node;
		node = node->next;
	}
}

void	print_env_node(void *node)
{
	t_env *env;
	
	env = (t_env *) node;
	printf("%s=%s\n", env->key, env->value);
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
		key = ft_strndup(envp[i], value - envp[i] - 1);
		add_env(key, value);
		i++;
	}
}