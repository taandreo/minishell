/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structures_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:59 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/07 21:12:01 by tairribe         ###   ########.fr       */
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
	command_part->cmd_path = NULL;
	command_part->forked = false;
	command_part->in_pipe[0] = -1;
	command_part->in_pipe[1] = -1;
	command_part->out_pipe[0] = -1;
	command_part->out_pipe[1] = -1;
}
