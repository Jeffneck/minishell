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

// void	pop_original_wildcard_token(t_tknlist *list_tkn, t_token *wildcard_tkn)
// {
// 	static int	already_popped = 0;

// 	if (!already_popped)
// 	{
// 		pop_node_in_place(list_tkn, wildcard_tkn);
// 		already_popped = 0;
// 	}
// }


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

int	is_str_suffix(char *suffix, char *str)
{
	size_t	len_str; 
	size_t	len_suffix; 
	size_t	suffix_pos; 
	ft_printf("suffix = %s\t", suffix);
	ft_printf("str = %s\n", str);
	if (!suffix || !str)
		return (0);
	if (!suffix[0])
		return (1);
	len_str = ft_strlen(str);
	len_suffix = ft_strlen(suffix);
	suffix_pos = len_str - len_suffix;
	if (suffix_pos < 0)
		return (0);
	str = str + suffix_pos;
	while(*str)
	{
		if(*str != *suffix)
			return (0);
		suffix++;
		str++;
	}
	return (1);
}

void	swap_tokens(t_tknlist	*lst, t_token *tkn1, t_token *tkn2)
{
    if (tkn1 == NULL || tkn2 == NULL || lst == NULL || lst->head == NULL || lst->tail == NULL)
        return ; // Vérifiez les cas d'erreur et renvoyez NULL si nécessaire

    // Si tkn1 est la tête de la liste
    if (tkn1 == lst->head) {
        lst->head = tkn2;
    } else {
        (tkn1->prev)->next = tkn2;
    }

    // Si tkn2 est la queue de la liste
    if (tkn2 == lst->tail) {
        lst->tail = tkn1;
    } else {
        (tkn2->next)->prev = tkn1;
    }

    // Mettez à jour les liens pour les nœuds tkn1 et tkn2
    t_token *tmp_prev = tkn1->prev;
    // t_token *tmp_next = tkn1->next;
	// ft_printf("tkn1 = %s\n", tkn1->content);//
	// ft_printf("tkn1->next = %s\n", tkn1->next->content);//
	// ft_printf("tkn2 = %s\n", tkn2->content);//
	// ft_printf("tkn2->next = %s\n", tkn2->next->content);//
    tkn1->prev = tkn2;
    tkn1->next = tkn2->next;
    tkn2->prev = tmp_prev;
    tkn2->next = tkn1;
	// ft_printf("after tkn1 = %s\n", tkn1->content);//
	// ft_printf("after tkn1->next = %s\n", tkn1->next->content);//
	// ft_printf("after tkn2 = %s\n", tkn2->content);//
	// ft_printf("after tkn2->next = %s\n", tkn2->next->content);//
	sleep(10);//
	// display_tknlist(lst); //test debug
}

int	ft_strcmp_case_insensitive(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i])
	{
		if (ft_tolower(s1[i]) != ft_tolower(s2[i]))
			break;
		i++;
	}
	return (ft_tolower(s1[i]) - ft_tolower(s2[i]));
}

size_t	tknlst_size(t_tknlist *tknlst)
{
	t_token	*curr;
	size_t	i;

	curr = tknlst->head;
	i = 0;
	while (curr)
	{
		curr = curr->next;
		i++;
	}
	return (i);
}

void	ascii_case_sensitive_sort(t_tknlist *list_expnd)
{
	// t_tknlist	*list_sorted;
	// size_t		i;

	// ft_bzero(list_expnd, 1);
	// curr = list_expnd->head;
	// t_target =
	// while()
	// while(curr)
	// {
	// 	if (!target || ft_strcmp_case_sensitive(curr->content, target->content) > 0)
	// 		target = curr;
	// }

	t_token		*curr;
	t_token		*next;
	size_t		i;
	size_t		lst_size;

	i = 0;
	lst_size = tknlst_size(list_expnd);
	while(i < lst_size)
	{
		curr = list_expnd->head;
		// next = curr->next;
		while (curr && next)
		{
			// ft_printf("curr = %s\n", curr->content);//
			// ft_printf("curr->next = %s\n", curr->next->content);//
			// ft_printf("real next = %s\n", next->content);//
			next = curr->next;
			if (ft_strcmp_case_insensitive(curr->content, next->content) > 0)
			{
				swap_tokens(list_expnd, curr, next);
				// ft_printf("after curr = %s\n", curr->content);//
				// ft_printf("after curr->next = %s\n", curr->next->content);//
				// ft_printf("after real next = %s\n", next->content);//
				continue;
			}
			curr = curr->next;
		}
		i++;
	}
}

void	add_tknlst_in_tknlst_after_target(t_tknlist *list, t_token *target_tkn, t_tknlist *list_expnd)
{
	if (!list || !list->head || !target_tkn || !list_expnd || !list_expnd->head)
		return ;
	if (target_tkn == list->tail)
		list->tail = list_expnd->tail;
	(list_expnd->tail)->next = target_tkn->next;
	target_tkn->next = (list_expnd->head);
}

t_tknlist	*expand_wildcard(t_token *curr, int hide_mode) //list_lexer inutile
{
    DIR *dir;
    struct dirent *entry;
	char ** splitted;
	t_token	*new_tkn;

	// t_token *init_tkn;
	// init_tkn = curr;
	// int	flag_pop_init;
	// flag_pop_init = 0;
	t_tknlist *list_expnd;
	init_list(&list_expnd);


	splitted = split_gc(strtrim_gc(curr->content, "*", TMP), '*', TMP);
	ft_printf("splitted = %p splitted[0] = %s\n", splitted, splitted[0]);

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
		if ((hide_mode && entry->d_name[0] == '.') || !str_contains_all_subs_ordered(entry->d_name, splitted) || (splitted[0] && !is_str_suffix(splitted[char2len(splitted) - 1], entry->d_name)))
			continue;
		ft_printf("node is being created\n");//
		// if (!flag_pop_init++) //inutile car gere avec la valeur de retour d' expand wildcard
		// 	pop_node_in_place(&list_lexer, init_tkn);
		new_tkn = create_node(WORD, strdup_gc(entry->d_name, EXPANDER), 0); //valider pou la var link
		if (!new_tkn)
			return (clear_garbage(TMP, free), closedir(dir), NULL); //error management, penser a close
		add_node(list_expnd, new_tkn);
		//add_after_another(list_expnd, curr, new_tkn); //inutile puisque les listes sont gerees de maniere independantes
		// curr = curr->next;
	}
	display_tknlist(list_expnd);
	// ft_printf("1\n");//
	ascii_case_sensitive_sort(list_expnd);
	// display_tknlist(list_expnd);
	return (clear_garbage(TMP, free), closedir(dir), list_expnd); //error management, penser a close
}


//idee creer un nouveau node non linke pour chaque nouvelle sortie de readdir
//creer une liste avec toutes les sorties de read dir (sans filtre)

// creer un prefixe dont on est sur

//ft_split avec * ==necessite d' avoir fait l' expansion de toutes les variables dans le token en amont
//contain substr in order, les * 