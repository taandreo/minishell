/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structures_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:59 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:33:00 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	init_command_part_fields(t_command_part *command_part)
{
	command_part->type = TOKEN_INVALID;
	command_part->u_cmd.builtin_cmd = NULL;
	command_part->u_cmd.cmd_name = NULL;
	command_part->u_cmd.grouping = NULL;
	command_part->arguments = NULL;
	command_part->redirections = NULL;
}
