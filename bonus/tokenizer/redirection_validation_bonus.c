#include "minishell_bonus.h"

t_bool	is_redirections(char *token)
{
	if (ft_strncmp(token, "<", 1) == 0
		|| ft_strncmp(token, ">", 1) == 0
		|| ft_strncmp(token, "<<", 2) == 0
		|| ft_strncmp(token, ">>", 2) == 0)
		return (true);
	return (false);
}
