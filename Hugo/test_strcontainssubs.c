# include <stdio.h>

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

int main() {
    char *str1 = "hello world";
    char *subs1[] = {"hell", "wor", "ld", NULL}; // Doit retourner 1
    char *subs2[] = {"world", "hello", NULL}; // Doit retourner 0 (désordonné)
    char *subs3[] = {"he", "o w", "rld", NULL}; // Doit retourner 1
    char *subs4[] = {"hello", "planet", NULL}; // Doit retourner 0 (manquant)

    printf("Test 1: %d\n", str_contains_all_subs_ordered(str1, subs1));
    printf("Test 2: %d\n", str_contains_all_subs_ordered(str1, subs2));
    printf("Test 3: %d\n", str_contains_all_subs_ordered(str1, subs3));
    printf("Test 4: %d\n", str_contains_all_subs_ordered(str1, subs4));

    return 0;
}