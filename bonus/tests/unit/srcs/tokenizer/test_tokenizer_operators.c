/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenizer_operators.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:29:41 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:32 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.h"

int				tokenize_operators(char *input, size_t *position,
		t_token_list **tokens, t_token_flags *flags);

void test_peek_next(void **state)
{
	(void) state;
	char *str = "abc";
	char expected = 'b';
	size_t len = strlen(str);
	char b = peek_next(str, 0, len);
	assert_int_equal(expected, b);
	char c = peek_next(str, 1, len);
	assert_int_equal(expected+1, c);
	char n = peek_next(str, 2, len);
	assert_int_equal('\0',n);
}

void test_is_string_start(void **state)
{
	(void) state;
	char *str = "abc";
	t_token_flags flags = init_token_flags(strlen(str));
	assert_false(is_string_start('*', &flags));
	flags.has_heredoc = true;
	assert_true(is_string_start('*', &flags));
	flags.has_heredoc = false;
	assert_true(is_string_start('?', &flags));
	assert_false(is_string_start('&', &flags));
	assert_false(is_string_start('|', &flags));
	assert_false(is_string_start('>', &flags));
	assert_false(is_string_start('<', &flags));
	flags.var_len = 10;
	if (flags.var_len > -1)
	{
		assert_true(is_string_start('&', &flags));
		assert_true(is_string_start('|', &flags));
		assert_true(is_string_start('>', &flags));
		assert_true(is_string_start('<', &flags));
	}
	flags.var_len = -1;
	assert_true(is_string_start('$', &flags));
	assert_true(is_string_start('a', &flags));
	assert_false(is_string_start('\0', &flags));
}

void test_has_quotes(void **state)
{
	(void)state;
	assert_false(has_quotes('*'));
	assert_false(has_quotes('|'));
	assert_false(has_quotes('<'));
	assert_false(has_quotes('>'));
	assert_false(has_quotes('&'));
	assert_false(has_quotes('@'));
	assert_false(has_quotes('a'));
	assert_false(has_quotes('\0'));
	assert_true(has_quotes('\"'));
	assert_true(has_quotes('\''));
}

void test_tokenize_operators_success(void **state)
{
	(void)state;
	t_token_list *tokens = create_token_list();
	char *input = "&&|||&abc";
	size_t input_len = strlen(input);
	t_token_flags flags = init_token_flags(input_len);
	size_t pos = 0;
	int status = tokenize_operators(input, &pos, &tokens, &flags);
	assert_int_equal(status, 0);
	t_token_node *current = tokens->head;
	assert_int_equal(current->token.type, TOKEN_AND);
	assert_string_equal(current->token.value, "&&");
	assert_true(flags.is_command);
	assert_false(flags.is_redirection);
	assert_false(flags.has_command);

	status = tokenize_operators(input, &pos, &tokens, &flags);
	assert_int_equal(status, 0);
	current = current->next;
	assert_int_equal(current->token.type, TOKEN_OR);
	assert_string_equal(current->token.value, "||");
	assert_true(flags.is_command);
	assert_false(flags.is_redirection);
	assert_false(flags.has_command);

	status = tokenize_operators(input, &pos, &tokens, &flags);
	assert_int_equal(status, 0);
	current = current->next;
	assert_int_equal(current->token.type, TOKEN_PIPE);
	assert_string_equal(current->token.value, "|");
	assert_true(flags.is_command);
	assert_false(flags.is_redirection);
	assert_false(flags.has_command);

	status = tokenize_operators(input, &pos, &tokens, &flags);
	assert_int_equal(status, 0);
	current = current->next;
	assert_int_equal(current->token.type, TOKEN_INVALID);
	assert_string_equal(current->token.value, "&");
	assert_true(flags.is_command);
	assert_false(flags.is_redirection);
	assert_false(flags.has_command);

	free_token_list(&tokens);
	status = tokenize_operators(input, &pos, &tokens, &flags);
	assert_int_equal(status, 2);
}

void test_tokenize_operators_fail(void **state)
{
	(void)state;
	t_token_list *tokens = create_token_list();
	char *input = "||";
	size_t input_len = strlen(input);
	t_token_flags flags = init_token_flags(input_len);
	size_t pos = 0;
	setup_malloc_behavior(0,1);
	int status = tokenize_operators(input, &pos, &tokens, &flags);
	assert_int_equal(status, 2);
	assert_null(tokens);
}

