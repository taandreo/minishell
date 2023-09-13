#include "minishell_bonus.h"

void	advance_space(char *input, size_t *pos, t_token_list **tokens,
			size_t input_len);

t_token_list	*tokenizer(char *input, t_token_flags *flags)
{
	t_token_list	*tokens;
	char			*input_dup;
	size_t			position;
	char			c;

	position = 0;
	tokens = create_token_list();
	input_dup = ft_strdup(input);
	while (position < ft_strlen(input_dup))
	{
		c = input_dup[position];
		if (ft_is_space(c))
		{
			advance_space(input_dup, &position, &tokens, ft_strlen(input_dup));
			continue ;
		}
		flags->status = tokenize_by_category(&input_dup, &position, &tokens, flags);
		if (flags->status != SUCCESS)
			break ;
	}
	if (flags->paren_count)
		flags->status = unclosed_paren_error(&tokens);
	if (flags->status == SUCCESS)
		add_token(&tokens, TOKEN_END, "");
	free(input_dup);
	return (tokens);
}

void	advance_space(char *input, size_t *pos, t_token_list **tokens,
			size_t input_len)
{
	if (peek_next(input, *pos, input_len) == '*')
		add_token(tokens, TOKEN_SPACE, " ");
	if ((*tokens)->tail->token.type == TOKEN_EXIT_CODE && peek_next(input,
			*pos, ft_strlen(input)))
		add_token(tokens, TOKEN_SPACE, " ");
	(*pos)++;
}
