/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:50:25 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/15 23:14:52 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	print_export_node(void *node)
{
	t_env	*env;

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
	while (env[i])
	{
		if (!(ft_isalnum(env[i]) || env[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

void	get_export_variable(char *param, size_t *error)
{
	char	*key;
	char	*value;

	value = ft_strchr(param, '=');
	if (value)
	{
		value++;
		key = ft_strndup(param, value - param - 1);
	}
	else
		key = ft_strdup(param);
	if (is_valid_env(key))
		add_env(key, value);
	else
	{
		*error = GENERAL_ERROR;
		dprintf(2, "minishell: export: `%s': not a valid identifier\n", param);
	}
	free(key);
}

int	builtin_export(char **params)
{
	size_t	error;

	error = EXIT_SUCCESS;
	if (!*params)
		ft_lstiter(g_vars.env, print_export_node);
	while (*params)
	{
		get_export_variable(*params, &error);
		params++;
	}
	return (error);
}
