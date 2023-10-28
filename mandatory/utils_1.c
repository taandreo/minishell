/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 21:03:52 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/28 21:44:15 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_and_cleanup(char **malloced_str1, char **malloced_str2)
{
	char	*new_str;

	if (!malloced_str1 || !malloced_str2 || !*malloced_str1 || !*malloced_str2)
		return (NULL);
	new_str = ft_strjoin(*malloced_str1, *malloced_str2);
	free(*malloced_str1);
	*malloced_str1 = NULL;
	free(*malloced_str2);
	*malloced_str2 = NULL;
	if (!new_str)
		return (NULL);
	return (new_str);
}

char	*join_1st_and_cleanup(char **malloced_str1, char *malloced_str2)
{
	char	*new_str;

	if (!malloced_str1 || !malloced_str2 || !*malloced_str1)
		return (NULL);
	new_str = ft_strjoin(*malloced_str1, malloced_str2);
	free(*malloced_str1);
	*malloced_str1 = NULL;
	if (!new_str)
		return (NULL);
	return (new_str);
}
