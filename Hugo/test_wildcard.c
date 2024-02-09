#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
# include "lexer.h"


int	str_contains_all_subs_ordered(char *str, char **subs)
{
	size_t	i;
	size_t	sub;
	size_t	j;
	
	if (!str || !subs || !subs[0])
		return (0);
	i = 0;
	sub = 0;
	j = 0;
	while(str[i])
	{
		if (subs[sub][j] == str[i])
			j++;
		if (!subs[sub][j])
		{
			printf("a");
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

int	add_after_another(t_lister	*list, t_token *el_flag, t_token *el_toplace)
{
	el_toplace->next = el_flag->next;
	el_flag->next = el_toplace;
	el_toplace->prev = el_flag;
	if (el_flag = list->tail)
		list->tail = el_toplace;
}

// int	create_expnd_lst_wildcards(char	*str, char **subs, t_lister *list_tkn, t_token *curr)
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


int	expand_wildcard(t_lister *list_tkn, t_token *curr) 
{
    DIR *dir;
    struct dirent *entry;
	char ** splitted;
	t_token	*new_tkn;
	
	splitted = ft_split(curr->content, '*');
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
		if (entry->d_name[0] != '.' && str_contains_all_subs_ordered(entry->d_name, splitted))
		{
			new_tkn = create_node(WORD, entry->d_name);
			if (!new_tkn)
				return (closedir(dir), 0); //error management, penser a close
			add_after_another(list_tkn, curr, new_tkn);
			curr = curr->next;
		}
	}
	return (closedir(dir), 1); //error management, penser a close
}

void	display_tknlist(t_lister *list)
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

int main(int argc, char **argv) 
{
    t_lister list;
	t_token *tkn_toexpand1;
	t_token *tkn_toexpand2;
	if (argc == 2)
	{
		tkn_toexpand1 = create_node(WORD, argv[1]);
		add_node(&list, tkn_toexpand1);
		display_tknlist(&list);
		if (tkn_toexpand1->content[0] == '.' )
			expand_wildcard_hided(&list, tkn_toexpand1);
		else
			expand_wildcard(&list, tkn_toexpand1);
		display_tknlist(&list);
	}

	if (argc == 3)
	{
		tkn_toexpand1 = create_node(WORD, argv[1]);
		add_node(&list, tkn_toexpand1);
		tkn_toexpand2 = create_node(WORD, argv[2]);
		add_node(&list, tkn_toexpand2);
		display_tknlist(&list);
		expand_wildcard(&list, tkn_toexpand1);
		expand_wildcard(&list, tkn_toexpand2);
		display_tknlist(&list);
	}

    return 0;
}


//idee creer un nouveau node non linke pour chaque nouvelle sortie de readdir
//creer une liste avec toutes les sorties de read dir (sans filtre)

// creer un prefixe dont on est sur

//ft_split avec * ==necessite d' avoir fait l' expansion de toutes les variables dans le token en amont
//contain substr in order, les * 