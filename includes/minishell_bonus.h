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