/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_and_envp_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 00:56:23 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 17:18:17 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	arg_len(t_arguments *list)
{
	int	len;

	len = 0;
	while (list)
	{
		len++;
		list = list->next;
	}
	return (len);
}

char	**list_to_args(t_arguments *list, t_command_part *cmd_part)
{
	t_arguments	*start;
	char		**args;
	size_t		i;

	start = list;
	args = ft_calloc(arg_len(list) + 2, sizeof(char *));
	if (is_token_cmd_name(cmd_part->type))
		args[0] = cmd_part->u_cmd.cmd_name->value;
	else if (is_builtin_token(cmd_part->type))
		args[0] = cmd_part->u_cmd.builtin_cmd->value;
	i = 1;
	while (start)
	{
		args[i] = start->string->value;
		i++;
		start = start->next;
	}
	args[i] = NULL;
	return (args);
}

char	*join_tmp_key_value(t_env *tmp)
{
	char	*tmp2;
	char	*envp;

	tmp2 = ft_strjoin(tmp->key, "=");
	if (!tmp2)
		return (return_mem_alloc_error());
	envp = ft_strjoin(tmp2, tmp->value);
	if (!envp)
		return (return_mem_alloc_error());
	free(tmp2);
	return (envp);
}

size_t	get_env_size(void)
{
	size_t	i;
	t_list	*current;

	i = 0;
	current = g_vars.env;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

char	**list_to_envp(void)
{
	size_t	len;
	char	**envp;
	t_list	*current;
	t_env	*tmp;
	size_t	i;

	i = 0;
	current = g_vars.env;
	len = get_env_size();
	envp = ft_calloc((len + 1), sizeof(char *));
	while (current)
	{
		tmp = current->content;
		if (tmp->value)
		{
			envp[i] = join_tmp_key_value(tmp);
			if (!envp[i])
				return (NULL);
		}
		current = current->next;
		i++;
	}
	return (envp);
}
