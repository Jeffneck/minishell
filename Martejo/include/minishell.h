/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:07:26 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/15 11:23:23 by hanglade         ###   ########.fr       */
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

typedef	struct s_btree
{
	char **content;
	int	io[2]; //in out (peut etre compose de pipe)
	int	branch;
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
	int				link;
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
	t_btree			*b_tree;
	
}				t_mini;

/**list**/
t_token		*create_node(t_tkntype type, char *content, int link);
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
 int	echo(char **cmds);
 int	size_cmds(char **cmds); //attention fonction en double

 /**env_builtin**/
 int	env(t_env *env);

/**pwd**/
int	ft_pwd(void);

 /**export**/
 int	ft_export(char **args, t_env **env);
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
void	print_sorted_env(t_env *env);

/****/
char	*create_prompt(t_mini *mini);
void	prompt_loop(t_mini *mini);










//TEST PARSER

//tknlist_utils
void	add_after_another(t_tknlist	*list, t_token *el_flag, t_token *el_toplace);
int	is_token_operator(t_token tkn);
void	pop_token_in_place(t_tknlist *list_tkn, t_token *to_pop);
void	add_tknlst_in_tknlst_after_target(t_tknlist *list, t_token *target_tkn, t_tknlist *list_expnd);
void	swap_tokens(t_tknlist	*lst, t_token *tkn1, t_token *tkn2);
size_t	tknlst_size(t_tknlist *tknlst);
void	display_tknlist(t_tknlist *list);


//libft-extension
char	*strcut_gc(char const *str, size_t cut_begin, size_t cut_end, int id_gc);
char	*replace_substr(char *str, char *replacement, size_t start, size_t len);
char	*remove_substr(char *str, size_t start, size_t len);
size_t	ft_strlen_until(const char *str, int(*f)(char));
size_t	ft_strlen_until_not(const char *str, int(*f)(char));
int	str_contains_all_subs_ordered(char *str, char **subs);
int	char_is_in_str(char c, char *str);
int	ft_strcmp_case_insensitive(char *s1, char *s2);
int	s1_is_s2_suffix(char *s1, char *s2);

t_btree	*parser(t_mini mini);

void	verify_syntax(t_tknlist *list_tkn);

int	type_need_reducing(t_tkntype type);
void	reducer(t_token	*tkn) ;


int	is_charset_env(char c);
char	*expand_dollar(t_env *env, char *str, size_t start);
char	*expander_handler(t_env *env, t_token *tkn, t_tknlist *tkn_lst);
void	expander(t_tknlist *tkn_lst, t_env *env);

void	tknlst_sort_ascii_case(t_tknlist *list_expnd);
int	is_compatible_file_wildcard(char *file, char **subs_needed, char *to_expand);
void	lstadd_compatible_cwd_files(t_tknlist *lst, char **subs_needed, char *to_expand);
void	expand_wildcard(t_token *tkn_to_expand, t_tknlist *tkn_lst);

#endif