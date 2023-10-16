.PHONY: all clean fclean re log bonus libft builtin_test

CC = clang
NAME = minishell
NAME_BONUS = .minishell_bonus.txt
LIBFT = libft/libft.a
LIBFT_DIR = libft
MANDATORY_DIR := mandatory
BONUS_DIR := bonus
OBJS_DIR := objects
BONUS_OBJS_DIR := bonus_objects
INC_DIR := includes
CFLAGS = -Wall -Wextra -Werror -g3
UNAME := $(shell uname)
LIBS := -lft -lreadline
LIB_DIRS := $(LIBFT_DIR)

ifeq ($(UNAME), Darwin)
 	# CFLAGS += -arch x86_64
	INC_DIR += -I /opt/homebrew/opt/readline/include
	LIB_DIRS += -L /opt/homebrew/opt/readline/lib
endif

SRCS = $(addprefix $(MANDATORY_DIR)/, minishell.c\
			minishell_run.c\
			init_main_vars.c\
			utils_1.c\
			utils_2.c\
			tokenizer/tokenize_by_category.c\
            tokenizer/tokenize_functions.c\
            tokenizer/tokenize_wildcard.c\
            tokenizer/init_var_string.c\
            tokenizer/tokenize_utils.c\
            tokenizer/tokenizer.c\
            tokenizer/token_list_creation.c\
            tokenizer/tokenizer_helper_functions.c\
            tokenizer/handle_quotes.c\
            tokenizer/strings_tokenizer_helper.c\
            tokenizer/add_string_token_helper.c\
            tokenizer/variable_expansion.c\
            tokenizer/handle_variable_expansion.c\
            tokenizer/add_cmd_name_token.c\
            tokenizer/add_token_helper.c\
            parse_tree/parser.c\
            parse_tree/parse_command.c\
            parse_tree/parse_redirections.c\
            parse_tree/parse_arguments.c\
            parse_tree/parser_helper_functions.c\
            parse_tree/command_part_helper.c\
            parse_tree/free/free_parse_tree1.c\
            parse_tree/free/free_parse_tree2.c\
            parse_tree/init/init_structures.c\
            parse_tree/parser_state_error.c\
			parse_tree/parser_utils.c\
            signals/sigusr.c\
            signals/sigint.c\
		)

BONUS = $(addprefix $(BONUS_DIR)/, minishell_bonus.c\
			minishell_run_bonus.c\
			init_main_vars_bonus.c\
			utils_bonus.c\
			utils_2_bonus.c\
			tokenizer/tokenize_by_category_bonus.c\
			tokenizer/tokenize_functions_bonus.c\
			tokenizer/tokenize_wildcard_bonus.c\
			tokenizer/init_var_string_bonus.c\
			tokenizer/tokenize_utils_bonus.c\
			tokenizer/tokenizer_bonus.c\
			tokenizer/token_list_creation_bonus.c\
			tokenizer/tokenizer_helper_functions_bonus.c\
			tokenizer/handle_quotes_bonus.c\
			tokenizer/strings_tokenizer_helper_bonus.c\
			tokenizer/add_string_token_helper_bonus.c\
			tokenizer/variable_expansion_bonus.c\
			tokenizer/handle_variable_expansion_bonus.c\
			tokenizer/add_cmd_name_token_bonus.c\
			tokenizer/add_token_helper_bonus.c\
			parse_tree/parser_bonus.c\
			parse_tree/parse_command_bonus.c\
			parse_tree/parse_redirections_bonus.c\
			parse_tree/parse_arguments_bonus.c\
			parse_tree/parser_helper_functions_bonus.c\
			parse_tree/command_part_helper_bonus.c\
			parse_tree/free/free_parse_tree1_bonus.c\
			parse_tree/free/free_parse_tree2_bonus.c\
			parse_tree/init/init_structures_bonus.c\
			parse_tree/parser_state_error_bonus.c\
            parse_tree/parser_utils_bonus.c\
			builtin/echo_bonus.c\
			builtin/pwd_bonus.c\
			builtin/export_bonus.c\
			builtin/env_bonus.c\
			builtin/unset_bonus.c\
			builtin/cd_bonus.c\
			builtin/exit_bonus.c\
			env/env_vars_0_bonus.c\
			env/env_vars_1_bonus.c\
			error/errors_bonus.c\
			error/free_minishell_bonus.c\
			error/free_utils_bonus.c\
			error/free_utils2_bonus.c\
			error/return_errors_helper_bonus.c\
            error/misuse_or_unclosed_error_bonus.c\
            error/file_access_errors_bonus.c\
			parser/args_and_envp_bonus.c\
			parser/execute_fork_bonus.c\
			parser/execute_pipeline_bonus.c\
            parser/execute_command_bonus.c\
            parser/execute_builtin_bonus.c\
            parser/execute_cmd_name_bonus.c\
            parser/get_cmd_path_bonus.c\
			parser/get_cmd_from_path_bonus.c\
            parser/expand_variables/update_command_part_bonus.c\
            parser/expand_variables/expand_and_concat_exit_code_bonus.c\
            parser/expand_variables/add_concat_string_bonus.c\
            parser/expand_variables/concat_string_utils_bonus.c\
            parser/expand_variables/expand_wildcard_bonus.c\
            parser/expand_variables/concat_string_bonus.c\
            parser/expand_variables/add_args_list_bonus.c\
            parser/expand_variables/add_args_utils_bonus.c\
            parser/expand_variables/expand_wildcard_utils_bonus.c\
         	parser/execute_redirections_bonus.c\
         	parser/execute_redirections_utils_bonus.c\
			parser/heredoc_bonus.c\
         	parser/restore_fd_bonus.c\
			parser/wait_bonus.c\
			signals/sigusr_bonus.c\
			signals/sigint_bonus.c\
		)

OBJS = $(patsubst $(MANDATORY_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))
BONUS_OBJS = $(patsubst $(BONUS_DIR)%.c, $(BONUS_OBJS_DIR)%.o, $(BONUS))
# All Src subdirectories
SRC_SUBDIR := $(dir $(OBJS))
BONUS_SUBDIR := $(dir $(BONUS_OBJS))
# Objects Subdirectories
OBJS_SUBDIR := $(subst $(MANDATORY_DIR), $(OBJS_DIR), $(SRC_SUBDIR))
BONUS_OBJS_SUBDIR := $(subst $(BONUS_DIR), $(BONUS_OBJS_DIR), $(BONUS_SUBDIR))

$(NAME): $(OBJS) | libft
	@if [ -d "$(BONUS_OBJS_DIR)" ]; then \
      		rm -rf $(BONUS_OBJS_DIR); \
      		rm -f $(NAME); \
    fi
	@$(CC) $(CFLAGS) $(OBJS) -o $@ -L$(LIB_DIRS) $(LIBS)
	@echo ________________________
	@echo Minishell binary created
	@echo ________________________

$(NAME_BONUS): $(BONUS_OBJS) | libft
	@if [ -d "$(OBJS_DIR)" ]; then \
      		rm -rf $(OBJS_DIR); \
      		rm -f $(NAME); \
    fi
	@$(CC) $(CFLAGS) $(BONUS_OBJS) -o minishell -L$(LIB_DIRS) $(LIBS)
	@touch $@
	@echo ______________________________
	@echo Minishell bonus binary created
	@echo ______________________________

$(OBJS_DIR)/%.o: $(MANDATORY_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(LIBFT_DIR)/include -I$(INC_DIR) -c $< -o $@

$(BONUS_OBJS_DIR)/%.o: $(BONUS_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(LIBFT_DIR)/include -I$(INC_DIR) -c $< -o $@

all: $(NAME)

bonus: $(NAME_BONUS)

libft:
	@make -C $(LIBFT_DIR)

clean:
	@rm -f minishell.o
	rm -rf $(OBJS_DIR)
	rm -rf $(BONUS_OBJS_DIR)
	rm -f  $(BUILTIN_TEST_OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS) $(LIBFT)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

git_libft:
	git clone https://github.com/taandreo/42-libft.git libft
log:
	@echo $(BUILTIN_TEST_OBJS)

# builtin TESTER
BUILTIN_TEST_BIN  = builtin_test
BUILTIN_TEST_DIR  = tests/builtin
BUILTIN_TEST_SRCS = $(wildcard $(BUILTIN_TEST_DIR)/*.c) $(subst $(BONUS_DIR)/minishell_bonus.c,,$(BONUS))
BUILTIN_TEST_OBJS = $(subst $(BONUS_DIR)/, $(BONUS_OBJS_DIR)/, $(patsubst %.c, %.o, $(BUILTIN_TEST_SRCS)))
BUILTIN_TEST_LIBS = -lcmocka

$(BUILTIN_TEST_DIR)/%.o: $(BUILTIN_TEST_DIR)/%.c
	$(CC) -I$(LIBFT_DIR)/include -I$(INC_DIR) -I$(BUILTIN_TEST_DIR) -c $< -o $@

$(BUILTIN_TEST_BIN): $(BUILTIN_TEST_OBJS) | libft
	@$(CC) $(BUILTIN_TEST_OBJS) -o builtin_test -L$(LIBFT_DIR) $(LIBS) $(BUILTIN_TEST_LIBS)
	./builtin_test