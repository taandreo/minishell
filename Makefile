.PHONY: all clean fclean re log bonus libft

CC = cc
NAME = minishell
NAME_BONUS = minishell_bonus
LIBFT = lib/libft.a
LIBFT_DIR = libft
MANDATORY_DIR := mandatory
BONUS_DIR := bonus
OBJS_DIR := objects
BONUS_OBJS_DIR := bonus_objects
INC_DIR := includes
CFLAGS = -Wall -Wextra -Werror
UNAME := $(shell uname)
LIBS := -lft -lreadline

ifeq ($(UNAME), Darwin)
	CFLAGS += -arch x86_64
endif

SRCS = $(addprefix $(MANDATORY_DIR)/, minishell.c\
			tokenizer/tokenizer.c\
		)

BONUS = $(addprefix $(BONUS_DIR)/, minishell_bonus.c\
			tokenizer/command_validation_bonus.c\
			tokenizer/tokenizer_bonus.c\
			tokenizer/command_not_found_bonus.c\
			tokenizer/redirection_validation_bonus.c\
			tokenizer/pipe_and_bonus_operator_bonus.c\
			init/init_grammar_bonus.c\
		)

OBJS = $(patsubst $(MANDATORY_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))
BONUS_OBJS = $(patsubst $(BONUS_DIR)%.c, $(BONUS_OBJS_DIR)%.o, $(BONUS))
# All Src subdirectories
SRC_SUBDIR := $(shell find $(MANDATORY_DIR) -type d)
BONUS_SUBDIR := $(shell find $(BONUS_DIR) -type d)
# Objects Subdirectories
OBJS_SUBDIR := $(subst $(MANDATORY_DIR), $(OBJS_DIR), $(SRC_SUBDIR))
BONUS_OBJS_SUBDIR := $(subst $(BONUS_DIR), $(BONUS_OBJS_DIR), $(BONUS_SUBDIR))

$(NAME): $(OBJS) | libft
	$(CC) $(CFLAGS) $(OBJS) -o $@ -L$(LIBFT_DIR) $(LIBS)

$(NAME_BONUS): $(BONUS_OBJS) | libft
	@if [ -f "$(NAME)" ]; then \
  			rm -f $(NAME); \
  	fi
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o minishell -L$(LIBFT_DIR) $(LIBS)

$(OBJS_DIR)/%.o: $(MANDATORY_DIR)/%.c
	@if [ ! -d "$(OBJS_DIR)" ]; then \
    		mkdir -p $(OBJS_SUBDIR); \
    fi
	$(CC) $(CFLAGS) -I$(LIBFT_DIR)/include -I$(INC_DIR) -c $? -o $@

$(BONUS_OBJS_DIR)/%.o: $(BONUS_DIR)/%.c
	@if [ ! -d "$(BONUS_OBJS_DIR)" ]; then \
		mkdir -p $(BONUS_OBJS_SUBDIR); \
	fi
	$(CC) $(CFLAGS) -I$(LIBFT_DIR)/include -I$(INC_DIR) -c $? -o $@

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