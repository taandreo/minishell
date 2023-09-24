/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:50:25 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/24 13:26:07 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	print_export_node(void *node)
{
	t_env *env;
	
	env = (t_env *) node;
	if (env->value == NULL)
		printf("declare -x %s\n", env->key);
	else
		printf("declare -x %s=\"%s\"\n", env->key, env->value);
}

t_bool	is_valid_env(char *env)
{
	size_t	i;

	i = 0;
	if (!(ft_isalpha(env[i]) || env[i] == '_'))
		return (false);
	while(env[i])
	{
		if (!(ft_isalnum(env[i]) || env[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

int bultin_export(char **params)
{
	char    *key;
	char	*value;
	size_t	error;

	error = EXIT_SUCCESS;
	if (!*params)
		ft_lstiter(g_env, print_export_node);
	while (*params)
	{
		value = ft_strchr(*params, '=');
		if (value)
		{
			value++;
			key = ft_strndup(*params, value - *params - 1);
		}
		else
			key = ft_strdup(*params);
		if (is_valid_env(key))
			add_env(key, value);
		else
		{
			error = GENERAL_ERROR;
			dprintf(2, "minishell: export: `%s': not a valid identifier\n", *params);
		}
		free(key);
		params++;
	}
	return (error);
}