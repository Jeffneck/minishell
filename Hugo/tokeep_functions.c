int	char_is_in_word(char *begin_word, char c)
{
	size_t	i;

	i = 0;
	while(begin_word[i] && !ft_isspace(begin_word[i]))
	{
		if (begin_word[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	str_contains_char(char *str, char c)
{
	size_t	i;

	i = 0;
	while(str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

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

int	is_word_begin(char *str, size_t pos)
{
	if(pos == 0)
		return(1);
	else if (is_space(str[pos - 1])
		return(1);
	return (0);
}

char	*replace_endstr(char *str, char *replacement, size_t end_pos)
{
	//utiliser pour $var
	char	*new;
	size_t	len_str;
	size_t	size_new;

	if (!str || !replacement)
		return (NULL);
	len_str = ft_strlen(str);
	size_new = (len_str - (len_str - end_pos) + ft_strlen(replacement) + 1) * sizeof(char);
	new = (char *) malloc_gc(size_new, ID);
	if (!new)
		return (NULL);// appliquer gestion d' erreur
	ft_memcpy(new, str, end_pos); //ou end_pos + 1 ou end_pos - 1
	ft_strcat(new, replacement);
	return (new);
}

