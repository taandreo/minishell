#include "minishell_bonus.h"

void	parser_loop(char **split_input, t_grammar *grammar);


void	tokenizer_bonus(char *input, t_grammar *grammar)
{
	char	**split_input;

	split_input = ft_split(input, ' ');
	parser_loop(split_input, grammar, word_count);
}

void	parser_loop(char **split_input, t_grammar *grammar)
{
	size_t	i;

	i = 0;
	while (split_input[i])
	{
		if (!grammar->has_command)
		{
			if (is_builtin(split_input[i], grammar))
				handle_builtin(split_input, grammar, &i);
			else if (is_command(split_input[i], grammar))
				handle_command(split_input, grammar, &i);
			else
				handle_not_command_error(split_input[i]);
		}
		else
		{
			if (is_redirections(split_input[i]))
				handle_redirections(split_input, grammar, &i);
			if (is_pipe_or_bonus_operators(split_input[i]))
				handle_pipes_and_bonus_operators(split_input, grammar, &i);
		}
		i++;
	}
}

void	handle_redirections(char **split_input, t_grammar *grammar, size_t *i)
{
	if (ft_strcmp("<", split_input[*i]) == 0)
		handle_input_redirection(split_input, grammar, i);
	if (ft_strcmp(">", split_input[*i]) == 0)
		handle_output_redirection(split_input, grammar, i);
	if (ft_strcmp(">>", split_input[*i]) == 0)
		handle_output_append(split_input, grammar, i);
	if (ft_strcmp("<<", split_input[*i]) == 0)
		handle_heredoc(split_input, grammar, i);
}
