#include "minishell_bonus.h"

t_bool	is_redirections(char *token)
{
	if (strcmp(token, "<") == 0
		|| strcmp(token, ">") == 0
		|| strcmp(token, "<<") == 0
		|| strcmp(token, ">>") == 0)
		return (true);
	return (false);
}
