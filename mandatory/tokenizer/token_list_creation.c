/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_creation_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:31:06 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:07 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_node	*init_new_node(t_token_list **tokens,
					t_token_node *new_node, t_token_type type,
					const char *value);

t_token_list	*create_token_list(void)
{
	t_token_list	*token_list;

	token_list = malloc(sizeof(t_token_list));
	if (!token_list)
		return (return_mem_alloc_error());
	token_list->head = NULL;
	token_list->tail = NULL;
	token_list->current = NULL;
	token_list->count = 0;
	return (token_list);
}

int	add_token(t_token_list **tokens, t_token_type type,
			const char *value)
{
	t_token_node	*new_node;

	if (!tokens || !*tokens)
		return (MISUSE);
	new_node = NULL;
	new_node = init_new_node(tokens, new_node, type, value);
	if (!new_node)
		return (MISUSE);
	if ((*tokens)->tail)
		(*tokens)->tail->next = new_node;
	else
		(*tokens)->head = new_node;
	(*tokens)->tail = new_node;
	(*tokens)->count++;
	return (SUCCESS);
}

void	free_token_list(t_token_list **token_list_ptr)
{
	t_token_node	*current;
	t_token_node	*next;

	if (!token_list_ptr || ! *token_list_ptr)
		return ;
	current = (*token_list_ptr)->head;
	while (current)
	{
		next = current->next;
		free(current->token.value);
		free(current);
		current = next;
	}
	free(*token_list_ptr);
	*token_list_ptr = NULL;
}

t_token_node	*init_new_node(t_token_list **tokens,
					t_token_node *new_node, t_token_type type,
					const char *value)
{
	new_node = (t_token_node *)malloc(sizeof(t_token_node));
	if (!new_node)
	{
		free_token_list(tokens);
		return (NULL);
	}
	new_node->token.type = type;
	new_node->token.value = ft_strdup(value);
	if (!new_node->token.value)
	{
		free(new_node);
		new_node = NULL;
		free_token_list(tokens);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}
