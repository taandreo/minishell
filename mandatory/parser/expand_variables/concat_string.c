/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:55:23 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:42:21 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_string	*concat_string(t_string *string, t_vars *vars, t_token_type type)
{
	string = expand_exit_code(string, vars, type);
	if (!string)
		return (NULL);
	string = concat_exit_code(string, vars, type);
	if (!string)
		return (NULL);
	return (string);
}
