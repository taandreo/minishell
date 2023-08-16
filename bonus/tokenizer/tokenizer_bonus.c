#include "minishell_bonus.h"

char *handle_quotes(const char *input, size_t *position);

t_token_list	tokenizer(char *input, size_t input_len)
{
	t_token_list	tokens;
	size_t			position;
	char			c;
	t_bool			is_command;
	t_bool			is_redirection;

	position = 0;
	is_command = true;
	is_redirection = false;
	tokens = create_token_list();
	while (position < input_len)
	{
		while (ft_is_space(c))
			position++;
		if (has_quotes(c))
			add_token(&tokens, TOKEN_STRING, handle_quotes(input, &position));
		position++;
	}
}

char	*handle_quotes(const char *input, size_t *position)
{
	char	quote_type;
	size_t	start;
	size_t	length;
	char 	*quoted_string;

	quote_type = input[*position];
	start = *position + 1;
	(*position)++;
	while (input[*position] && input[*position] != quote_type) {
		if (quote_type == '\"' && input[*position] == '$')
		{
			// TODO: Variable Expansion
			(*position)++;
		}
		(*position)++;
	}
	if (input[*position] != quote_type)
	{
		ft_dprintf(2, "Error: Unclosed quote.");
		exit (1);
	}
	length = *position - start;
	quoted_string = ft_strndup(input + start, length);
	if (!quoted_string)
	{
		ft_dprintf(2, "Error: Unable to allocate memory.");
		exit (1);
	}
	(*position)++;
	return (quoted_string);
}

//void	parser_loop(char **split_input, t_grammar *grammar,
//					t_parse_tree *parse_tree)
//{
//	size_t	i;
//
//	i = 0;
//	while (split_input[i])
//	{
//		if (!grammar->has_command)
//			handle_first_tokens(split_input, grammar, parse_tree, &i);
//		else
//		{
//			if (is_redirections(split_input[i]))
//				handle_redirections(split_input, grammar, parse_tree, &i);
//			if (is_pipe_or_bonus_operators(split_input[i]))
//				handle_pipes_and_bonus_operators(split_input, grammar, &i);
//		}
//		i++;
//	}
//}
//
//void	handle_first_tokens(char **split_input, t_grammar *grammar,
//							t_parse_tree *parse_tree, size_t *i)
//{
//	if (ft_strcmp(split_input[0], "|") == 0
//		|| ft_strcmp(split_input[0], "||") == 0
//		|| ft_strcmp(split_input[0], "&&") == 0)
//		syntax_error(split_input[0]);
//	if (is_redirections(split_input[*i]))
//		handle_redirections(split_input, grammar, parse_tree, i);
//	else if (is_builtin(split_input[*i], grammar))
//		handle_builtin(split_input, grammar, i);
//	else if (is_command(split_input[*i], grammar))
//		handle_command(split_input, grammar, i);
//	else
//		handle_not_command_error(split_input[*i]);
//}
//
//void	handle_redirections(char **split_input, t_grammar *grammar,
//							t_parse_tree *parse_tree, size_t *i)
//{
//	if (ft_strncmp("<", split_input[*i], 1) == 0)
//		handle_input_redirection(split_input, grammar, parse_tree, i);
//	else if (ft_strncmp(">", split_input[*i], 1) == 0)
//		handle_output_redirection(split_input, grammar, parse_tree, i);
//	else if (ft_strncmp(">>", split_input[*i], 2) == 0)
//		handle_output_append(split_input, grammar, i);
//	else if (ft_strncmp("<<", split_input[*i], 2) == 0)
//		handle_heredoc(split_input, grammar, i);
//	if (!is_pipe_or_bonus_operators(split_input[*i]))
//		append_command_arguments(split_input, grammar, i);
//}
