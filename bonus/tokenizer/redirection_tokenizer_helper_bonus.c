#include "minishell_bonus.h"

int	add_token_1_pos(size_t *pos, t_token_list *tokens,
					   t_token_type type, t_token_flags *flags)
{
	(*pos)++;
	flags->has_heredoc = false;
	if (type == TOKEN_REDIRECTION_INPUT)
		return (add_token(tokens, type, "<"));
	return (add_token(tokens, type, ">"));
}

int	add_token_2_pos(size_t *pos, t_token_list *tokens,
			t_token_type type, t_token_flags *flags)
{
	*pos += 2;
	if (type == TOKEN_REDIRECTION_HEREDOC)
	{
		flags->has_heredoc = true;
		return (add_token(tokens, type, "<<"));
	}
	return (add_token(tokens, type, ">>"));
}
