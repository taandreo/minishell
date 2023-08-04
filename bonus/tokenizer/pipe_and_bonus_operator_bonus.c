#include "minishell_bonus.h"

t_bool	is_pipe_or_bonus_operators(char *token)
{
	if (ft_strcmp("|", token) == 0
		|| ft_strcmp("||", token) == 0
		|| ft_strcmp("&&", token) == 0)
		return (true);
	return (false);
}


