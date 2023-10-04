/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:32:36 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/25 20:36:42 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

# include "libft.h"
# include "ft_printf.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>

# define NBR_BUILTINS 7
# define NOTEXEC 126
# define CMDNFND 127
# define SUCCESS 0
# define GENERAL_ERROR 1
# define MISUSE 2
# define EXIT_OFFSET 128
# define PATH_MAX	4096

extern t_list *g_env;

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
	TOKEN_INVALID,
	TOKEN_LEFT_PARENTHESIS,
	TOKEN_RIGHT_PARENTHESIS,
	TOKEN_REDIRECTIONS,
	TOKEN_REDIRECTION_INPUT,
	TOKEN_REDIRECTION_OUTPUT,
	TOKEN_REDIRECTION_APPEND,
	TOKEN_REDIRECTION_HEREDOC,
	TOKEN_ERROR,
	TOKEN_COMMAND_NAME,
	TOKEN_FILENAME,
	TOKEN_WILDCARD,
	TOKEN_STRING,
	TOKEN_EXIT_CODE,
	TOKEN_SPACE,
	TOKEN_GROUP,
	TOKEN_NONE,
	TOKEN_END
}	t_token_type;

typedef struct s_token_flags
{
	t_bool	is_command;
	t_bool	is_redirection;
	t_bool	has_heredoc;
	t_bool	has_command;
	t_bool	inside_quotes;
	t_bool	init_var;
	t_bool	has_exit_code;
	size_t	input_len;
	size_t	paren_count;
	int		var_len;
	int		status;
	char	quote_type;
	char	*var;
	char	*string;
	char	*input;
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

typedef struct s_parser_state
{
	int		paren_count;
	int		status;
	t_bool	error;
	t_bool	has_paren;
}	t_parser_state;

typedef struct s_string
{
	t_token_type	type;
	char			*value;
	struct s_string	*next;
}	t_string;

typedef struct s_arguments
{
	t_token_type		type;
	t_string			*string;
	struct s_arguments	*next;
}	t_arguments;

typedef struct s_redirection
{
	t_token_type	type;
	t_string		*filename;
}	t_redirection;

typedef struct s_redirections
{
	t_redirection			*redirection;
	struct s_redirections	*next;
}	t_redirections;

typedef struct s_builtin_cmd
{
	t_token_type	type;
	char			*value;
}	t_builtin_cmd;

typedef struct s_command	t_command;

typedef struct s_grouping
{
	t_command		*enclosed_cmd;
}	t_grouping;

typedef struct s_command_part
{
	t_token_type			type;
	union
	{
		t_builtin_cmd	*builtin_cmd;
		t_string		*cmd_name;
		t_grouping		*grouping;
	} u_cmd;
	t_arguments				*arguments;
	t_redirections			*redirections;
}	t_command_part;

typedef struct s_pipeline
{
	t_token_type		type;
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

typedef struct s_vars
{
	t_token_list	*tokens;
	t_command		*parse_tree;
	t_parser_state	state;
}	t_vars;

typedef struct s_env
{
	char	*key;
	char	*value;
}			t_env;

t_token_list	*tokenizer(char *input, t_token_flags *flags);
char			*handle_quotes(char **input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				misuse_or_unclosed_quotes_error(t_token_list **tokens);
int				unclosed_quotes_error(t_token_list **tokens);
t_bool			is_builtin(char *token);
t_bool			is_command(char *token);
t_bool			is_redirections(char *token);
t_bool			is_pipe_or_bonus_operators(char *token);
void			handle_not_command_error(char *token);
t_token_list	*create_token_list(void);
int				add_token(t_token_list **tokens, t_token_type type,
					const char *value);
void			free_token_list(t_token_list **token_list_ptr);
t_bool			has_quotes(char c);
char			peek_next(const char *input, size_t position, size_t input_len);
t_bool			is_string_start(char c, t_token_flags *flags);
t_token_flags	init_token_flags(size_t input_len);
int				add_token_1_pos(size_t *pos, t_token_list **tokens,
					t_token_type type, t_token_flags *flags);
int				add_token_2_pos(size_t *pos, t_token_list **tokens,
					t_token_type type, t_token_flags *flags);
t_token_type	get_builtin_token(char *token);
char			*get_string_from_input(char **input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
int				add_builtin_or_command(char *return_string,
					t_token_list **tokens, t_token_flags *flags, char next);
int				add_filename_or_string(char *return_string,
					t_token_list **tokens, t_token_flags *flags, char next);
t_bool			add_command_or_string(t_token_list **tokens,
					t_token_flags *flags, char *input, size_t *pos);
t_command		*parse(t_token_list *tokens, t_parser_state *state);
t_command		*parse_command(t_token_list *tokens, t_parser_state *state);
t_grouping		*parse_grouping(t_command_part *command_part,
					t_token_list *tokens, t_parser_state *state);
t_redirections	*parse_redirections(t_token_list *tokens,
					t_parser_state *state);
t_arguments		*parse_arguments(t_token_list *tokens, t_parser_state *state);
t_string		*parse_string(t_token_list *tokens, t_parser_state *state);
t_bool			is_operator_or_end(t_token_type type);
t_token			current_token(const t_token_list *tokens);
t_token_type	current_token_type(t_token_list *tokens);
void			advance_token(t_token_list *tokens);
t_token			peek_token(t_token_list *tokens);
void			free_command(t_command *cmd);
void			free_grouping(t_grouping *grouping);
void			free_conjunctions(t_conjunctions *conj);
void			free_pipeline(t_pipeline *pipe);
void			free_command_part(t_command_part *cmd_part);
void			free_builtin_command(t_builtin_cmd *cmd);
void			free_redirections(t_redirections *redirs);
void			free_redirection(t_redirection *redir);
void			free_arguments(t_arguments *args);
void			free_string(t_string *str);
void			init_command_part_fields(t_command_part *command_part);
void			subsequent_redirections(t_command_part *command_part,
					t_redirections *initial_redirections, t_token_list *tokens,
					t_parser_state *state);
t_builtin_cmd	*handle_builtin_tokens(t_command_part *command_part,
					t_token_list *tokens, t_parser_state *state);
t_string		*handle_command_name_tokens(t_command_part *command_part,
					t_token_list *tokens, t_parser_state *state);
void			*return_mem_alloc_error(void);
void			*return_syntax_error(const char *value);
void			*free_and_return_null(void *ptr);
void			*free_2_and_return_null(void *ptr1, void *ptr2);
char			*expand_variable_string(char *input, size_t *pos,
					t_token_flags *flags);
char			*join_and_cleanup(char **malloced_str1, char **malloced_str2);
char			*join_1st_and_cleanup(char **malloced_str1, char *str2);
char			*substitute_variable(char *input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
int				strings_handle_variable_expansion(char **input, size_t *pos,
					t_token_flags *flags);
char			*quotes_handle_variable_expansion(char **input, size_t *pos,
					t_token_flags *flags);
void			free_str_and_nullify(char **ptr);
void			*initialize_var_string(char *input, size_t pos,
					t_token_flags *flags, const char *start);
void			*add_unclosed_quotes_token(t_token_list **tokens,
					t_token_flags *flags, char *quoted_string);
int				check_ambiguous_redirect(char *string, t_token_list **tokens,
					t_token_flags *flags);
t_bool			has_variable_expansion(char **input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
t_bool			handle_special_case_variable(char *input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_by_category(char **input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_quotes(char **input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_redirections(char *input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_operators(char *input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_strings(char **input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_parenthesis(const char *input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
int				tokenize_wildcard(char *input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);
void			*free_nullify_and_return_null(char **ptr);
int				unclosed_paren_error(t_token_list **tokens, char **prompt);
t_bool			decrease_len(t_token_flags *flags);
int				free_vars_and_return_misuse(char *string, char *tmp);
void			*free_str_nullify_and_malloc_error(char **str);
void			free_2_str_and_nullify(char **str1, char **str2);
t_bool			is_token_command_name(t_token_type type);
int				execute_command(t_command **cmd, t_vars *vars);
t_string		*expand_exit_code(t_string	*string, t_vars *vars, t_token_type type);
t_string		*expand_wildcard(t_string *string, t_vars *vars, t_token_type type);
t_string		*concat_exit_code(t_string *string, t_vars *vars, t_token_type type);
t_token_type	peek_string_type(t_string	*string);
t_string		*clean_next_token(t_string *current);
t_string		*concat_string(t_string *string, t_command_part  *cmd_part, t_vars *vars, t_token_type type);
void			add_concat_string(t_command_part *cmd_part, t_string *concat_str,
					t_vars *vars, t_token_type type);
int				update_cmd_part_values(t_command_part *cmd_part, t_vars *vars);
char			*builtin_type_to_value(t_token_type  type);
t_string		*delete_first_node(t_string *str);
t_arguments		*add_first_args(t_string *str_list, t_arguments *args,
					t_vars *vars, t_token_type type);
t_arguments		*add_last_arg_list(t_string *str_list, t_arguments *args,
					t_vars *vars, t_token_type type);
t_arguments		*add_middle_args(t_string *str_list, t_arguments *args,
					t_vars *vars, t_token_type type);
void			walk_last_args(t_arguments *last_args);
void			*free_resources_and_return_null(DIR *dir,
					t_string *result_head);
t_string		*add_new_node(char *filename, t_vars *vars,	t_token_type type);
void			sorted_insert(t_string **head_ref, t_string *new_node);
DIR				*open_dir_or_error(void);
int				execute_builtin(char *builtin, t_command_part *cmd_part,
					t_vars *vars);
int				execute_cmd_name(char *cmd_name, t_command_part *cmd_part,
					t_vars *vars);
int				execute_redirections_only(t_redirections *redirs, t_vars *vars);
// BULTIN
int				bultin_echo(char **params);
int				bultin_pwd(char **params);
int				bultin_cd(char **params);
int				bultin_export(char **params);
int				bultin_env(char **params);
int				bultin_unset(char **params);
int				bultin_exit(char **params);
t_bool			is_valid_env(char *env);
t_list			*search_env(char *key);
// ENV
void            add_env(char *key, char *value);
void	        init_env(char **envp);
void	        print_env();
void			remove_env(char *key);
char			*get_env(char *key);
void			free_env(void *env);
void			free_all_envs(void);

// ERROR
void			print_perror(char *cmd, char *msg);
int				success_or_mem_error(int exit_status);
void			print_mem_alloc_error(void);
void			*misuse_state_error(t_parser_state *state);
void			*print_misuse_state_error(t_parser_state *state);
void			subsequent_arguments(t_command_part *command_part, t_token_list *tokens,
					t_parser_state *state);
void			*error_and_exit_code_false(t_token_flags *flags);
void			*null_exit_code_false_free_string(t_token_flags *flags);
void			*null_exit_code_false(t_token_flags *flags);
void			set_flags_variables(char *input, size_t *pos,
					t_token_flags *flags);
t_bool			is_builtin_token(t_token_type type);
t_bool			add_command_union(t_command_part  *command_part,
				t_redirections	*redirections, t_token_list  *tokens,
					t_parser_state *state);
t_bool			is_operator_or_invalid_token(t_token_type type);
void			*null_and_free_grouping(t_grouping *grouping);
void			*general_error_ambiguous_redirect(t_vars *vars,
					t_string *string);
void			*null_free_args_misuse(t_arguments *args,
					t_arguments *curr_args, t_string *str_list, t_vars *vars);
#endif
