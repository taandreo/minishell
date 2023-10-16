/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_tokenizer_helper_bonus.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:31:04 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:17 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_bool	handle_character(char **input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags);
char	*process_quotes(char **input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags);

char	*get_string_from_input(char **input, size_t *pos,
			t_token_list **tokens, t_token_flags *flags)
{
	const char	*start;

	if (!flags->string)
		flags->string = ft_strdup("");
	start = (*input) + *pos;
	if (!flags->string)
		return (error_and_exit_code_false(flags));
	if (!flags->var)
		flags->var = initialize_var_string((*input), *pos, flags, start);
	if (!flags->var)
		return (error_and_exit_code_false(flags));
	while (decrease_len(flags) && (*input)[*pos]
			&& is_string_start((*input)[*pos], flags))
	{
		if (!handle_character(input, pos, tokens, flags))
			return (null_exit_code_false(flags));
	}
	return (flags->string);
}

char	*process_quotes(char **input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags)
{
	char		*tmp;
	const char	*start;

	start = *input + *pos;
	tmp = ft_strndup(start, *input + *pos - start);
	if (!tmp)
		return (free_str_nullify_and_malloc_error(&flags->string));
	flags->string = join_and_cleanup(&flags->string, &tmp);
	if (!flags->string)
		return (free_str_nullify_and_malloc_error(&tmp));
	tmp = handle_quotes(input, pos, tokens, flags);
	if (!tmp)
		return (free_and_return_null(flags->string));
	return (tmp);
}

char	*join_flags_string(char **flags_string, char **buffer)
{
	if (*flags_string)
		*flags_string = join_and_cleanup(flags_string, buffer);
	else
		*flags_string = join_1st_and_cleanup(buffer, "");
	return (*flags_string);
}

t_bool	handle_character(char **input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags)
{
	char	*buffer;

	buffer = NULL;
	if (flags->var_len < 0)
	{
		if ((*input)[*pos] == '\'' || (*input)[*pos] == '\"')
		{
			flags->string = process_quotes(input, pos, tokens, flags);
			if (!flags->has_exit_code)
				return (flags->string != NULL);
			return (true);
		}
		if ((*input)[*pos] == '$' && !flags->has_heredoc)
			return (has_variable_expansion(input, pos, tokens, flags));
	}
	buffer = ft_strndup((*input) + *pos, 1);
	if (!buffer)
		return (free_nullify_and_return_mem_error(&flags->string) != NULL);
	flags->string = join_flags_string(&flags->string, &buffer);
	if (!flags->string)
		return (return_mem_alloc_error() != NULL);
	(*pos)++;
	return (true);
}

int	strings_handle_variable_expansion(char **input, size_t *pos,
			t_token_flags *flags)
{
	char	*tmp;
	char	*to_be_tokenized;
	char	*start;

	flags->inside_quotes = false;
	tmp = expand_variable_string(*input, pos, flags);
	if (!tmp)
	{
		free_str_nullify_and_malloc_error(&flags->string);
		return (MISUSE);
	}
	flags->var_len = ft_strlen(tmp);
	start = *input + *pos;
	to_be_tokenized = ft_strjoin(tmp, start);
	if (!to_be_tokenized)
	{
		free_2_str_and_nullify(&flags->string, &tmp);
		print_mem_alloc_error();
		return (MISUSE);
	}
	free(*input);
	free(tmp);
	*input = to_be_tokenized;
	*pos = 0;
	return (SUCCESS);
}
