#include "minishell_bonus.h"

t_token_list	create_token_list(void)
{
	t_token_list	token_list;

	token_list.head = NULL;
	token_list.tail = NULL;
	token_list.count = 0;
	return (token_list);
}

void	add_token(t_token_list *token_list, t_token_type type,
			const char *value)
{
	t_token_node	*new_node;

	new_node = (t_token_node *)malloc(sizeof(t_token_node));
	new_node->token.type = type;
	new_node->token.value = ft_strdup(value);
	new_node->next = NULL;
	if (token_list->tail)
		token_list->tail->next = new_node;
	else
		token_list->head = new_node;
	token_list->tail = new_node;
	token_list->count++;
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
