#include "minishell_bonus.h"

void	init_command_part_fields(t_command_part *command_part)
{
	command_part->type = TOKEN_INVALID;
	command_part->u_cmd.builtin_command = NULL;
	command_part->u_cmd.command_name = NULL;
	command_part->arguments = NULL;
	command_part->redirections = NULL;
}