#include "minishell_bonus.h"

void	handle_not_command_error(char *token)
{
	ft_dprintf(2, "%s: command not found", token);
	exit(CMDNFND);
}
