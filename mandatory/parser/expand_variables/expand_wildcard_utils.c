/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:43:19 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/15 23:44:21 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	sorted_insert(t_string **head_ref, t_string *new_node)
{
	t_string	*current;

	if (*head_ref == NULL
		|| ft_strcasecmp((*head_ref)->value, new_node->value) >= 0)
	{
		new_node->next = *head_ref;
		*head_ref = new_node;
	}
	else
	{
		current = *head_ref;
		while (current->next != NULL
			&& ft_strcasecmp(current->next->value, new_node->value) < 0)
			current = current->next;
		new_node->next = current->next;
		current->next = new_node;
	}
}

t_string	*add_new_node(char *filename, t_vars *vars,	t_token_type type)
{
	t_string	*new_node;

	new_node = malloc(sizeof(t_string));
	if (!new_node)
	{
		vars->state.status = MISUSE;
		return (return_mem_alloc_error());
	}
	new_node->type = type;
	new_node->value = ft_strdup(filename);
	if (!new_node->value)
	{
		vars->state.status = MISUSE;
		return (return_mem_alloc_error());
	}
	return (new_node);
}

DIR	*open_dir_or_error(void)
{
	DIR	*dir;

	dir = opendir(".");
	if (!dir)
	{
		perror("minishell: opendir: Error while opening the current directory");
		return (NULL);
	}
	return (dir);
}
