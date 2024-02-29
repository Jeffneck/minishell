#include <stdio.h>
#include "libft/libft.h"

int main()
{
	int value;
	unsigned char c;

	value = ft_atoi("987654321987654321");
	printf("%d\n", value);
	c = value;
	printf("c = %d\n", c);
	return (0);
}