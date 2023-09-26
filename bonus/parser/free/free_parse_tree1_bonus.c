/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parse_tree1_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:37 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:32:38 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	free_command_part(t_command_part *cmd_part)
{
	if (!cmd_part)
		return ;
	if (cmd_part->type == TOKEN_COMMAND_NAME)
		free_string(cmd_part->u_cmd.cmd_name);
	else if (cmd_part->type == TOKEN_GROUP)
		free_grouping(cmd_part->u_cmd.grouping);
	else if (cmd_part->type >= TOKEN_ECHO && cmd_part->type <= TOKEN_EXIT)
		free_builtin_command(cmd_part->u_cmd.builtin_cmd);
	free_arguments(cmd_part->arguments);
	free_redirections(cmd_part->redirections);
	free(cmd_part);
}

void	free_pipeline(t_pipeline *pipe)
{
	if (!pipe)
		return ;
	free_command_part(pipe->cmd_part);
	free_pipeline(pipe->next);
	free(pipe);
}

void	free_conjunctions(t_conjunctions *conj)
{
	if (!conj)
		return ;
	free_pipeline(conj->pipeline);
	free_conjunctions(conj->next);
	free(conj);
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_pipeline(cmd->pipeline);
	free_conjunctions(cmd->conjunctions);
	free(cmd);
}

void	free_grouping(t_grouping *grouping)
{
	if (!grouping)
		return ;
	free_command(grouping->enclosed_cmd);
	free(grouping);
}
