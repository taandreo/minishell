#include "minishell_bonus.h"

void	free_argument(t_argument *arg)
{
	if (arg)
	{
		free(arg->value);
		free(arg);
	}
}

void	free_arguments(t_arguments *args)
{
	if (args)
	{
		free_argument(args->argument);
		free_arguments(args->next);
		free(args);
	}
}

void	free_redirection(t_redirection *redir)
{
	if (redir)
	{
		free(redir->filename);
		free(redir);
	}
}

void	free_redirections(t_redirections *redirs)
{
	if (redirs)
	{
		free_redirection(redirs->redirection);
		free_redirections(redirs->next);
		free(redirs);
	}
}

void	free_builtin_command(t_builtin_command *cmd)
{
	if (cmd)
	{
		free_arguments(cmd->arguments);
		free(cmd);
	}
}