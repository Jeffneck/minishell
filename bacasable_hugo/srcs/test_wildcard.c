#include "../include/minishell.h"


int	str_contains_all_subs_ordered(char *str, char **subs)
{
	size_t	i;
	size_t	sub;
	size_t	j;
	
	if (!str || !subs)
		return (0);
	if (!subs[0])
		return (1);
	i = 0;
	sub = 0;
	j = 0;
	while(str[i])
	{
		if (subs[sub][j] == str[i])
			j++;
		if (!subs[sub][j])
		{
			printf("a");//
			sub++;
			j = 0;
			if (!subs[sub])
				return (1);
			continue;
		}
		i++;
	}
	return (0);
}

// t_list	**create_expnd_lst_wildcards(char	*str, char	**subs)
// {
// 	static t_list	**lst_out;
// 	t_list	*el;

// 	if (str_contains_all_subs_ordered(str, subs));
// 	{
// 		el = ft_lstnew(str);
// 		if (!el)
// 			return (ft_lstclear(lst_out, free), NULL);
// 		ft_lstadd_back(lst_out, el);
// 	}
// 	return (lst_out);

// }

void	add_after_another(t_tknlist	*list, t_token *el_flag, t_token *el_toplace)
{
	el_toplace->next = el_flag->next;
	el_flag->next = el_toplace;
	el_toplace->prev = el_flag;
	if (el_flag == list->tail)
		list->tail = el_toplace;
}

void	pop_node_in_place(t_tknlist *list_tkn, t_token *to_pop)
{
    if (to_pop == NULL)
		return;
    if (to_pop != list_tkn->head)
        (to_pop->prev)->next = to_pop->next;
	else
		list_tkn->head = to_pop->next;
    if (to_pop != list_tkn->tail)
        (to_pop->next)->prev = to_pop->prev;
	else 
		list_tkn->tail = to_pop->prev;
    if (to_pop->content)
		del_one_garbage(to_pop->content, LEXER);
    del_one_garbage(to_pop, LEXER);
}

// 

// int	create_expnd_lst_wildcards(char	*str, char **subs, t_tknlist *list_tkn, t_token *curr)
// {
// 	t_token	*el;

// 	if (str_contains_all_subs_ordered(str, subs));
// 	{
// 		el = ft_lstnew(str);
// 		if (!el)
// 			return (0); //error management
// 		add_after_another(list_tkn, curr, el);
// 		curr = curr->next;
// 	}
// 	return (1);

// }

void	pop_original_wildcard_token(t_tknlist *list_tkn, t_token *wildcard_tkn)
{
	static int	already_popped = 0;

	if (!already_popped)
	{
		pop_node_in_place(list_tkn, wildcard_tkn);
		already_popped = 0;
	}
}


void	display_tknlist(t_tknlist *list)
{
		t_token *curr;
		int	i;

		curr = list->head;
		i = 0;
		while (curr)
		{
			printf("--NODE (%d)--\n", i);
			printf("type = %d\n", curr->type);
			printf("content = %s\n\n", curr->content);
			curr = curr->next;
			i++;
		}
}

//faire pop le node initial de wildcard uniquement si le retour de la fonction == 0

int	is_asterisk(char c)
{
	if (c == '*')
		return (1);
	return (0);
}

int	expand_wildcard(t_tknlist *list_tkn, t_token *curr, int hide_mode) 
{
    DIR *dir;
    struct dirent *entry;
	char ** splitted;
	t_token	*new_tkn;
	
	splitted = split_gc(strtrim_gc(curr->content, "*", TMP), '*', TMP);

	//revoir cette partie -----------
    dir = opendir(".");
    if (dir == NULL)
	{
        perror("Erreur lors de l'ouverture du répertoire courant");
        exit(1);
    }
	//revoir cette partie -----------

	while ((entry = readdir(dir)) != NULL)
	{
		printf("sortie readdir = %s\n", entry->d_name);//
		if ((hide_mode && entry->d_name[0] == '.') || !str_contains_all_subs_ordered(entry->d_name, splitted))
		{
			continue;
		}
		// pop_original_wildcard_token(list_tkn, curr);
		ft_printf("node is being created\n");// 
		new_tkn = create_node(WORD, strdup_gc(entry->d_name, EXPANDER), 0); //valider pou la var link
		if (!new_tkn)
			return (clear_garbage(TMP, free), closedir(dir), 0); //error management, penser a close
		add_after_another(list_tkn, curr, new_tkn);
		curr = curr->next;
	}
	return (clear_garbage(TMP, free), closedir(dir), 1); //error management, penser a close
}


//idee creer un nouveau node non linke pour chaque nouvelle sortie de readdir
//creer une liste avec toutes les sorties de read dir (sans filtre)

// creer un prefixe dont on est sur

//ft_split avec * ==necessite d' avoir fait l' expansion de toutes les variables dans le token en amont
//contain substr in order, les * 