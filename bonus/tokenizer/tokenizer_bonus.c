#include "minishell_bonus.h"

void	parser_loop(char **split_input, t_grammar *grammar,
					t_parse_tree *parse_tree);
void	init_parse_tree(t_parse_tree *parse_tree);
void	handle_first_tokens(char **split_input, t_grammar *grammar,
							t_parse_tree *parse_tree, size_t *i);



void	tokenizer(char *input, )
{
	char	**split_input;

	split_input = ft_split(input, ' ');
	parser_loop(split_input, grammar, parse_tree);
}

void	parser_loop(char **split_input, t_grammar *grammar,
					t_parse_tree *parse_tree)
{
	size_t	i;

	i = 0;
	while (split_input[i])
	{
		if (!grammar->has_command)
			handle_first_tokens(split_input, grammar, parse_tree, &i);
		else
		{
			if (is_redirections(split_input[i]))
				handle_redirections(split_input, grammar, parse_tree, &i);
			if (is_pipe_or_bonus_operators(split_input[i]))
				handle_pipes_and_bonus_operators(split_input, grammar, &i);
		}
		i++;
	}
}

void	handle_first_tokens(char **split_input, t_grammar *grammar,
							t_parse_tree *parse_tree, size_t *i)
{
	if (ft_strcmp(split_input[0], "|") == 0
		|| ft_strcmp(split_input[0], "||") == 0
		|| ft_strcmp(split_input[0], "&&") == 0)
		syntax_error(split_input[0]);
	if (is_redirections(split_input[*i]))
		handle_redirections(split_input, grammar, parse_tree, i);
	else if (is_builtin(split_input[*i], grammar))
		handle_builtin(split_input, grammar, i);
	else if (is_command(split_input[*i], grammar))
		handle_command(split_input, grammar, i);
	else
		handle_not_command_error(split_input[*i]);
}

void	handle_redirections(char **split_input, t_grammar *grammar,
							t_parse_tree *parse_tree, size_t *i)
{
	if (ft_strncmp("<", split_input[*i], 1) == 0)
		handle_input_redirection(split_input, grammar, parse_tree, i);
	else if (ft_strncmp(">", split_input[*i], 1) == 0)
		handle_output_redirection(split_input, grammar, parse_tree, i);
	else if (ft_strncmp(">>", split_input[*i], 2) == 0)
		handle_output_append(split_input, grammar, i);
	else if (ft_strncmp("<<", split_input[*i], 2) == 0)
		handle_heredoc(split_input, grammar, i);
	if (!is_pipe_or_bonus_operators(split_input[*i]))
		append_command_arguments(split_input, grammar, i);
}
