/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 22:07:29 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/25 22:25:20 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	print_perror(char *cmd, char *msg)
{
	ft_dprintf(STDERR_FILENO, "minishell: %s: ", cmd);
	perror(msg);
}
