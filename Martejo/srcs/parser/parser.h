#ifndef PARSER_H
# define PARSER_H

#include <sys/types.h>
#include <dirent.h>
#include "../../include/minishell.h"

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

#endif