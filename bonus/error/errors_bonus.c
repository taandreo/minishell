/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 22:07:29 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/26 22:40:28 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	print_perror(char *cmd, char *msg)
{
	if (cmd != NULL)
		ft_dprintf(STDERR_FILENO, "minishell: %s: ", cmd);
	else
		ft_dprintf(STDERR_FILENO, "minishell: ");
	perror(msg);
}
