#include "minishell_bonus.h"

size_t	count_words(const char *s, char c);
void	parser_loop(char **split_input, t_grammar *grammar, size_t word_count);
void	handle_redirections(char **split_input, t_grammar *grammar, size_t *i);

void	tokenizer_bonus(char *input, t_grammar *grammar)
{
	size_t	word_count;
	char	**split_input;

	word_count = count_words(input, ' ');
	split_input = ft_split(input, ' ');
	parser_loop(split_input, grammar, word_count);
}

void	parser_loop(char **split_input, t_grammar *grammar, size_t word_count)
{
	size_t	i;

	i = 0;
	while (i < word_count)
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
			else if (ft_strcmp("|", split_input[i]) == 0
				|| ft_strcmp("||", split_input[i]) == 0
				|| ft_strcmp("&&", split_input[i]) == 0)
				handle_pipes_and_bonus_operators(split_input, grammar, &i);
		}
		i++;
	}
}

void	handle_redirections(char **split_input, t_grammar *grammar, size_t *i)
{
	if (ft_strcmp("<", split_input[i]) == 0)
		handle_input_redirection(split_input, grammar, &i);
	else if (ft_strcmp(">", split_input[i]) == 0)
		handle_output_redirection(split_input, grammar, &i);
	else if (ft_strcmp(">>", split_input[i]) == 0)
		handle_output_append(split_input, grammar, &i);
	else if (ft_strcmp("<<", split_input[i]) == 0)
		handle_heredoc(split_input, grammar, &i);
}

size_t	count_words(const char *s, char c)
{
	size_t	word_count;

	word_count = 0;
	while (*s)
	{
		if (*s != c)
		{
			while (*s && *s != c)
				s++;
			word_count++;
			s--;
		}
		s++;
	}
	return (word_count);
}
