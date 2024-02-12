/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:07:26 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/12 17:50:47 by hanglade         ###   ########.fr       */
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
#include <dirent.h>
# include "../libft/libft.h"

typedef enum e_id_gc
{
	LEXER = 1,
	ENV,
	EXPANDER,
	TREE,
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
int	update_oldpwd(t_env *env);
int	update_pwd(t_env *env);
int	go_to_path(t_env *env);
int	cd(char **cmds, t_env *env);

/**echo**/
 int	echo(char **cmds);
 int	size_cmds(char **cmds); //attention fonction en double

 /**env_builtin**/
 int	env(t_env *env);

/**pwd**/
int	ft_pwd(void);

 /**export**/
 int	ft_export(char **args, t_env *env);
 int	is_valid_env(const char *env);

 /**Unset**/
int	unset(char **value, t_env *env);

/**env_handler**/
t_env	*init_env(char **env_array);
char	*ft_strndup(char *buffer, int len, t_id_gc id);
 void	env_add_back(t_env **env, t_env *new);
 int	env_add(char *value, t_env *env, int mod);
 void	*get_env_name_var(char *dest, char *src);

 /**env_utils**/
 size_t	size_env(t_env *lst);
 char	*env_to_str(t_env *lst);
 char	*get_env_path(t_env *env, const char *var, size_t len); // changer nom fonction ? 

 /**sort_env**/
 int	strlen_2d(char **tab); // a voir si utile
 void	sort_env(char **tab, int len_env);
void	print_sorted_env(t_env *env);



int	expand_wildcard(t_tknlist *list_tkn, t_token *curr, int hide_mode); 
int	is_asterisk(char c);
void	display_tknlist(t_tknlist *list);
void	pop_node_in_place(t_tknlist *list_tkn, t_token *to_pop);

/****/


#endif