#include "minishell_bonus.h"

t_bool	current_token_match(char **position, t_string *current, int *match);
t_bool	next_tokens_match(char **position, t_string *current, int *match);
t_bool	match_logic(char *position, t_string *string);

t_string *expand_wildcard(t_string *string, t_vars *vars, t_token_type type)
{
	t_string *result_head;
	t_string *new_node;
	struct dirent *entry;

	DIR *dir = opendir(".");
	if (!dir)
	{
		perror("minishell: opendir: Error while opening the current directory");
		return (NULL);
	}
	result_head = NULL;
	while ((entry = readdir(dir)) != NULL)
	{
		if (match_logic(entry->d_name, string))
		{
			new_node = add_new_node(entry->d_name, vars, type);
			if (!new_node)
				return (free_resources_and_return_null(dir, result_head));
			sorted_insert(&result_head, new_node);
		}
	}
	closedir(dir);
	return (result_head);
}

t_bool	match_logic(char *position, t_string *string)
{
	t_string *current;
	int match;

	current = string;
	match = 1;
	while (current && match)
	{
		if (current->type == TOKEN_WILDCARD)
		{
			if (!current->next)
				break ;
			if (!next_tokens_match(&position, current, &match))
				break ;
		}
		else
		{
			if (!current_token_match(&position, current, &match))
				break ;
		}
		current = current->next;
	}
	return (match && (!position || *position == '\0'
			|| (current && current->type == TOKEN_WILDCARD)));
}

t_bool	current_token_match(char **position, t_string *current, int *match)
{
	if (ft_strncmp(*position, current->value,
			ft_strlen(current->value)) == 0)
		*position += ft_strlen(current->value);
	else
	{
		*match = 0;
		return (false);
	}
	return (true);
}

t_bool	next_tokens_match(char **position, t_string *current, int *match)
{
	if (current->next && current->next->type != TOKEN_WILDCARD
		&& current->next->next)
	{
		*position = ft_strnstr(*position, current->next->value,
				ft_strlen(*position));
		if (!*position) {
			*match = 0;
			return (false);
		}
	}
	else if (current->next && current->next->type != TOKEN_WILDCARD
				&& !current->next->next)
	{
		*position += ft_strlen(*position) - ft_strlen(current->next->value);
		if (ft_strncmp(*position, current->next->value,
				strlen(current->next->value)) != 0)
		{
			*match = 0;
			return (false);
		}
	}
	return (true);
}
