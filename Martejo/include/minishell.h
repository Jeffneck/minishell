/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:07:26 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/22 13:21:17 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <limits.h>
# include <errno.h>
# include "../libft/libft.h"
# include <sys/types.h>
# include <dirent.h>
# include <fcntl.h>

extern int g_status;

typedef enum e_id_gc
{
	TKN_LIST = 1,
	B_TREE,
	ENV,
	TMP
}	t_id_gc;

typedef enum e_tkntype
{
	WORD,
	IN,
	OUT,
	HEREDOC,
	APPEND,
	PIPE,
	AND,
	OR,
	LEFT_PAR,
	ONE_QUOTE,
	TWO_QUOTE,
	PARENTHESIS,
}	t_tkntype;


//ne pas mettre ifndef pour ne pas rendre les valeurs suivantes modifiables
# define FD_READ 0
# define FD_WRITE 1
# define FD_IN 0
# define FD_OUT 1

typedef struct s_io
{
    int fd_in;
    int fd_out;
} t_io;

typedef	struct s_btree
{
	char	**cmds;
	int		branch;
	t_tkntype	type;
	struct s_btree	*left;
	struct s_btree	*right;
}	t_btree;

typedef enum {
    false, // false vaut 0
    true   // true vaut 1
} e_bool;


typedef enum e_error
{
	NOTHING = -1,
	MALLOC_E,
	FILE_E,
	TYPE_E
}	t_error;

/**list_token**/
typedef struct s_token
{
	t_tkntype		type;
	char			*content;
	int				link; //transformer en bool
	size_t			index;
	int				priority;
	int				used_flag;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s__tknlist
{
	struct s_token	*head;
	struct s_token	*tail;
}	t_tknlist;

/**list_env**/
typedef struct	s_env
{
	char			*value;
	int				secret;
	struct s_env	*next;
}				t_env;

typedef struct	s_mini
{
	//t_token			*start;
	t_env			*env;
	t_tknlist		*tkn_lst;
	t_btree			*b_tree; //tree root
	t_io			io_global;
	
}				t_mini;

/**list**/
t_token	*create_node(t_tkntype typed, char *value, int linked);
int			add_node(t_tknlist *list, t_token *node);
void		init_list(t_tknlist **list);

/**lexer**/
t_tknlist	*lexer(char *buffer);
int			detect_type(const char c, const char c2);
void		handle_token(char *buffer, t_tknlist *list, t_tkntype type, int *i);

/**lexer_utils**/
char	*ft_strndup(char *buffer, int len, t_id_gc id); // add to libft
int			is_operator(const char c, const char c2);
void		detect_error_type(const char c);

/**token_handler**/
int			simple_quote_handler(char *buffer, t_tknlist *list);
int			pipe_handler(char *buffer, t_tknlist *list);
int			file_handler(char *buffer, t_tknlist *list, t_tkntype type);
int			cmd_handler(char *buffer, t_tknlist *list);

/**token_handler_suite**/
int			parenthese_handler(char *buffer, t_tknlist *list);
int			double_quote_handler(char *buffer, t_tknlist *list);
int	operator_handler(char *buffer, t_tknlist *list, t_tkntype type);

/**error**/
void		error_handler_lexer(int id_gc, char *msg);

/**cd**/
int	is_in_env(t_env *env, char *args);
int	update_oldpwd(t_env **env);
int	update_pwd(t_env **env);
int	go_to_path(t_env **env);
int	cd(char **cmds, t_env **env);

/**echo**/
int	echo(char **cmds, t_io fds);
int	size_cmds(char **cmds); //attention fonction en double

 /**env_builtin**/
int	env(t_env *env, t_io fds); // A voir pour changer nom

/**pwd**/
int	ft_pwd(t_io fds);

 /**export**/
int	ft_export(char **args, t_env **envt, t_io fds);
 int	is_valid_env(const char *env);

 /**Unset**/
int	unset(char **value, t_env *env);

/**env_handler**/
t_env	*init_env(char **env_array);
char	*ft_strndup(char *buffer, int len, t_id_gc id);
 void	env_add_back(t_env **env, t_env *new);
 int	env_add(char *value, t_env **env, int mod);
 void	*get_env_name_var(char *dest, char *src);

 /**env_utils**/
 size_t	size_env(t_env *lst);
 char	*env_to_str(t_env *lst);
 char	*get_env_path(t_env *env, const char *var, size_t len); // changer nom fonction ? 

 /**sort_env**/
 int	strlen_2d(char **tab); // a voir si utile
 void	sort_env(char **tab, int len_env);
void	print_sorted_env(t_env *env, t_io fds);

/****/
char	*create_prompt(t_mini *mini);
void	prompt_loop(t_mini *mini);


//TEST PARSER

//tknlist_utils
void	add_after_another(t_tknlist	*list, t_token *el_flag, t_token *el_toplace);
int	is_logical_op_tkn(t_tkntype tkntype);
int	is_cmd_tkn(t_tkntype tkntype);
int	is_redir_tkn(t_tkntype tkntype);
int	is_operator_tkn(t_tkntype tkntype);
void	pop_token_in_place(t_tknlist *list_tkn, t_token *to_pop);
void	add_tknlst_in_tknlst_after_target(t_tknlist *list, t_token *target_tkn, t_tknlist *list_expnd);
void	swap_tokens(t_tknlist	*lst, t_token *tkn1, t_token *tkn2);
size_t	tknlst_size(t_tknlist *tknlst);
void	display_tknlist(t_tknlist *list);


//libft-extension
char	*strcut_gc(char const *str, size_t cut_begin, size_t cut_end, int id_gc);
char	*ft_strcut(char const *str, size_t cut_begin, size_t cut_end);
char	*replace_substr(char *str, char *replacement, size_t start, size_t len);
char	*remove_substr(char *str, size_t start, size_t len);
size_t	ft_strlen_until(const char *str, int(*f)(char));
size_t	ft_strlen_until_not(const char *str, int(*f)(char));
size_t	ft_strlen_until_char(const char *str, char c);
size_t	ft_strlen_until_not_char(const char *str, char c);
int	str_contains_all_subs_ordered(char *str, char **subs);
int	char_is_in_str(char c, char *str);
int	ft_strcmp_case_insensitive(char *s1, char *s2);
int	s1_is_s2_suffix(char *s1, char *s2);

t_btree	*parser(t_tknlist *tknlst, t_env *env);

void	verify_syntax(t_tknlist *list_tkn);

int	type_need_reducing(t_tkntype type);
void	reducer(t_token	*tkn) ;

int is_link_sensitive(t_tkntype type);
void    linker(t_tknlist *tkn_lst);

int	is_charset_env(char c);
char	*expand_dollar(t_env *env, char *str, size_t start);
char	*expander_handler(t_env *env, t_token *tkn, t_tknlist *tkn_lst);
void	expander(t_tknlist *tkn_lst, t_env *env);

t_btree	*place_in_tree(t_btree *tree_el, t_btree *toplace, int index);
t_btree	*btree_new(t_token	*tkn_toconvert);
t_btree	*create_bin_tree(t_tknlist *tknlst, t_env *env);

//tree_utils.c
size_t	count_argc_cmd(t_token *curr);
char	**get_argv_cmd(t_token *curr, size_t argc);
char	**strdup_in_newchar2(char *delimiter);
char	**extract_cmd_argv(t_token *curr);
t_token	*find_prior_token(t_token *curr);
int	cmd_is_inside_pipe(t_btree *root, int branch_id);

//display_tree.c
void depth_first_search(t_btree *tree_el, void (*visit)(t_btree *, int));
void display_node(t_btree *tree_el, int depth);
void root_first_search(t_btree *tree_el, void (*visit)(t_btree *, int));

void	tknlst_sort_ascii_case(t_tknlist *list_expnd);
int	is_compatible_file_wildcard(char *file, char **subs_needed, char *to_expand);
void	lstadd_compatible_cwd_files(t_tknlist *lst, char **subs_needed, char *to_expand);
void	expand_wildcard(t_token **p_tkn_to_expand, t_tknlist *tkn_lst);

//exec_builtins
int	fork_builtin(t_env **envt, t_btree *tree_el, t_io fds);
int		exec_builtin(t_env **envt, t_btree *tree_el, t_io fds);
int		is_builtin(char *command);

//exec_handler
int		exec_handler(t_mini *mini, t_btree *tree_el, t_io fds);

//exec_bin
int	exec_bin(t_env *env, t_btree *tree_el, t_io fds);
void	exec_process(t_btree *tree_el, t_env *env, t_io fds);
// static char	*path_handler(t_btree *tree_el, t_env *env);
// static void	check_path(char  *command, char *path);
// static void	print_path_error(char *arg, int exit_status, int error);
char	*get_cmd_path(char *cmd, t_env *env);
char	**find_path(t_env *env);
char	*check_command_path(char *cmd, char *path);
char	*ft_strjoin_pipex(char *s1, char *s2, char *sep);
char	**env_to_char2(t_env *env);
void	free_and_exit(int exit_code);

//browse_tree.c
void traverse_heredoc_node(t_mini *mini, t_btree *tree_el, t_io io_inherited);
void traverse_redir_input_node(t_mini *mini, t_btree *tree_el, t_io io_inherited);
void traverse_redir_output_node(t_mini *mini, t_btree *tree_el, t_io io_inherited);
void traverse_pipe_node(t_mini *mini, t_btree *tree_el, t_io io_inherited);
void traverse_logical_op_node(t_mini *mini, t_btree *tree_el, t_io io_inherited);
void browse_tree(t_mini *mini, t_btree *tree_el, t_io io_inherited);

#endif