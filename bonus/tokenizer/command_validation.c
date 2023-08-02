#include "minishell_bonus.h"

t_bool	is_builtin(char *token, t_grammar *grammar)
{
	size_t	i;

	i = 0;
	while (i < NBR_BUILTINS)
	{
		if (ft_strcmp(token, grammar->builtins[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

t_bool	is_command(char *token)
{
	struct stat stats;

	if (stat(token, &stats) == -1) {
		ft_dprintf(2,"Failed to get file status");
		exit (1);
	}
	if (S_ISDIR(stats.st_mode))
	{
		ft_dprintf(2, "minishell: %s: Is a directory", token);
		exit(NOTEXEC);
	}
	if (access(token, F_OK) == 0 && access(token, X_OK) == -1)
	{
		ft_dprintf(2, "minishell: %s: Permission denied", token);
		exit(NOTEXEC);
	}
	if (access(token, F_OK) == -1)

}