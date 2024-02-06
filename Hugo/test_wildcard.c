#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>


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

void listerFichiersRepertoireCourant() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir("."); // Ouvre le répertoire courant
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du répertoire courant");
        exit(1);
    }

    printf("Liste des fichiers dans le répertoire courant :\n");

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Vérifie que c'est un fichier régulier
            printf("%s", entry->d_name);
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