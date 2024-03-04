#include "../../include/minishell.h"

void	tknlst_sort_ascii_case(t_tknlist *list_expnd)
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
char	*remove_dup_chars(char *src)
{
	char	*new;
	size_t	size_new;
	size_t	i;
	size_t	j;

	size_new = ft_strlen(src) + 1;
	i = 0;
	while (src[i])
	{
		if(src[i + 1] && src[i + 1] == src[i])
			size_new--;
		i++;
	}
	new = (char *) ft_calloc(size_new,sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while(src[i])
	{
		if (!src[i + 1] || src[i + 1] != src[i])
			new[j++] = src[i];
		i++;
	}
	return (new);
}

int	is_compatible_file_wildcard(char *file, char **subs_needed, char *to_expand)
{
	if (to_expand[0] == '*' && file[0] == '.')
		return (0);
	if (!str_contains_all_subs_ordered(file, subs_needed))
		return (0);
	
	// if (to_expand[ft_strlen(to_expand) - 1] != '*' && s1_is_s2_suffix(subs_needed[char2len(subs_needed) - 1], file))
	// 	return (0);
	// if (to_expand[ft_strlen(to_expand) - 1] == '*' && !s1_is_s2_suffix(subs_needed[char2len(subs_needed) - 1], file))
	// 	return (0);
}

void	lstadd_compatible_cwd_files(t_tknlist *lst, char **subs_needed, char *to_expand)
{
	//ft_printf(" lstadd_compatible_cwd_files\n");
	
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
			new_tkn = create_node(WORD, strdup_gc(entry->d_name, TKN_LIST), 0); //valider pou la var link
			if (!new_tkn)
			{
				closedir(dir);
				exit(EXIT_FAILURE);//error mnagement
			}
			add_node(lst, new_tkn);
		}
		entry = readdir(dir);
	}
	closedir(dir);
}

void	expand_wildcard(t_token **p_tkn_to_expand, t_tknlist *tkn_lst) //recupe la fonction du test
{
	//ft_printf(" expand_wildcard\n");
	//splitter la str initiale
	t_token	*tkn_to_expand;
	tkn_to_expand = *p_tkn_to_expand;
	char ** splitted;
	splitted = split_gc(tkn_to_expand->content, '*', TMP); //strtrim surement pas obligatoire 

	//faire une liste dans laquelle les str compatibles sont ajoutees
	t_tknlist *wildcard_lst;
	init_list(&wildcard_lst);//gestion d' erreur sur retour ?
	lstadd_compatible_cwd_files(wildcard_lst, splitted, tkn_to_expand->content);;
	if (!wildcard_lst || !wildcard_lst->head)
		return ;
	//trier la liste dans l' ordre attendu
	tknlst_sort_ascii_case(wildcard_lst);
	add_tknlst_in_tknlst_after_target(tkn_lst, tkn_to_expand, wildcard_lst);
	pop_token_in_place(tkn_lst, tkn_to_expand);
	*p_tkn_to_expand = wildcard_lst->head;
	// display_tknlist(tkn_lst);//
	clear_garbage(TMP, free);
	return ;
}






































//idee creer un nouveau node non linke pour chaque nouvelle sortie de readdir
//creer une liste avec toutes les sorties de read dir (sans filtre)

// creer un prefixe dont on est sur

//ft_split avec * ==necessite d' avoir fait l' expansion de toutes les variables dans le token en amont
//contain substr in order, les * 