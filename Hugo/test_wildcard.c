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

char	*expand_wildcard(char *str) 
{
    DIR *dir;
    struct dirent *entry;
	char *tmp;

	char ** split;
	split = ft_split(str, '*');


    dir = opendir("."); // on souhaite ouvrir le cwd donc on reverra cette partie
    if (dir == NULL)
	{
        perror("Erreur lors de l'ouverture du répertoire courant");
        exit(1);
    }

    printf("Liste des fichiers dans le répertoire courant :\n");

    while ((entry = readdir(dir)) != NULL)
	{
		news ;
		if (!expanded)
			exit(EXIT_FAILURE); //revoir
	}
    }

    closedir(dir);
}

int main() {
    listerFichiersRepertoireCourant();
    return 0;
}


//idee creer un nouveau node non linke pour chaque nouvelle sortie de readdir
//creer une liste avec toutes les sorties de read dir (sans filtre)

// creer un prefixe dont on est sur

//ft_split avec * ==necessite d' avoir fait l' expansion de toutes les variables dans le token en amont
//contain substr in order, les * 