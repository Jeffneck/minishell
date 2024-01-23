/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step0_comments.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 15:36:20 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/27 11:21:13 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// Si le caractère actuel dans la chaîne cmd à la position i est une guillemet simple ('),
//  alors la fonction incrémente la valeur de i de 1 pour passer au caractère suivant (après le guillemet simple). 
//Ensuite, elle parcourt la chaîne en incrémentant i jusqu'à ce qu'elle atteigne un autre guillemet simple ('), 
// ce qui signifie qu'elle saute par-dessus toute la chaîne de caractères entre guillemets simples.
// Si le caractère actuel dans la chaîne cmd à la position i est une guillemet double ("), alors la fonction effectue une opération similaire. 
//Elle incrémente i de 1 pour passer au caractère suivant (après le guillemet double) et parcourt la chaîne en incrémentant i jusqu'à ce qu'elle atteigne un autre guillemet double (").

// En résumé, cette fonction permet de gérer correctement les chaînes de caractères délimitées par des guillemets simples (' ') 
// ou des guillemets doubles (" ") dans le traitement d'une commande. Elle saute par-dessus ces chaînes de caractères pour que le reste de la commande 
// puisse être analysé sans tenir compte de leur contenu. Cela est utile pour l'interprétation des commandes dans un shell ou un analyseur syntaxique.
static void	jump_quotes(char *cmd, int *i)
{
	if (cmd[*i] && cmd[*i] == '\'')
	{
		*i += 1;
		while (cmd[*i] && cmd[*i] != '\'')
			*i += 1;
	}
	if (cmd[*i] && cmd[*i] == '"')
	{
		*i += 1;
		while (cmd[*i] && cmd[*i] != '"')
			*i += 1;
	}
}

// cette fonction remove_comments est utilisée pour supprimer les commentaires d'une ligne de commande contenue dans la chaîne input.
//  Elle prend en compte les commentaires qui commencent par un dièse (#) et qui sont précédés d'un espace ou d'une tabulation. 
// Elle parcourt la chaîne en sautant par-dessus les guillemets simples et doubles pour éviter de supprimer les dièses situés à l'intérieur de chaînes de caractères.

void	remove_comments(char **input)
{
	int		i;

	i = 0;
	jump_quotes(*input, &i); //on ne traitera pas les # s'ils sont contenus entre des guillemets
	if ((*input)[i] == '#')
	{
		(*input)[i] = '\0';
		return ;
	}
	while ((*input)[++i])
	{
		jump_quotes(*input, &i);
		if ((*input)[i] == '#' && ft_ischarset((*input)[i - 1], " \t", NULL))
		{
			(*input)[i] = '\0';
			break ;
		}
	}
}
