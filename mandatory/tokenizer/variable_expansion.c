/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:21 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/28 21:44:00 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_valid_var_name(char c);
char	*lookup_variable(char *var);
char	*advance_position(char *input, size_t *pos,
			t_token_flags *flags);

char	*expand_variable_string(char *input, size_t *pos, t_token_flags *flags)
{
	size_t	start_pos;
	size_t	len;
	char	*var;
	char	*str;

	len = 0;
	start_pos = *pos + 1;
	if (!ft_isalpha(input[start_pos]) && input[start_pos] != '_')
		return (advance_position(input, pos, flags));
	str = NULL;
	while (input[start_pos + len] && is_valid_var_name(input[start_pos + len]))
		len++;
	var = ft_strndup(input + start_pos, len);
	if (!var)
		return (NULL);
	str = lookup_variable(var);
	free(var);
	*pos = start_pos + len;
	return (str);
}

t_bool	is_valid_var_name(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*lookup_variable(char *var)
{
	char	*value;
	char	*new_str;

	new_str = NULL;
	value = get_env(var);
	if (!value)
		return (ft_strdup(""));
	new_str = ft_strdup(value);
	if (!new_str)
		return (NULL);
	return (new_str);
}

char	*advance_position(char *input, size_t *pos, t_token_flags *flags)
{
	char	*var;
	size_t	dollar;

	dollar = *pos;
	flags->init_var = true;
	while (is_string_start(input[*pos], flags))
	{
		(*pos)++;
		if (input[*pos] && (input[*pos]) == flags->quote_type)
		{
			(*pos)--;
			break ;
		}
	}
	if (input[*pos] == '\0' && flags->inside_quotes)
		(*pos)--;
	var = get_var_value(input, pos, dollar, flags);
	flags->init_var = false;
	if (!var)
		return (NULL);
	return (var);
}

char	*quotes_handle_variable_expansion(char **input, size_t *pos,
			t_token_flags *flags)
{
	char	*tmp;

	flags->inside_quotes = true;
	tmp = expand_variable_string(*input, pos, flags);
	if (!tmp)
	{
		if (flags->string)
		{
			free(flags->string);
			flags->string = NULL;
		}
		return (NULL);
	}
	return (tmp);
}
