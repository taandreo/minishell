#include "minishell_bonus.h"

t_bool	is_builtin(char *token)
{
	if (ft_strcmp(token, "echo") == 0
		|| ft_strcmp(token, "cd") == 0
		|| ft_strcmp(token, "pwd") == 0
		|| ft_strcmp(token, "export") == 0
		|| ft_strcmp(token, "unset") == 0
		|| ft_strcmp(token, "env") == 0
		|| ft_strcmp(token, "exit") == 0)
		return (true);
	return (false);
}

t_bool	decrease_len(t_token_flags *flags)
{
	if (flags->var_len > -1)
		flags->var_len--;
	return (true);
}
