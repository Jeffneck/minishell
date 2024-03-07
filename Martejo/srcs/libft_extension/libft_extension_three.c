#include "../../include/minishell.h"

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
	// ft_printf("s1_is_s2_suffix s1 =%s s2 =%s\n", s1, s2);
	size_t len_s1 = 0;
    size_t len_s2 = 0;
    size_t suffix_pos = 0;

    if (!s1 || !s2)
        return 0;

    len_s1 = ft_strlen(s1);
    len_s2 = ft_strlen(s2);

    if (len_s1 > len_s2)
        return 0;

    suffix_pos = len_s2 - len_s1;
    s2 = s2 + suffix_pos;

    while (*s1 && *s2) {
        if (*s1 != *s2)
            return 0;
        s1++;
        s2++;
    }

    return *s1 == '\0'; // Vérifie si s1 est une chaîne vide
}

bool s1_is_s2_prefix(char *s1, char *s2)
{
    if (!s1 || !s2)
        return (false);
    
    while (*s1 && *s2)
	{
        if (*s1 != *s2)
            return (false);
        s1++;
        s2++;
    }
	// ft_printf("prefix end\n");
    return (*s1 == '\0');
}