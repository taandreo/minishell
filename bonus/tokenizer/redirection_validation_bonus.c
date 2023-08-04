#include "minishell_bonus.h"

t_bool	is_redirections(char *token)
{
	if (ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, "<<") == 0
		|| ft_strcmp(token, ">>") == 0)
		return (true);
	return (false);
}
