/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 16:09:55 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:39:31 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*search_env(char *key)
{
	t_list	*node;
	t_env	*env;

	node = g_vars.env;
	while (node)
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
	}
	else
	{
		env = ft_calloc(1, sizeof(t_env));
		env->key = ft_strdup(key);
		node = ft_lstnew(env);
		ft_lstadd_back(&g_vars.env, node);
	}
	if (value != NULL)
		env->value = ft_strdup(value);
	else
		env->value = NULL;
}

char	*get_env(char *key)
{
	t_list	*node;
	t_env	*env;

	node = search_env(key);
	if (!node)
		return (NULL);
	env = node->content;
	return (env->value);
}

void	remove_env(char *key)
{
	t_list	*node;
	t_list	*prev;
	t_env	*env;

	node = g_vars.env;
	prev = NULL;
	while (node)
	{
		env = node->content;
		if (ft_strcmp(env->key, key) == 0)
		{
			if (node == g_vars.env)
				g_vars.env = node->next;
			else
				prev->next = node->next;
			ft_lstdelone(node, free_env);
			return ;
		}
		prev = node;
		node = node->next;
	}
}

void	init_env(char **envp)
{
	size_t	i;
	char	*value;
	char	*key;

	i = 0;
	g_vars.env = NULL;
	while (envp[i])
	{
		value = ft_strchr(envp[i], '=');
		value++;
		key = ft_strndup(envp[i], value - envp[i] - 1);
		add_env(key, value);
		free(key);
		i++;
	}
}
