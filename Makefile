#######################
#        ▘  ▌ ▜       #
#  ▌▌▀▌▛▘▌▀▌▛▌▐ █▌▛▘  #
#  ▚▘█▌▌ ▌█▌▙▌▐▖▙▖▄▌  #
#######################

#	Basic Variables:
NAME		= minishell
PNAME		= Minishell
SRC_DIR	= src/
OBJ_DIR	= .obj/
INC_DIR	= inc/

# File Variables:

TOK_FILES = builtins.c cutter.c envp.c finisher.c helpers.c lexer.c parenthesis.c realloc.c redirs.c token_maker.c utils.c
TOKEN			= $(addprefix 2_token/, $(TOK_FILES))

LOP_FILES = cleanup.c executor.c heredoc.c minishell.c pipes.c runner.c single.c subshell.c
LOOP			= $(addprefix 3_loop/, $(LOP_FILES))

SRC_FILES	= $(TOKEN) $(LOOP)
SRC				= $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ				:= $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

# Compiler Variables:
CC				= cc
CFLAGS		= -Wall -Wextra -Werror -g
LIBFT			= ./libft/libft.a

# Makefile Variables:
MAKEFLAGS += --no-print-directory

####################
#      ▜   ▗ ▗     #
#  ▛▌▀▌▐ █▌▜▘▜▘█▌  #
#  ▙▌█▌▐▖▙▖▐▖▐▖▙▖  #
#  ▌               #
####################

RESET		:= $(shell tput sgr0)
CYAN		:= $(shell tput setaf 87)
BLUE		:= $(shell tput setaf 111)
RED			:= $(shell tput setaf 160)
PINK		:= $(shell tput setaf 200)
YELLOW	:= $(shell tput setaf 220)

################
#      ▜       #
#  ▛▘▌▌▐ █▌▛▘  #
#  ▌ ▙▌▐▖▙▖▄▌  #
################

.PHONY: all true_all clean fclean re clean_install banner mlx bonus

all: $(NAME)

$(NAME): $(OBJ)
	@make libft.a
	@$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) -lreadline

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

libft.a:
	@echo "${YELLOW}Building Libft++${RESET}"
	@$(MAKE) -C ./libft/ 1>/dev/null 2>&1
	@echo "${BLUE}Libft++ installed${RESET}"

clean:
	@rm -fr $(OBJ_DIR)
	@$(MAKE) -C ./libft/ clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C ./libft/ fclean

re: fclean all
