#include "../../include/minishell.h"

int is_link_sensitive(t_tkntype type)
{
    if (type == WORD || type == ONE_QUOTE || type == TWO_QUOTE || type == APPEND || type == HEREDOC || type == IN || type == OUT)
        return (1);
    return (0);
}

void    linker(t_tknlist *tkn_lst)
{
    //avancer dans la liste et fusionner les nodes en 1 seul avec strjoin quand link = 1 && curr->next && is_word_type(curr->next->type)
    t_token *curr;
    t_token *next;
    char    *new_content;
    
    curr = tkn_lst->head;
    while(curr)
    {
        next = curr->next;
        if(next && curr->link == 1 && is_link_sensitive(curr->type) && is_link_sensitive(next->type))
        {
            new_content = strjoin_gc(curr->content, next->content, TKN_LIST);
            curr->link = next->link;
            if(!new_content)
                exit(EXIT_FAILURE);//err
            del_one_garbage(curr->content, TKN_LIST);
            pop_token_in_place(tkn_lst, next);
            curr->content = new_content;
            continue;
        }
        curr = curr->next;
    }
}
