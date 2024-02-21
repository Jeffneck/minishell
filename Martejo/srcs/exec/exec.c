#include "../../include/minishell.h"

void    exec(t_btree *btree_root, t_env *)
{
    // parcours en profondeur qui demarre a partir de root (non pas a partir du node le plus profond)
    // a chaque node | : creation d' un pipe et modification de l' heritage des nodes enfants (different entre child_left et child_right)
    // a chaque node redirect, le child_left voit son heritage modifie en consequence par fd_in ou fd_out en fonction du mode de redirection(IN/OUT)
    // a chaque node && : le chemin vers child_right n' est autorise que si le code d' erreur remonte est de 0
    // a chaque node || : le chemin vers child_right n' est autorise que si le code d' erreur remonte est > 0
    // chaque node herite des sorties io de son parent
    // un node () transmet une copie de l' env plutot que l' env reel ce qui fait que tout cd ou ajout de variable ne sera pas reflete sur l'env du shell.

    //les codes d' erreur sont soit remontes de child vers parent soit conserves qq part
    //les in/out sont soit contenus dans chaque node soit transmis 

}