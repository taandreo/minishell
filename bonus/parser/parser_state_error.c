#include "minishell_bonus.h"

void	*misuse_state_error(t_parser_state *state)
{
	state->status = MISUSE;
	state->error = true;
	return (NULL);
}

void	*print_misuse_state_error(t_parser_state *state)
{
	state->status = MISUSE;
	state->error = true;
	return (return_mem_alloc_error());
}
