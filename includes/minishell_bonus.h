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

typedef enum e_token_type
{
	TOKEN_COMMAND_NAME,
	TOKEN_ECHO,
	TOKEN_CD,
	TOKEN_PWD,
	TOKEN_EXPORT,
	TOKEN_UNSET,
	TOKEN_ENV,
	TOKEN_EXIT,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PIPE,
	TOKEN_REDIRECTION_INPUT,
	TOKEN_REDIRECTION_OUTPUT,
	TOKEN_REDIRECTION_APPEND,
	TOKEN_REDIRECTION_HEREDOC,
	TOKEN_STRING,
	TOKEN_SPECIAL_ARG,
	TOKEN_END
} t_token_type;

typedef struct e_token
{
	t_token_type	type;
	char			*value;
} t_token;

typedef struct e_token_node {
	t_token				token;
	struct e_token_node	*next;
} t_token_node;

typedef struct e_token_list
{
	t_token_node	*head;
	t_token_node	*tail;
	size_t			count;
}	t_token_list;


/* TODO: Vou mudar essas duas estruturas para um parser mais consistente, ja
 * TODO: que não podemos parsear apenas divindo por espaços.*/
//typedef struct e_grammar
//{
//	char	*command;
//	char	**builtins;
//	char 	*command_part;
//	char	*input_file_name;
//	char	*output_file_name;
//	char 	*tag;
//	char	*path;
//	t_bool	has_command;
//	t_bool	has_double_quotes;
//	t_bool	has_single_quotes;
//	t_bool	has_pipe;
//	t_bool	has_input_redirection;
//	t_bool	has_output_redirection;
//	t_bool	has_heredoc;
//	t_bool	has_append;
//}		t_grammar;
//
//typedef struct e_command
//{
//	int		output_file;
//	int		input_file;
//	char	*input_file_name;
//	char	*output_file_name;
//	char	*command_part;
//	char	*command;
//	char	*tag;
//	t_bool	has_input_redirection;
//	t_bool	has_output_redirection;
//	t_bool	has_append;
//	t_bool	has_heredoc;
//}	t_command;

t_token_list	tokenizer(char *input, size_t input_len);
void			init_grammar(t_grammar *grammar);
void			init_builtins(t_grammar *grammar);
void			copy_builtins_values(t_grammar *grammar);
t_bool			is_builtin(char *token, t_grammar *grammar);
t_bool			is_command(char *token, t_grammar *grammar);
t_bool			is_redirections(char *token);
t_bool			is_pipe_or_bonus_operators(char *token);
void			handle_not_command_error(char *token);
void			handle_redirections(char **split_input,	t_grammar *grammar,
									t_parse_tree *parse_tree,
									size_t *i);
void			handle_input_redirection(char **split_input, t_grammar *grammar,
								 		t_parse_tree *parse_tree,
								 		size_t *i);
void			handle_output_redirection(char **split_input,
										  t_grammar *grammar,
										  t_parse_tree *parse_tree, size_t *i);
t_command		*allocate_node(void);
t_pair			*allocate_pair(void);
void			init_parse_tree(t_parse_tree *parse_tree);
void			syntax_error(char *token);
t_token_list	create_token_list(void);
void			add_token(t_token_list *token_list, t_token_type type,
						  const char *value);
void			free_token_list(t_token_list *token_list);
t_bool			has_quotes(char c);
