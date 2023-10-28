/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_state_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:41:07 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/28 21:41:50 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*misuse_state_error(t_parser_state *state)
{
	state->status = MISUSE;
	state->error = true;
	return (NULL);
}

void	*print_misuse_state_error(t_parser_state *state)
{
	state->status = MISUSE;
	state->error = true;
	return (return_mem_alloc_error());
}
