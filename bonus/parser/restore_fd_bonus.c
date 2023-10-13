#include "minishell_bonus.h"

void restore_stdout(int saved_stdout, t_vars *vars)
{
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		write(STDERR_FILENO, "minishell: dup2",
				ft_strlen("minishell: dup2"));
		perror("");
		vars->state.error = true;
		vars->state.status = GENERAL_ERROR;
	}
}

void restore_stdin(int saved_stdin, t_vars *vars)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
	{
		write(STDERR_FILENO, "minishell: dup2",
				ft_strlen("minishell: dup2"));
		perror("");
		vars->state.error = true;
		vars->state.status = GENERAL_ERROR;
	}
}
