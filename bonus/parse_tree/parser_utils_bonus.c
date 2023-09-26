#include "minishell_bonus.h"

t_bool	is_builtin_token(t_token_type type)
{
	if (type <= TOKEN_EXIT)
		return (true);
	return (false);
}

t_bool	is_operator_or_invalid_token(t_token_type type)
{
	if (type >= TOKEN_AND && type <= TOKEN_REDIRECTION_HEREDOC)
		return (true);
	return (false);
}

t_bool	is_operator_or_end(t_token_type type)
{
	if ((type >= TOKEN_AND && type <= TOKEN_PIPE)
		|| (type >= TOKEN_REDIRECTIONS && type <= TOKEN_REDIRECTION_HEREDOC)
		|| type == TOKEN_END)
		return (true);
	return (false);
}

t_bool is_token_command_name(t_token_type type)
{
	if (type == TOKEN_COMMAND_NAME || type == TOKEN_WILDCARD
		|| type == TOKEN_EXIT_CODE)
		return (true);
	return (false);
}
