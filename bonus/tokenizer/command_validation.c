#include "minishell_bonus.h"

t_bool	is_builtin(char *token, t_grammar *grammar)
{
	size_t	i;

	i = 0;
	while (i < BUILTINS)
	{
		if (ft_strcmp(token, grammar->builtins[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

t_bool	is_command(char *token)
{

}