#include "minishell_bonus.h"

void	sorted_insert(t_string **head_ref, t_string *new_node)
{
	t_string	*current;

	if (*head_ref == NULL || strcmp((*head_ref)->value, new_node->value) >= 0)
	{
		new_node->next = *head_ref;
		*head_ref = new_node;
	} else
	{
		current = *head_ref;
		while (current->next != NULL
			&& strcmp(current->next->value, new_node->value) < 0)
			current = current->next;
		new_node->next = current->next;
		current->next = new_node;
	}
}

t_string	*add_new_node(char *filename, t_vars *vars,	t_token_type type)
{
	t_string *new_node;

	new_node = malloc(sizeof(t_string));
	if (!new_node)
	{
		print_mem_alloc_error();
		vars->state.status = MISUSE;
		return (NULL);
	}
	new_node->type = type;
	new_node->value = ft_strdup(filename);
	if (!new_node->value)
	{
		print_mem_alloc_error();
		vars->state.status = MISUSE;
		return (NULL);
	}
	return (new_node);
}
