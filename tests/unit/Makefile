MINISHELL_DIR := ../..
MINISHELL_MANDATORY_DIR := $(MINISHELL_DIR)/mandatory
MINISHELL_BONUS_DIR := $(MINISHELL_DIR)/bonus
MINISHELL_OBJS_DIR := $(MINISHELL_DIR)/objects
MINISHELL_BONUS_OBJS_DIR := $(MINISHELL_DIR)/bonus_objects
MINISHELL_LIBFT_DIR := $(MINISHELL_DIR)/libft
MINISHELL_INC_DIR := $(MINISHELL_DIR)/includes
TEST_OBJS_DIR := objects
TEST_DIR := srcs
NAME := tests
CC = clang
CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS := -Wl,--wrap=malloc
UNAME := $(shell uname)
LIBS := -lft -lreadline -lcmocka

ifeq ($(UNAME), Darwin)
	CFLAGS += -arch x86_64
endif

## Minishell Objects###################################################
MINISHELL_BONUS = $(addprefix $(MINISHELL_BONUS_DIR)/, tokenizer/token_list_creation_bonus.c\
						error/return_errors_helper_bonus.c\
		)
MINISHELL_OBJS = $(patsubst $(MINISHELL_MANDATORY_DIR)/%.c, $(MINISHELL_OBJS_DIR)/%.o, $(MINISHELL_SRCS))
MINISHELL_BONUS_OBJS = $(patsubst $(MINISHELL_BONUS_DIR)/%.c, $(MINISHELL_BONUS_OBJS_DIR)/%.o, $(MINISHELL_BONUS))
######################################################################

# Tester Objects
TEST_SRCS = $(addprefix $(TEST_DIR)/, main.c\
 				tokenizer/test_add_token.c\
 			)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.c, $(TEST_OBJS_DIR)/%.o, $(TEST_SRCS))
TEST_OBJS_SUBDIR := $(dir $(TEST_OBJS))

$(NAME): $(MINISHELL_BONUS_OBJS) $(TEST_OBJS) | libft
	@$(CC) $(CFLAGS) $(LDFLAGS) $(MINISHELL_BONUS_OBJS) $(TEST_OBJS) -o $@ -L$(MINISHELL_LIBFT_DIR) -L/usr/local/lib/ $(LIBS)

libft:
	@make -C $(MINISHELL_LIBFT_DIR)

minishell:
	@make bonus -C $(MINISHELL_DIR)

$(TEST_OBJS_DIR)/%.o : $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(MINISHELL_LIBFT_DIR)/include -I$(MINISHELL_INC_DIR) -Iincludes -c $? -o $@

$(MINISHELL_BONUS_OBJS_DIR)/%.o: $(MINISHELL_BONUS_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(MINISHELL_LIBFT_DIR)/include -I$(MINISHELL_INC_DIR) -Iincludes -c $? -o $@

all: $(NAME)

test: all
	./$(NAME)

clean:
	rm -rf $(TEST_OBJS_DIR)
	make clean -C $(MINISHELL_DIR)

fclean: clean
	rm -rf $(NAME)
	make fclean -C $(MINISHELL_DIR)

log:
	@echo $(TEST_OBJS)
	@echo $(TEST_OBJS_SUBDIR)
	@echo $(MINISHELL_BONUS_OBJS)