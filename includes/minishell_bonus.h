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
# define MISUSE 2
# define GENERAL_ERROR 1
# define EXIT_OFFSET 128

typedef enum e_connector
{
	empty,
	pipeline,
	and,
	or
}	t_connector;

typedef struct e_grammar
{
	char	*command;
	char	**builtins;
	char 	*redirection;
	char 	*command_part;
	char	*input_file_name;
	char	*output_file_name;
	char 	*tag;
	char	*path;
	t_bool	has_command;
	t_bool	has_double_quotes;
	t_bool	has_single_quotes;
	t_bool	has_pipe;
	t_bool	has_input_redirection;
	t_bool	has_output_redirection;
	t_bool	has_heredoc;
	t_bool	has_append;
}		t_grammar;

typedef struct e_node
{
	int		output_file;
	int		input_file;
	char	*input_file_name;
	char	*output_file_name;
	char	*command_part;
	char	*command;
	char	*tag;
	t_bool	has_input_redirection;
	t_bool	has_output_redirection;
	t_bool	has_append;
	t_bool	has_heredoc;
}	t_node;

typedef struct e_pair
{
	t_connector	connector;
	t_node	*left;
	t_node	*right;
}	t_pair;


typedef struct e_parse_tree
{
	t_pair				*pair;
	struct e_parse_tree *next;
}	t_parse_tree;

void	tokenizer_bonus(char *input, t_grammar *grammar
		, t_parse_tree *parse_tree);
void	init_grammar(t_grammar *grammar);
void	init_builtins(t_grammar *grammar);
void	copy_builtins_values(t_grammar *grammar);
t_bool	is_builtin(char *token, t_grammar *grammar);
t_bool	is_command(char *token, t_grammar *grammar);
t_bool	is_redirections(char *token);
t_bool	is_pipe_or_bonus_operators(char *token);
void	handle_not_command_error(char *token);
void	handle_redirections(char **split_input, t_parse_tree *parse_tree,
							size_t *i);
void	handle_input_redirection(char **split_input, t_parse_tree *parse_tree,
								 size_t *i);
void	handle_output_redirection(char **split_input, t_parse_tree *parse_tree,
								  size_t *i);
t_node	*allocate_node(void);
t_pair	*allocate_pair(void);
void	init_parse_tree(t_parse_tree *parse_tree);
