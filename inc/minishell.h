
#ifndef MINISHELL_H
# define MINISHELL_H

/* Includes: */
// Includes of our own libs:
# include "libftpp.h"
// Includes of standard libs:
# include <ctype.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>

/* Typedefs: */
// Prototypes of structs for internal use:
typedef struct s_token	t_token;
typedef struct s_redir	t_redir;

// Structs:
typedef struct s_token
{
	int		status;
	int		link_type;
	int		builtin;
	int		subshell;
	bool	cutoff;
	bool	pipe_in;
	bool	pipe_out;
	bool	if_server;
	bool	if_waiter;
	bool	subshell_end;
	char	*cmd;
	char	**argv;
	t_token	*prev;
	t_token	*next;
	t_redir	**redirs;
}	t_token;

typedef struct s_redir
{
	int		fd_src;
	int		heredoc_fd;
	bool	truncate;
	bool	is_input;
	bool	heredoc;
	char	*target;
	char	*eof;
}	t_redir;

// Enums:
typedef enum e_errors
{
	SUCCESS,
	MALLOC,
	NO_BIN,
	FORK_ERR,
	PIPE_ERR,
	PERM_ERR,
	OPEN_ERR,
	HERE_ERR
}	t_errors;

typedef enum e_links
{
	PIPE_LINK,
	IF_LINK,
	AND_LINK,
	NO_LINKS
}	t_links;

typedef enum e_bins
{
	NOT_A_BUILTIN,
	CD,
	ECHO,
	ENV,
	UNSET,
	EXPORT,
	PWD,
	EXIT
}	t_bins;

/* Function Prototypes: */
// Prototypes for 2_token:
int		ft_h_skip_space(char *str, size_t i);
int		ft_check_builtin(char *program);
void	ft_h_add_word(char ***arr, int *count, char *word);
void	ft_handle_subshell(t_token *tok, char **raw);
void	*ft_realloc(void *ptr, size_t src_size, size_t dst_size);
bool	ft_h_isspace(char c);
bool	ft_h_operator(char c);
char	*ft_get_bin(char *program, char **envp);
char	**ft_ms_cut_words(char *input);
char	***ft_ms_split(char *input);
t_token	*ft_tokenize(char ***arr, char **envp);
t_token	*ft_new_token(void);
t_token	*ft_finish_tokens(t_token *head, char **envp);
t_redir	*ft_new_redir(char *raw);

// Prototypes for 3_loop
int		ft_execute(t_token *head, char **envp, bool newhere);
int		ft_run_single(t_token **tok, char **envp);
int		ft_run_subshell(t_token **token, char **envp);
int		ft_run_pipeline(t_token **curr, char **envp);
int		ft_prepare_heredocs(t_token *head);
void	ft_exec_token(t_token *tok, char **envp);
void	ft_clean_list(t_token **list);
// Prototypes for 4_builtins
int		ft_exec_bultin(t_token *token, char **envp);

#endif
