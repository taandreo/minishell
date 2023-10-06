/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 13:42:03 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/04 21:50:27 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	builtin_unset(char **params)
{
	size_t	error;

	error = EXIT_SUCCESS;
	if (!*params)
		return (EXIT_SUCCESS);
	while (*params)
	{
		if (is_valid_env(*params))
			remove_env(*params);
		else
		{
			error = EXIT_FAILURE;
			dprintf(2, "minishell: unset: `%s': not a valid identifier\n"\
			, *params);
		}
		params++;
	}
	return (error);
}
