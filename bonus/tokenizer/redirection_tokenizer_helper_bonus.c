#include "minishell_bonus.h"

void	move_2_pos_and_add_token(size_t *pos, t_token_list *tokens,
			t_token_type type, const char *value)
{
	*pos += 2;
	add_token(tokens, type, value);
}

void	move_1_pos_and_add_token(size_t *pos, t_token_list *tokens,
			t_token_type type, const char *value)
{
	(*pos)++;
	add_token(tokens, type, value);
}