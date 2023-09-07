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

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

# include "libft.h"
# include "ft_printf.h"
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <sys/stat.h>

# define NBR_BUILTINS 7
# define NOTEXEC 126
# define CMDNFND 127
# define SUCCESS 0
# define GENERAL_ERROR 1
# define MISUSE 2
# define EXIT_OFFSET 128

typedef enum e_token_type
{
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
	TOKEN_REDIRECTIONS,
	TOKEN_REDIRECTION_INPUT,
	TOKEN_REDIRECTION_OUTPUT,
	TOKEN_REDIRECTION_APPEND,
	TOKEN_REDIRECTION_HEREDOC,
	TOKEN_COMMAND_NAME,
	TOKEN_FILENAME,
	TOKEN_STRING,
	TOKEN_EXIT_CODE,
	TOKEN_SPACE,
	TOKEN_SPECIAL_ARG,
	TOKEN_INVALID,
	TOKEN_ERROR,
	TOKEN_END
}	t_token_type;

typedef struct s_token_flags
{
	t_bool	is_command;
	t_bool	is_redirection;
	t_bool	has_heredoc;
	t_bool	has_command;
	t_bool	inside_quotes;
	size_t	input_len;
	int		status;
	char	quote_type;
	char	*var;
	char	*string;
}	t_token_flags;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}	t_token;

typedef struct s_token_node
{
	t_token				token;
	struct s_token_node	*next;
}	t_token_node;

typedef struct s_token_list
{
	t_token_node	*head;
	t_token_node	*tail;
	t_token_node	*current;
	size_t			count;
}	t_token_list;

typedef struct s_argument
{
	t_token_type	type;
	char			*value;
}	t_argument;

typedef struct s_arguments
{
	t_argument			*argument;
	struct s_arguments	*next;
}	t_arguments;

typedef struct s_redirection
{
	t_token_type	type;
	char			*filename;
}	t_redirection;

typedef struct s_redirections
{
	t_redirection			*redirection;
	struct s_redirections	*next;
}	t_redirections;

typedef struct s_builtin_command
{
	t_token_type	type;
	t_arguments		*arguments;
}	t_builtin_command;

typedef struct s_command_part
{
	t_token_type			type;
	union
	{
		t_builtin_command	*builtin_cmd;
		char				*cmd_name;
	} u_cmd;
	t_arguments				*arguments;
	t_redirections			*redirections;
}	t_command_part;

typedef struct s_pipeline
{
	t_command_part		*cmd_part;
	struct s_pipeline	*next;
}	t_pipeline;

typedef struct s_conjunctions
{
	t_token_type			type;
	t_pipeline				*pipeline;
	struct s_conjunctions	*next;
}	t_conjunctions;

typedef struct s_command
{
	t_pipeline		*pipeline;
	t_conjunctions	*conjunctions;
}	t_command;

t_token_list	*tokenizer(char *input, t_token_flags *flags);
char			*handle_quotes(const char *input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				misuse_or_unclosed_quotes_error(t_token_list **tokens);
int				unclosed_quotes_error(t_token_list **tokens);
t_bool			is_builtin(char *token);
t_bool			is_command(char *token);
t_bool			is_redirections(char *token);
t_bool			is_pipe_or_bonus_operators(char *token);
void			handle_not_command_error(char *token);
void			syntax_error(char *token);
t_token_list	*create_token_list(void);
int				add_token(t_token_list **tokens, t_token_type type,
					const char *value);
void			free_token_list(t_token_list **token_list_ptr);
t_bool			has_quotes(char c);
char			peek_next(const char *input, size_t position, size_t input_len);
t_bool			is_string_start(char c);
t_token_flags	init_flags(size_t input_len);
int				add_token_1_pos(size_t *pos, t_token_list **tokens,
					t_token_type type, t_token_flags *flags);
int 			add_token_2_pos(size_t *pos, t_token_list **tokens,
					t_token_type type, t_token_flags *flags);
t_token_type	get_builtin_token(char *token);
char			*get_string_from_input(const char *input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
int				add_builtin_or_command(char *return_string,
					t_token_list **tokens, t_token_flags *flags);
int				add_filename_or_string(char *return_string,
					t_token_list **tokens, t_token_flags *flags);
t_command		*parse(t_token_list *tokens);
t_token			current_token(const t_token_list *tokens);
t_token_type	current_token_type(t_token_list *tokens);
void			advance_token(t_token_list *tokens);
t_command		*parse_command(t_token_list *tokens);
void			free_command(t_command *cmd);
void			free_conjunction(t_conjunctions *conj);
void			free_pipeline(t_pipeline *pipe);
void			free_command_part(t_command_part *cmd_part);
void			free_builtin_command(t_builtin_command *cmd);
void			free_redirections(t_redirections *redirs);
void			free_redirection(t_redirection *redir);
void			free_arguments(t_arguments *args);
void			free_argument(t_argument *arg);
void			init_command_part_fields(t_command_part *command_part);
void			add_subsequent_redirections_to_initial(t_command_part *command_part,
					t_redirections *initial_redirections);
t_command_part	*handle_builtin_tokens(t_command_part *command_part,
					t_token_list *tokens);
t_command_part	*handle_command_name_tokens(t_command_part *command_part,
					t_token_list *tokens);
void			*return_mem_alloc_error(void);
void			*return_syntax_error(const char *value);
void			*free_and_return_null(void *ptr);
void			*free_2_and_return_null(void *ptr1, void *ptr2);
char			*expand_variable_string(const char *input, size_t *pos);
char			*join_and_cleanup(char **malloced_str1, char **malloced_str2);
char			*substitute_variable(const char *input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
char			*strings_handle_variable_expansion(const char *input, size_t *pos,
					t_token_flags *flags);
char			*quotes_handle_variable_expansion(const char *input, size_t *pos,
					t_token_flags *flags);
void			free_str_and_nullify(char **ptr);
void			*initialize_var_string(const char *input, size_t pos,
					t_token_flags *flags, const char  *start);
void			*add_unclosed_quotes_token(t_token_list **tokens,
					t_token_flags *flags, char *quoted_string);
int				check_ambiguous_redirect(char *string, t_token_list **tokens,
					t_token_flags *flags);
t_bool			has_variable_expansion(const char *input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
t_bool			handle_special_case_variable(const char *input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_by_category(const char *input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_quotes(const char *input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_redirections(const char *input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_operators(const char *input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_strings(const char *input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
#endif
