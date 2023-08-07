#include "minishell_bonus.h"
void	check_input_redirection_error(char **split_input, const size_t *i);
void	syntax_error(char *token);
void	handle_input_redirection(char **split_input, t_grammar *grammar
								 , size_t *i)
{
	grammar->has_input_redirection = true;
	grammar->redirection = "<";
	(*i)++;
	if (is_redirections(split_input[*i]))
		syntax_error(split_input[*i]);
	check_input_redirection_error(split_input, i);
	grammar->input_file_name = split_input[*i];
	(*i)++;
	if (is_redirections(split_input[*i]))
		handle_redirections(split_input, grammar, i);
}

void	check_input_redirection_error(char **split_input, const size_t *i)
{
	if (access(split_input[*i], F_OK) == -1)
	{
		ft_dprintf(2,"minishell: %s: No such file or directory\n", split_input[*i]);
		exit (GENERAL_ERROR);
	}
}

void	syntax_error(char *token)
{
	ft_dprintf(2,"minishell: syntax error near unexpected token `%s'\n", token);
	exit (MISUSE);
}
