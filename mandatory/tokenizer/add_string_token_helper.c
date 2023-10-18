/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_string_token_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:30:49 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:25 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_hdoc_filename_or_error(char *string, t_token_list **tokens,
		t_token_flags *flags);

int	add_builtin_or_command(char *string, t_token_list **tokens,
		t_token_flags *flags, char next)
{
	int		exit_status;

	exit_status = SUCCESS;
	if (string)
	{
		if (is_builtin(string))
			exit_status = add_token(tokens, get_builtin_token(string), string);
		else if (flags->inside_quotes && flags->quote_type == '\"')
			exit_status = add_token(tokens, TOKEN_COMMAND_NAME_QUOTES, string);
		else
		{
			if (ft_strlen(string) != 0)
				exit_status = add_token(tokens, TOKEN_COMMAND_NAME_UNQUOTES,
						string);
		}
	}
	if (!next || next == ' ')
	{
		if (check_if_is_cmd_true(string, tokens))
		{
			flags->is_command = false;
			flags->has_command = true;
		}
	}
	return (exit_status);
}

int	add_filename_or_string(char *string, t_token_list **tokens,
		t_token_flags *flags, char next)
{
	int	exit_status;

	exit_status = SUCCESS;
	if (string)
		exit_status = add_hdoc_filename_or_error(string, tokens, flags);
	if (!next || next == ' ')
	{
		flags->is_redirection = false;
		if (!flags->has_command)
			flags->is_command = true;
	}
	return (exit_status);
}

int	check_ambiguous_redirect(char *string, t_token_list **tokens,
		t_token_flags *flags)
{
	if (flags->var_len > -1 || ft_strlen(string) == 0)
	{
		write(STDERR_FILENO, "minishell: ", ft_strlen("minishell: "));
		write(STDERR_FILENO, flags->var, ft_strlen(flags->var));
		write(STDERR_FILENO, ": ambiguous redirect\n",
			ft_strlen(": ambiguous redirect\n"));
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

int	add_hdoc_filename_or_error(char *string, t_token_list **tokens,
		t_token_flags *flags)
{
	int	status;

	if (flags->has_heredoc)
	{
		status = add_token(tokens, TOKEN_STRING, string);
		flags->has_heredoc = false;
	}
	else
	{
		if (flags->inside_quotes)
		{
			status = add_token(tokens, TOKEN_FILENAME, string);
			flags->inside_quotes = false;
		}
		else
			status = check_ambiguous_redirect(string, tokens, flags);
	}
	return (status);
}
