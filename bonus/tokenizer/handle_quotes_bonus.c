/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:30:54 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:21 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*extract_quoted_string(char **input, size_t *pos,
			t_token_list **tokens, t_token_flags *flags);
char	*add_char_and_advance_pos(char *input, size_t *pos);
char	*handle_question_mark(char *input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags);

char	*handle_quotes(char **input, size_t *position,
			t_token_list **tokens, t_token_flags *flags)
{
	flags->quote_type = (*input)[*position];
	if (!flags->string)
	{
		flags->string = ft_strdup("");
		if (!flags->string)
			return (return_mem_alloc_error());
	}
	flags->string = extract_quoted_string(input, position, tokens, flags);
	if (!flags->string && !flags->has_exit_code)
		return (return_mem_alloc_error());
	if ((*input)[*position] != flags->quote_type)
		return (add_unclosed_quotes_token(tokens, flags, flags->string));
	(*position)++;
	flags->init_var = true;
	if ((*input)[*position] && is_string_start((*input)[*position], flags))
	{
		flags->init_var = false;
		return (get_string_from_input(input, position, tokens, flags));
	}
	return (flags->string);
}

char	*extract_quoted_string(char **input, size_t *pos,
			t_token_list **tokens, t_token_flags *flags)
{
	char		*tmp;
	const char	*start;

	start = *input + *pos;
	if (!flags->var)
	{
		flags->var = initialize_var_string(*input, *pos, flags, start);
		if (!flags->var)
			return (NULL);
	}
	(*pos)++;
	while ((*input)[*pos] && (*input)[*pos] != flags->quote_type)
	{
		if (flags->quote_type == '\"' && (*input)[*pos] == '$')
			tmp = substitute_variable(*input, pos, tokens, flags);
		else
			tmp = add_char_and_advance_pos(*input, pos);
		if (!tmp)
			return (free_str_nullify_and_malloc_error(&flags->string));
		flags->string = join_and_cleanup(&flags->string, &tmp);
	}
	if (flags->has_exit_code && flags->string && ft_strlen(flags->string) == 0)
		free_str_and_nullify(&flags->string);
	return (flags->string);
}

char	*add_char_and_advance_pos(char *input, size_t *pos)
{
	char	*tmp;

	tmp = ft_strndup(input + *pos, 1);
	(*pos)++;
	return (tmp);
}

char	*substitute_variable(char *input, size_t *pos,
			t_token_list **tokens, t_token_flags *flags)
{
	char	*var;

	if (input[*pos + 1] == '?')
		var = handle_question_mark(input, pos, tokens, flags);
	else
		var = quotes_handle_variable_expansion(&input, pos, flags);
	return (var);
}

char	*handle_question_mark(char *input, size_t *pos, t_token_list **tokens,
		t_token_flags *flags)
{
	char	*var;

	var = ft_strdup("");
	if (!var)
		return (return_mem_alloc_error());
	if (flags->string && ft_strlen(flags->string) > 0)
	{
		if (!add_command_or_string(tokens, flags, input, pos))
			return (free_and_return_null(var));
		free(flags->string);
		flags->string = ft_strdup("");
	}
	if (add_token(tokens, TOKEN_EXIT_CODE, "$?") != SUCCESS)
		return (free_2_and_return_null(flags->string, var));
	*pos += 2;
	flags->has_exit_code = true;
	return (var);
}
