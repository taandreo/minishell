#include "minishell_bonus.h"

t_token_node	*init_new_node(t_token_list *token_list,
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

int	add_token(t_token_list *token_list, t_token_type type,
			const char *value)
{
	t_token_node	*new_node;

	if (!token_list)
		return (MISUSE);
	new_node = (t_token_node *)malloc(sizeof(t_token_node));
	new_node = init_new_node(token_list, new_node, type, value);
	if (!new_node)
		return (MISUSE);
	if (token_list->tail)
		token_list->tail->next = new_node;
	else
		token_list->head = new_node;
	token_list->tail = new_node;
	token_list->count++;
	return (SUCCESS);
}

void	free_token_list(t_token_list *token_list)
{
	t_token_node	*current;
	t_token_node	*next;

	current = token_list->head;
	while (current)
	{
		next = current->next;
		free(current->token.value);
		free(current);
		current = next;
	}
}

t_token_node	*init_new_node(t_token_list *token_list,
					t_token_node *new_node, t_token_type type,
					const char *value)
{
	if (!new_node)
	{
		free_token_list(token_list);
		return (return_mem_alloc_error());
	}
	new_node->token.type = type;
	new_node->token.value = ft_strdup(value);
	if (!new_node->token.value)
	{
		free_token_list(token_list);
		free(new_node);
		return (return_mem_alloc_error());
	}
	new_node->next = NULL;
	return (new_node);
}
