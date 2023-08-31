#include "minishell_bonus.h"

int	add_builtin_or_command(char *string, t_token_list **tokens,
							  t_token_flags *flags)
{
	int exit_status;

	if (is_builtin(string))
		exit_status = add_token(tokens, get_builtin_token(string),string);
	else
		exit_status = add_token(tokens, TOKEN_COMMAND_NAME, string);
	flags->is_command = false;
	flags->has_command = true;
	return (exit_status);
}

int	add_filename_or_string(char *string, t_token_list **tokens,
							  t_token_flags *flags)
{
	int	exit_status;

	if (flags->has_heredoc)
		exit_status = add_token(tokens, TOKEN_STRING, string);
	else
	{
		if (flags->inside_quotes)
		{
			exit_status = add_token(tokens, TOKEN_FILENAME, string);
			flags->inside_quotes = false;
		}
		else
		{
			if (ft_strchr(string, ' ') || ft_strlen(string) == 0)
			{
				ft_dprintf(STDERR_FILENO, "minishell: %s: ambiguos redirect\n", flags->var);
				free_token_list(tokens);
				return (GENERAL_ERROR);
			}
			else
				exit_status = add_token(tokens, TOKEN_FILENAME, string);
		}
	}
	flags->is_redirection = false;
	if (!flags->has_command)
		flags->is_command = true;
	return (exit_status);
}
