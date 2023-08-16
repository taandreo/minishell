#include "minishell_bonus.h"

void	handle_input_redirection(char **split_input, t_grammar *grammar,
								t_parse_tree *parse_tree, size_t *i)
{
	(*i)++;
	if (is_redirections(split_input[*i]))
		syntax_error(split_input[*i]);
	if (access(split_input[*i], F_OK) == -1)
	{
		ft_dprintf(2, "minishell: %s: No such file or directory\n",
			split_input[*i]);
		exit (GENERAL_ERROR);
	}
	grammar->input_file_name = split_input[*i];
	(*i)++;
	if (is_redirections(split_input[*i]))
		handle_redirections(split_input, grammar, parse_tree, i);
}

void	handle_output_redirection(char **split_input, t_grammar *grammar,
								 t_parse_tree *parse_tree, size_t *i)
{
	grammar->has_input_redirection = true;
	grammar->redirection = ">";
	(*i)++;
	if (is_redirections(split_input[*i]))
		syntax_error(split_input[*i]);
	grammar->output_file_name = split_input[*i];

	(*i)++;
	if (is_redirections(split_input[*i]))
		handle_redirections(split_input, grammar, i);
}

void	syntax_error(char *token)
{
	ft_dprintf(2,"minishell: syntax error near unexpected token `%s'\n", token);
	exit (MISUSE);
}
