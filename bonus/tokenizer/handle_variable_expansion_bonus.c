/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable_expansion_bonus.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:30:56 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:20 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	set_flags_variables(char *input, size_t pos, t_token_flags *flags);
t_bool	add_token_string(t_token_list **tokens, t_token_flags *flags);

t_bool	has_variable_expansion(char **input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	if ((*input)[*pos + 1] == '?')
		return (handle_special_case_variable(*input, pos, tokens, flags));
	if (strings_handle_variable_expansion(input, pos, flags) != SUCCESS)
		return (false);
	return (flags->string != NULL);
}

t_bool	handle_special_case_variable(char *input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	if (flags->string && ft_strlen(flags->string) > 0)
		if (!add_command_or_string(tokens, flags, input, pos))
			return (return_mem_alloc_error() != NULL);
	if (add_token(tokens, TOKEN_EXIT_CODE, "$?") != SUCCESS)
	{
		ft_dprintf(STDERR_FILENO, "Error: Memory allocation failed\n");
		return (free_nullify_and_return_null(&flags->string) != NULL);
	}
	free_str_and_nullify(&flags->string);
	*pos += 2;
	flags->has_exit_code = true;
	return (true);
}

t_bool	add_command_or_string(t_token_list **tokens, t_token_flags *flags,
			char *input, size_t *pos)
{
	if (flags->is_command)
	{
		if (add_token(tokens, TOKEN_COMMAND_NAME, flags->string) != SUCCESS)
			return (free_nullify_and_return_null(&flags->string) != NULL);
		if (*pos + 1 < ft_strlen(input))
			set_flags_variables(input, *pos, flags);
	}
	else if (flags->is_redirection)
	{
		if (flags->has_heredoc)
			return (add_token_string(tokens, flags));
		else
		{
			if (add_token(tokens, TOKEN_FILENAME, flags->string)!= SUCCESS)
				return (free_nullify_and_return_null(&flags->string) != NULL);
			if (*pos + 1 < ft_strlen(input))
				set_flags_variables(input, *pos, flags);
		}
	}
	else
		return (add_token_string(tokens, flags));
	return (true);
}

void	set_flags_variables(char *input, size_t pos, t_token_flags *flags)
{
	if (flags->is_command)
	{
		if (!input[pos + 1] || input[pos + 1] == ' ')
		{
			flags->is_command = false;
			flags->has_command = true;
		}
	}
	else if (flags->is_redirection)
	{
		if (!input[pos + 1] || input[pos + 1] == ' ')
		{
			flags->is_redirection = false;
			if (!flags->has_command)
				flags->is_command = true;
		}
	}
}

t_bool	add_token_string(t_token_list **tokens, t_token_flags *flags)
{
	if (add_token(tokens, TOKEN_STRING, flags->string) != SUCCESS)
		return (free_nullify_and_return_null(&flags->string) != NULL);
	return (true);
}
