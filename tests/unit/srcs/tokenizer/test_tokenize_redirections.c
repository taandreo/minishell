#include "tester.h"

int	tokenize_redirections(const char *input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags);

void test_tokenize_redirections(void **state)
{
	(void)state;
	t_token_list *tokens = create_token_list();
	char *input = "<<>><>abc";
	size_t input_len = strlen(input);
	t_token_flags flags = init_flags(input_len);
	size_t pos = 0;
	int status = tokenize_redirections(input, &pos, &tokens, &flags);
	assert_int_equal(status, 0);
	t_token_node *current = tokens->head;
	assert_int_equal(current->token.type, TOKEN_REDIRECTION_HEREDOC);
	assert_string_equal(current->token.value, "<<");
	assert_true(flags.has_heredoc);
	assert_false(flags.is_command);
	assert_true(flags.is_redirection);

	status = tokenize_redirections(input, &pos, &tokens, &flags);
	assert_int_equal(status, 0);
	current = current->next;
	assert_int_equal(current->token.type, TOKEN_REDIRECTION_APPEND);
	assert_string_equal(current->token.value, ">>");
	assert_false(flags.is_command);
	assert_false(flags.has_heredoc);
	assert_true(flags.is_redirection);

	status = tokenize_redirections(input, &pos, &tokens, &flags);
	assert_int_equal(status, 0);
	current = current->next;
	assert_int_equal(current->token.type, TOKEN_REDIRECTION_INPUT);
	assert_string_equal(current->token.value, "<");
	assert_false(flags.is_command);
	assert_false(flags.has_heredoc);
	assert_true(flags.is_redirection);

	status = tokenize_redirections(input, &pos, &tokens, &flags);
	assert_int_equal(status, 0);
	current = current->next;
	assert_int_equal(current->token.type, TOKEN_REDIRECTION_OUTPUT);
	assert_string_equal(current->token.value, ">");
	assert_false(flags.is_command);
	assert_false(flags.has_heredoc);
	assert_true(flags.is_redirection);

	free_token_list(&tokens);
	status = tokenize_redirections(input, &pos, &tokens, &flags);
	assert_int_equal(status, 2);
}