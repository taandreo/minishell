.PHONY: all clean fclean re log bonus libft

CC = clang
NAME = minishell
NAME_BONUS = .minishell_bonus.txt
LIBFT = lib/libft.a
LIBFT_DIR = libft
MANDATORY_DIR := mandatory
BONUS_DIR := bonus
OBJS_DIR := objects
BONUS_OBJS_DIR := bonus_objects
INC_DIR := includes
CFLAGS = -Wall -Wextra -Werror -g3
UNAME := $(shell uname)
LIBS := -lft -lreadline

ifeq ($(UNAME), Darwin)
	CFLAGS += -arch x86_64
endif

SRCS = $(addprefix $(MANDATORY_DIR)/, minishell.c\
			tokenizer/tokenizer.c\
		)

BONUS = $(addprefix $(BONUS_DIR)/, minishell_bonus.c\
			free_utils_bonus.c\
			free_utils2_bonus.c\
			tokenizer/tokenize_by_category_bonus.c\
			tokenizer/tokenize_functions.c\
			tokenizer/tokenize_wildcard.c\
			tokenizer/init_var_string_bonus.c\
			tokenizer/tokenize_utils_bonus.c\
			tokenizer/tokenizer_bonus.c\
			tokenizer/token_list_creation_bonus.c\
			tokenizer/tokenizer_helper_functions_bonus.c\
			tokenizer/command_not_found_bonus.c\
			tokenizer/handle_quotes_bonus.c\
			tokenizer/strings_tokenizer_helper_bonus.c\
			tokenizer/add_string_token_helper.c\
			tokenizer/variable_expansion.c\
			tokenizer/handle_variable_expansion_bonus.c\
			tokenizer/add_token_helper_bonus.c\
			parser/parser_bonus.c\
			parser/parse_command_bonus.c\
			parser/parser_helper_functions_bonus.c\
			parser/command_part_helper_bonus.c\
			error/return_errors_helper_bonus.c\
			parser/free/free_parse_tree1_bonus.c\
			parser/free/free_parse_tree2_bonus.c\
			parser/init/init_structures_bonus.c\
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
	@$(CC) $(CFLAGS) $(OBJS) -o $@ -L$(LIBFT_DIR) $(LIBS)
	@echo ________________________
	@echo Minishell binary created
	@echo ________________________

$(NAME_BONUS): $(BONUS_OBJS) | libft
	@if [ -d "$(OBJS_DIR)" ]; then \
      		rm -rf $(OBJS_DIR); \
      		rm -f $(NAME); \
    fi
	@$(CC) $(CFLAGS) $(BONUS_OBJS) -o minishell -L$(LIBFT_DIR) $(LIBS)
	@touch $@
	@echo ______________________________
	@echo Minishell bonus binary created
	@echo ______________________________

$(OBJS_DIR)/%.o: $(MANDATORY_DIR)/%.c
	mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(LIBFT_DIR)/include -I$(INC_DIR) -c $< -o $@

$(BONUS_OBJS_DIR)/%.o: $(BONUS_DIR)/%.c
	mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(LIBFT_DIR)/include -I$(INC_DIR) -c $< -o $@

all: $(NAME)

bonus: $(NAME_BONUS)

libft:
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJS_DIR)
	rm -rf $(BONUS_OBJS_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS) $(LIBFT)
	make fclean -C $(LIBFT_DIR)

re: fclean all

git_libft:
	git clone https://github.com/taandreo/42-libft.git libft
log:
	@echo $(BONUS_OBJS)