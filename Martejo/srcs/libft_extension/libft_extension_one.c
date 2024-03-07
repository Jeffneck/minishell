#include "../../include/minishell.h"

char	*strcut_gc(char const *str, size_t cut_begin, size_t cut_end, int id_gc)
{
	char	*dest;
	int		len; //size_t au cas ou super longue chaine"

	if (!str)
		return (NULL);
	len = ft_strlen(str) - cut_begin - cut_end;
	// ft_printf("len after cut = %d", len);
	if (len <= 0)
		return (strdup_gc("", id_gc));
	dest = (char *)calloc_gc((len + 1), sizeof(char), id_gc); //utile pour le \0 automatique ?
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, &str[cut_begin], len + 1); //les char end sont bien retires sans compter le char de fin ?
	dest[len] = '\0'; //utile
	return (dest);
}

char	*ft_strcut(char const *str, size_t cut_begin, size_t cut_end)
{
	//ft_printf("ft_strcut\n");
	char	*dest;
	int		len; //size_t au cas ou super longue chaine"

	if (!str)
		return (NULL);
	len = ft_strlen(str) - cut_begin - cut_end;
	// ft_printf("len after cut = %d", len);
	if (len <= 0)
		return (ft_strdup(""));
	dest = (char *)ft_calloc((len + 1), sizeof(char)); //utile pour le \0 automatique ?
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, &str[cut_begin], len + 1); //les char end sont bien retires sans compter le char de fin ?
	dest[len] = '\0'; //utile
	return (dest);
}

char	*replace_substr(char *str, char *replacement, size_t start, size_t len)
{
	char	*new;
	size_t	size_new;

	size_new = (ft_strlen(str) - len + ft_strlen(replacement)) + 1;
	new = (char *) ft_calloc(size_new, sizeof(char));
	if (new == NULL)
		return (NULL);// appliquer gestion d' erreur
	ft_memcpy(new, str, start); //ou start + 1 ou start - 1
	ft_strcat(new, replacement);
	ft_strcat(new, &str[start + len]); //valider
	return (new);
}

char	*remove_substr(char *str, size_t start, size_t len_toremove)
{
	char	*new;
	size_t	size_new;

	
	size_new = ft_strlen(str) - len_toremove + 1;
	// printf("len_to rm = %zu, size new = %zu\n", len_toremove, size_new);//
	new = (char *) ft_calloc(size_new, sizeof(char));
	if (new == NULL)
		return (NULL);// appliquer gestion d' erreur
	ft_memcpy(new, str, start); //ou start + 1 ou start - 1
	ft_strcat(new, &str[start + len_toremove]); //valider
	return (new);
}