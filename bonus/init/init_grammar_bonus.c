#include "minishell_bonus.h"

void	init_grammar(t_grammar *grammar)
{
	grammar->command = NULL;
	grammar->arguments = NULL;
	grammar->redirection = NULL;
	grammar->command_part = NULL;
	grammar->file_name = NULL;
	grammar->string = NULL;
	grammar->tag = NULL;
	grammar->command_name = NULL;
	grammar->path = NULL;
	grammar->has_command = false;
	grammar->has_double_quotes = false;
	grammar->has_single_quotes = false;
	grammar->has_pipe = false;
	grammar->has_redirection = false;
	grammar->has_heredoc = false;
	grammar->has_append = false;
	init_builtins(grammar);
}

void	init_builtins(t_grammar *grammar)
{
	int i;

	i = 0;
	grammar->builtins = ft_calloc(8, sizeof(char *));
	if (!grammar->builtins)
	{
		ft_dprintf(STDERR_FILENO, "Malloc Error\n");
		exit(1);
	}
	while (i < 7)
	{
		grammar->builtins[i] = ft_calloc(7, sizeof(char));
		if (!grammar->builtins[i]) {
			ft_dprintf(STDERR_FILENO, "Malloc Error\n");
			exit(1);
		}
		i++;
	}
	copy_builtins_values(grammar);
}

void	copy_builtins_values(t_grammar *grammar)
{
	ft_strlcpy(grammar->builtins[0], "echo", 5);
	ft_strlcpy(grammar->builtins[1], "cd", 3);
	ft_strlcpy(grammar->builtins[2], "pwd", 4);
	ft_strlcpy(grammar->builtins[3], "export", 7);
	ft_strlcpy(grammar->builtins[4], "unset", 6);
	ft_strlcpy(grammar->builtins[5], "env", 4);
	ft_strlcpy(grammar->builtins[6], "exit", 5);
}