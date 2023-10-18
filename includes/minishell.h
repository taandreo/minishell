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

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "ft_printf.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>

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
	TOKEN_INVALID,
	TOKEN_LEFT_PARENTHESIS,
	TOKEN_RIGHT_PARENTHESIS,
	TOKEN_REDIRECTIONS,
	TOKEN_REDIRECTION_INPUT,
	TOKEN_REDIRECTION_OUTPUT,
	TOKEN_REDIRECTION_APPEND,
	TOKEN_REDIRECTION_HEREDOC,
	TOKEN_ERROR,
	TOKEN_COMMAND_NAME_QUOTES,
	TOKEN_COMMAND_NAME_UNQUOTES,
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
	int		kill_child;
	t_bool	error;
	t_bool	is_set;
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
	t_command				*enclosed_cmd;
}	t_grouping;

typedef struct s_command_part
{
	t_token_type			type;
	int						in_pipe[2];
	int						out_pipe[2];
	t_bool					forked;
	char					**args;
	pid_t					pid;
	union
	{
		t_builtin_cmd		*builtin_cmd;
		t_string			*cmd_name;
		t_grouping			*grouping;
	} u_cmd;
	t_arguments				*arguments;
	t_redirections			*redirections;
	char					*cmd_path;
}	t_command_part;

typedef struct s_pipeline
{
	t_token_type			type;
	t_command_part			*cmd_part;
	struct s_pipeline		*next;
}	t_pipeline;

typedef struct s_conjunctions
{
	t_token_type			type;
	t_pipeline				*pipeline;
	struct s_conjunctions	*next;
}	t_conjunctions;

typedef struct s_command
{
	t_pipeline				*pipeline;
	t_conjunctions			*conjunctions;
}	t_command;

typedef struct s_env
{
	char					*key;
	char					*value;
}			t_env;

typedef struct s_vars
{
	t_token_list			**tokens;
	t_command				*parse_tree;
	t_parser_state			state;
	char					**prompt;
	char					*nice_prompt;
	void					*args;
	t_bool					changed_stdout;
	t_bool					changed_stdin;
	t_bool					close_heredoc;
	t_bool					is_forked;
	int						saved_stdout;
	int						saved_stdin;
	t_list					*env;
}	t_vars;

extern t_vars				g_vars;

t_token_list	*tokenizer(char *input, t_token_flags *flags);
char			*handle_quotes(char **input, size_t *position,
					t_token_list **tokens, t_token_flags *flags);
int				misuse_or_unclosed_quotes_error(t_token_list **tokens);
int				unclosed_quotes_error(t_token_list **tokens);
t_bool			is_builtin(char *token);
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
t_bool			empty_token_list(t_token_list *tokens);
void			write_proper_msg(void);
void			init_fixed_vars(t_vars *vars);
t_arguments		*parse_arguments(t_token_list *tokens, t_parser_state *state);
t_string		*parse_string(t_token_list *tokens, t_parser_state *state);
t_bool			is_operator_or_end(t_token_type type);
t_token			current_token(const t_token_list *tokens);
t_token_type	current_token_type(t_token_list *tokens);
void			advance_token(t_token_list *tokens);
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
t_bool			is_redirection_or_string(t_token_type token);
t_bool			next_redirection_and_arguments(t_command_part *command_part,
					t_redirections *initial_redirections,
					t_token_list *tokens, t_parser_state *state);
void			next_redirections(t_command_part *command_part,
					t_redirections *initial_redirections, t_token_list *tokens,
					t_parser_state *state);
void			init_parse_tree_and_execute_cmd(t_vars *vars);
t_builtin_cmd	*handle_builtin_tokens(t_command_part *command_part,
					t_token_list *tokens, t_parser_state *state);
t_string		*handle_command_name_tokens(t_command_part *command_part,
					t_token_list *tokens, t_parser_state *state);
void			*return_mem_alloc_error(void);
void			*return_syntax_error(const char *value);
void			*free_and_return_null(void *ptr);
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
void			*free_str_nullify_and_malloc_error(char **str);
void			free_2_str_and_nullify(char **str1, char **str2);
t_bool			is_token_command_name(t_token_type type);
int				execute_command(t_command **cmd, t_vars *vars);
t_string		*expand_exit_code(t_string	*string, t_vars *vars,
					t_token_type type);
t_string		*expand_wildcard(t_string *string, t_vars *vars,
					t_token_type type);
t_string		*concat_exit_code(t_string *string, t_vars *vars,
					t_token_type type);
t_token_type	peek_string_type(t_string	*string);
t_string		*clean_next_token(t_string *current);
t_string		*concat_string(t_string *string, t_command_part *cmd_part,
					t_vars *vars, t_token_type type);
void			add_concat_string(t_command_part *cmd_part,
					t_string *concat_str, t_vars *vars, t_token_type type);
int				update_cmd_part_values(t_command_part *cmd_part, t_vars *vars);
char			*builtin_type_to_value(t_token_type type);
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
int				execute_builtin_command(t_command_part *data, char **args,
					t_vars *vars);
void			execute_builtin(t_command_part *data, t_vars *vars);
t_bool			is_token_cmd_name(t_token_type token);
void			execute_redirections(t_command_part *data, t_vars *vars);
void			input_file_to_stdin(int infile, t_vars *vars);
void			output_file_to_stdout(int outfile, t_vars *vars);
int				open_tmp_file(void);
void			heredoc_to_stdin(t_vars *vars);
char			*cmd_path_routine(char *cmd, t_token_type token, t_vars *vars);
void			restore_stdout(int saved_stdout, t_vars *vars);
void			restore_stdin(int saved_stdin, t_vars *vars);
t_bool			add_both_cmd_name(t_token_flags *flags, t_token_list **tokens,
					char **string);
t_bool			add_cmd_name_unquotes(char **string, t_token_list **tokens);
t_bool			add_cmd_name_quotes(char **string, t_token_list **tokens);
t_bool			check_if_is_cmd_true(char *string, t_token_list **tokens);
// BUILTIN
int				builtin_echo(char **params);
int				builtin_pwd(char **params);
int				builtin_cd(char **params);
int				builtin_export(char **params);
int				builtin_env(char **params);
int				builtin_unset(char **params);
int				builtin_exit(char **params, t_vars *vars);
t_bool			is_valid_env(char *env);
t_list			*search_env(char *key);
// ENV
void			add_env(char *key, char *value);
void			init_env(char **envp);
void			remove_env(char *key);
char			*get_env(char *key);
void			free_env(void *env);
void			free_all_envs(void);
// args_and_envp_bonus.c
char			**list_to_args(t_arguments *list, t_command_part *cmd_part);
char			**list_to_envp(void);
// execute_command_part.c
int				execute_command_part(t_command_part *data, t_vars *vars);
void			execute_redirection_heredoc(t_redirections *redir,
					t_vars *vars);
// execute_fork_bonus.c
void			execute_fork(t_pipeline *pipeline, t_vars *vars);
void			open_heredoc(t_redirections *redirections, t_vars *vars);
void			child_sigusr_handler(int signum);
void			parent_sigusr_handler(int signum);
void			trigger_child_sigusr(t_bool ignore);
void			start_signal_forked(pid_t pid);
void			start_signals_parent(void);
void			start_signal_heredoc(void);
void			trigger_parent_sigusr(void);
int				init_stdout_var(t_vars *vars);
int				init_stdin_var(t_vars *vars);
char			*get_cmd_path(char *cmd);
// ERROR
void			mem_alloc_set_error(t_vars *vars);
void			error_open_file(char *file, t_vars *vars);
void			*free_nullify_and_return_mem_error(char **ptr);
void			free_and_perror(t_vars *vars, int exit_code);
void			free_minishell(t_vars *vars);
void			free_mini_line(t_vars *vars);
void			print_perror(char *cmd, char *msg);
int				success_or_mem_error(int exit_status);
void			print_mem_alloc_error(void);
void			*misuse_state_error(t_parser_state *state);
void			*print_misuse_state_error(t_parser_state *state);
void			subsequent_arguments(t_command_part *command_part,
					t_token_list *tokens, t_parser_state *state);
void			*error_and_exit_code_false(t_token_flags *flags);
void			*null_exit_code_false_free_string(t_token_flags *flags);
void			*null_exit_code_false(t_token_flags *flags);
void			set_flags_variables(char *input, size_t *pos,
					t_token_flags *flags);
t_bool			is_builtin_token(t_token_type type);
t_bool			add_command_union(t_command_part *command_part,
					t_redirections *redirections, t_token_list *tokens,
					t_parser_state *state);
t_bool			is_operator_or_invalid_token(t_token_type type);
void			*null_and_free_grouping(t_grouping *grouping);
void			*general_error_ambiguous_redirect(t_vars *vars,
					t_string *string);
void			*null_free_args_misuse(t_arguments *args,
					t_arguments *curr_args, t_string *str_list, t_vars *vars);
void			free_minishell(t_vars *vars);
void			*return_no_such_cmd(char *str, t_vars *vars);
void			*return_is_dir_error_cmd(char *str, t_vars *vars);
void			*return_permission_denied(char *str, t_vars *vars);
void			*return_cmd_not_found(char *str, t_vars *vars);
// WAIT
int				wait_process(t_pipeline *pipeline);

#endif
