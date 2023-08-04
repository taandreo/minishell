/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:32:36 by tairribe          #+#    #+#             */
/*   Updated: 2023/07/09 17:01:29 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <sys/stat.h>

# define NBR_BUILTINS 7
# define NOTEXEC 126
# define CMDNFND 127
# define GENERAL_ERROR 1
# define EXIT_OFFSET 128

typedef struct e_grammar
{
	char	*command;
	char	*arguments;
	char	**builtins;
	char 	*redirection;
	char 	*command_part;
	char	*file_name;
	char	*string;
	char 	*tag;
	char 	*command_name;
	char	*path;
	t_bool	has_command;
	t_bool	has_double_quotes;
	t_bool	has_single_quotes;
	t_bool	has_pipe;
	t_bool	has_redirection;
	t_bool	has_heredoc;
	t_bool	has_append;
}	t_grammar;

void	tokenizer_bonus(char *input, t_grammar *grammar);
void	init_grammar(t_grammar *grammar);
void	init_builtins(t_grammar *grammar);
void	copy_builtins_values(t_grammar *grammar);
t_bool	is_builtin(char *token, t_grammar *grammar);
t_bool	is_command(char *token, t_grammar *grammar);
t_bool	is_redirections(char *token);
t_bool	is_pipe_or_bonus_operators(char *token);
void	handle_not_command_error(char *token);
void	handle_input_redirection(char **split_input, t_grammar *grammar
		, size_t *i);
