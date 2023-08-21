#include "minishell_bonus.h"

t_command	*parse(t_token_list *tokens)
{
	t_command	*parse_tree;

	parse_tree = parse_command(tokens);
	if (!parse_tree || current_token_type(tokens) != TOKEN_END)
	{
		if (parse_tree)
		{
			free_command(parse_tree);
			return_syntax_error(current_token(tokens).value);
		}
		exit(2);
	}
	return (parse_tree);
}
