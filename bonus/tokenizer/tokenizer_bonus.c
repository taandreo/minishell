#include "minishell_bonus.h"

t_token_list	*tokenizer(char *input, t_token_flags *flags)
{
	t_token_list	*tokens;
	size_t			position;
	char			c;

	position = 0;
	tokens = create_token_list();
	while (position < flags->input_len)
	{
		c = input[position];
		if (ft_is_space(c))
		{
			position++;
			continue ;
		}
		flags->status = tokenize_by_category(input, &position, &tokens, flags);
		if (flags->status != SUCCESS)
			break ;
	}
	if (flags->status == SUCCESS)
		add_token(&tokens, TOKEN_END, "");
	return (tokens);
}
