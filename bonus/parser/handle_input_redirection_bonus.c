#include "minishell_bonus.h"
void	check_input_redirection_error(char **split_input, const size_t *i);
void	syntax_error(char *token);
void	handle_input_redirection(char **split_input, t_grammar *grammar
								 , size_t *i)
{
	check_input_redirection_error(split_input, i);
	grammar->has_input_redirection = true;
	grammar->redirection = "<";
	while (split_input[*i] && !is_pipe_or_bonus_operators(split_input[*i]))
	{
		if (ft_strcmp(split_input[*i], "<") == 0)
		{
			if (ft_strcmp(split_input[*i - 1], "<") == 0)
				syntax_error(split_input[*i]);
			check_input_redirection_error(split_input, i);
		}
		else if (ft_strcmp(split_input[*i], ">") == 0
			|| ft_strcmp(split_input[*i], "<<") == 0
			|| ft_strcmp(split_input[*i], ">>") == 0)
		{
			if (ft_strcmp(split_input[*i - 1], "<") == 0)
				syntax_error(split_input[*i]);
			(*i)--;
			break ;
		}
		else
			grammar->input_file_name = split_input[*i];
		(*i)++;
	}
}

void	check_input_redirection_error(char **split_input, const size_t *i)
{
	if (access(split_input[*i + 1], F_OK) == -1)
	{
		ft_dprintf(2,"minishell: %s:No such file or directory\n", split_input[*i + 1]);
		exit (GENERAL_ERROR);
	}
}

void	syntax_error(char *token)
{
	ft_dprintf(2,"minishell: syntax error near unexpected token `%s'\n", token);
	exit (MISUSE);
}
