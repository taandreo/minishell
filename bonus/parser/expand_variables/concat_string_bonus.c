/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_string_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:55:23 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 20:20:27 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
