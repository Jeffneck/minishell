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

//expander_utils.c
//probleme, on ne peut pas gerer id_gc de l' exterieur (idee : creer une variable char ** qui contient str et replacement ou add to gc en dehors de la fonction ou toutjours utiliser le m^eme id ?
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

//expander_utils.c
//probleme, on ne peut pas gerer id_gc de l' exterieur (idee : creer une variable char ** qui contient str et replacement ou add to gc en dehors de la fonction ou toutjours utiliser le m^eme id ?
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

//avance tant que la fonction ne detecte pas un char cible
//libft
size_t	ft_strlen_until(const char *str, int(*f)(char))
{
	size_t	len;

	len = 0;
	while (str[len] && !f(str[len]))
		len++;
	return (len);
}

size_t	ft_strlen_until_char(const char *str, char c)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	return (len);
}

//avance tant que la fonction detecte un char cible
//libft
size_t	ft_strlen_until_not(const char *str, int(*f)(char))
{
	size_t	len;

	len = 0;
	while (str[len] && f(str[len]))
		len++;
	return (len);
}

size_t	ft_strlen_until_not_char(const char *str, char c)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] == c)
		len++;
	return (len);
}

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

int	char_is_in_str(char c, char *str)
{
	size_t	i;

	if (!str || !c)
		return (0);
	i = 0;
	while(str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
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

int	s1_is_s2_suffix(char *s1, char *s2)
{
	size_t	len_s2;
	size_t	len_s1;
	size_t	suffix_pos;
	// ft_printf("s1 = %s\t", s1);
	// ft_printf("s2 = %s\n", s2);
	if (!s1 || !s2)
		return (0);
	if (!s1[0])
		return (1);
	len_s2 = ft_strlen(s2);
	len_s1 = ft_strlen(s1);
	if (len_s1 < len_s2)
		return (0);
	suffix_pos = len_s2 - len_s1;
	s2 = s2 + suffix_pos;
	while(*s2)
	{
		if(*s2 != *s1)
			return (0);
		s1++;
		s2++;
	}
	return (1);
}