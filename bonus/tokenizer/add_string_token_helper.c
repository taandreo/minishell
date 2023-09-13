#include "minishell_bonus.h"

int	add_builtin_or_command(char *string, t_token_list **tokens,
		t_token_flags *flags)
{
	int		exit_status;

	exit_status = SUCCESS;
	if (string)
	{
		if (is_builtin(string))
			exit_status = add_token(tokens, get_builtin_token(string), string);
		else
			exit_status = add_token(tokens, TOKEN_COMMAND_NAME, string);
	}
	if (!flags->has_exit_code)
	{
		flags->is_command = false;
		flags->has_command = true;
	}
	return (exit_status);
}

int	add_filename_or_string(char *string, t_token_list **tokens,
		t_token_flags *flags)
{
	int	exit_status;

	exit_status = SUCCESS;
	if (string)
	{
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
				exit_status = check_ambiguous_redirect(string, tokens, flags);
		}
	}
	flags->is_redirection = false;
	if (!flags->has_command)
		flags->is_command = true;
	return (exit_status);
}

int	check_ambiguous_redirect(char *string, t_token_list **tokens,
		t_token_flags *flags)
{
	if (flags->var_len > -1 || ft_strlen(string) == 0)
	{
		ft_dprintf(STDERR_FILENO,
				"minishell: %s: ambiguous redirect\n", flags->var);
		free_token_list(tokens);
		return (GENERAL_ERROR);
	}
	else
		return (add_token(tokens, TOKEN_FILENAME, string));
}

void	*add_unclosed_quotes_token(t_token_list **tokens, t_token_flags *flags,
			char *quoted_string)
{
	add_token(tokens, TOKEN_ERROR, "minishell: Unclosed quotes");
	if (flags->var)
	{
		free(flags->var);
		flags->var = NULL;
	}
	if (quoted_string)
	{
		free(quoted_string);
		flags->string = NULL;
	}
	return (NULL);
}

int	add_special_or_string(char *string, t_token_list **tokens)
{
	if ((*tokens)->tail->token.type == TOKEN_ECHO
			&& ft_strcmp(string, "-n") == 0)
		return (add_token(tokens, TOKEN_SPECIAL_ARG, string));
	else
		return (add_token(tokens, TOKEN_STRING, string));
}
