#include "../libft.h"

t_list	**garbage(int id)
{
	static t_list	*collector[10];

	return (&collector[id]);
}

//Realloc--------------------------------------------------------------------------------------------------------------------------------
void	*ft_realloc_gc(void *ptr, size_t prev_size, size_t new_size, int id_gc)
{
	void	*new;
	size_t	size;

	if (!ptr)
		return (NULL);
	if (mem_alloc(new_size, &new, id_gc))
	{
		mem_remove(ptr, id_gc);
		return (NULL);
	}
	if (prev_size < new_size)
		size = prev_size;
	else
		size = new_size;
	ft_memcpy(new, ptr, size);
	mem_remove(ptr, id_gc);
	return (new);
}

//Mem alloc--------------------------------------------------------------------------------------------------------------------------------
void	*add_to_garbage(void *ptr, int id_gc)
{
	if (id_gc < 0 || id_gc >= 10)
        return NULL; //gestion d' erreur a voir
	t_list *el = ft_lstnew(ptr);
    if (el == NULL)
        return (free(ptr),NULL);
    ft_lstadd_front(garbage(id_gc), el);
    return ptr;
}

//n' utiliser que lorsque ptr est NULL, remplace la memalloc de robin qui a un comportement etonnant
void *malloc_gc(size_t size, int id_gc)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	if ((!add_to_garbage(ptr, id_gc)))
		return(free(ptr), NULL);
	return (ptr);

}

//Mem remove--------------------------------------------------------------------------------------------------------------------------------
static int	clean_first_el(t_list *tmp, void *ptr_to_free, int id_gc)
{
	t_list	**alst;

	if (tmp->content == ptr_to_free)
	{
		alst = garbage(id_gc);
		*alst = tmp->next;
		free(tmp->content);
		free(tmp);
		return (1);
	}
	return (0);
}

void	mem_remove(void *ptr_to_free, int id_gc)
{
	t_list	*tmp;
	t_list	*prev_el;

	if (*garbage(id_gc) == NULL)
		return ;
	tmp = *garbage(id_gc);
	if (clean_first_el(tmp, ptr_to_free, id_gc))
		return ;
	while (tmp && tmp->content != ptr_to_free)
	{
		prev_el = tmp;
		tmp = tmp->next;
	}
	if (tmp && tmp->content == ptr_to_free)
	{
		prev_el->next = tmp->next;
		free(tmp->content);
		free(tmp);
	}
}
