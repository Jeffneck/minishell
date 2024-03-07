#include "../../include/minishell.h"

static void	sort_tknlst_like_wildcard(t_tknlist *list_expnd)
{
	t_token		*curr;
	t_token		*next;
	size_t		i;
	size_t		lst_size;

	i = 0;
	lst_size = tknlst_size(list_expnd);
	while(i < lst_size)
	{
		curr = list_expnd->head;
		while (curr)
		{
			next = curr->next;
			if (next && ft_strcmp_case_insensitive(curr->content, next->content) > 0)
			{
				swap_tokens(list_expnd, curr, next);
				continue;
			}
			curr = curr->next;
		}
		i++;
	}
}
// char	*remove_dup_chars(char *src)
// {
// 	char	*new;
// 	size_t	size_new;
// 	size_t	i;
// 	size_t	j;

// 	if(!src)
// 		return(NULL); // utile ou contre productif
// 	size_new = ft_strlen(src) + 1;
// 	i = 0;
// 	while (src[i])
// 	{
// 		if(src[i + 1] && src[i + 1] == src[i])
// 			size_new--;
// 		i++;
// 	}
// 	new = (char *) calloc_gc(size_new, sizeof(char), TMP);
// 	if (!new)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	while(src[i])
// 	{
// 		if (!src[i + 1] || src[i + 1] != src[i])
// 			new[j++] = src[i];
// 		i++;
// 	}
// 	return (new);
// }

static int	is_compatible_file_wildcard(char *file, char **subs_needed, char *to_expand)
{
	// ft_printf("file = %s\n", file);
	if (to_expand[0] == '*' && file[0] == '.')
		return (0);
	if (to_expand[0] != '*' && !s1_is_s2_prefix(subs_needed[0], file))
		return (0);
	// ft_printf("prefix ok\n");
	if (!str_contains_all_subs_ordered(file, subs_needed))
		return (0);
	// ft_printf("all subs ok\n");
	if (to_expand[ft_strlen(to_expand) - 1] != '*' && !s1_is_s2_suffix(subs_needed[char2len(subs_needed) - 1], file))
		return (0);
	// ft_printf("suffix ok\n");
	return(1);
}

static void	lstadd_wildcard_expansions(t_tknlist *wildcard_lst, char **subs_needed, char *to_expand)
{
	//ft_printf(" lstadd_wildcard_expansions\n");
	
	DIR *dir;
    struct dirent *entry;
	t_token	*new_tkn;

	dir = opendir(".");
    if (dir == NULL)
		exit(EXIT_FAILURE); //retour d'erreur specifique
	entry = readdir(dir);
	while (entry)
	{
		if (is_compatible_file_wildcard(entry->d_name, subs_needed, to_expand))
		{
			new_tkn = create_node(WORD, strdup_gc(entry->d_name, TKN_LIST), 0); //valider pou la var link (le 0 final), a l' air ok
			if (!new_tkn)
			{
				closedir(dir); //probleme, en cas d' erreur de malloc, create node free et exit.
				exit(EXIT_FAILURE);//error mnagement
			}
			add_node(wildcard_lst, new_tkn);
		}
		entry = readdir(dir);
	}
	closedir(dir);
}

t_token	*expand_wildcard(t_token *tkn_toexpand, t_tknlist *tkn_lst) //recupe la fonction du test
{
	//ft_printf(" expand_wildcard\n");
	t_tknlist *wildcard_lst;
	char ** splitted;

	splitted = split_gc(tkn_toexpand->content, '*', TMP);
	init_list(&wildcard_lst);
	lstadd_wildcard_expansions(wildcard_lst, splitted, tkn_toexpand->content);
	if (!wildcard_lst->head)
		return (tkn_toexpand);
	//trier la liste dans l' ordre attendu
	sort_tknlst_like_wildcard(wildcard_lst);
	add_tknlst_in_tknlst_after_target(tkn_lst, tkn_toexpand, wildcard_lst);
	pop_token_in_place(tkn_lst, tkn_toexpand);
	tkn_toexpand = wildcard_lst->head;
	// display_tknlist(tkn_lst);//
	clear_garbage(TMP, free); // on peut supprimer a chaque grande etape plutot
	return (wildcard_lst->tail);
}






































//idee creer un nouveau node non linke pour chaque nouvelle sortie de readdir
//creer une liste avec toutes les sorties de read dir (sans filtre)

// creer un prefixe dont on est sur

//ft_split avec * ==necessite d' avoir fait l' expansion de toutes les variables dans le token en amont
//contain substr in order, les * 