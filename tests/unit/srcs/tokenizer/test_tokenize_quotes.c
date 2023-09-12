#include "tester.h"

char	*extract_quoted_string(const char *input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags);
char	*lookup_variable(char *var);

void test_tokenize_quotes_unclosed(void **state)
{
	(void)state;
	// Unclosed quotes
	char *input = "\"$VAR2 test";
	t_token_list *tokens = create_token_list();
	t_token_flags flags = init_flags(strlen(input));
	size_t pos = 0;
	int exit_status = tokenize_quotes(input, &pos, &tokens, &flags);
	assert_int_equal(exit_status, 1);
	assert_null(tokens);
}

void test_tokenize_quotes_success(void **state)
{
	(void)state;
	// 4th malloc failed
	char *input = "\"$VAR2 test\"abc";
	t_token_list *tokens = create_token_list();
	t_token_flags flags = init_flags(strlen(input));
	size_t pos = 0;
	int exit_status = tokenize_quotes(input, &pos, &tokens, &flags);
	assert_int_equal(exit_status, 0);
	assert_non_null(tokens);
	assert_int_equal(tokens->tail->token.type, TOKEN_COMMAND_NAME);
	assert_string_equal(tokens->tail->token.value, "file1.txt file2.txt testabc");
}

void test_tokenize_quotes_fail(void **state)
{
	(void)state;
	// first malloc failed
	char *input = "\"$VAR2 test\"abc";
	t_token_list *tokens = create_token_list();
	t_token_flags flags = init_flags(strlen(input));
	size_t pos = 0;
	setup_malloc_behavior(0,1);
	int exit_status = tokenize_quotes(input, &pos, &tokens, &flags);
	assert_int_equal(exit_status, 2);
	assert_null(tokens);
	// 2nd malloc failed
	input = "\"$VAR2 test\"abc";
	tokens = create_token_list();
	flags = init_flags(strlen(input));
	pos = 0;
	setup_malloc_behavior(1,1);
	exit_status = tokenize_quotes(input, &pos, &tokens, &flags);
	assert_int_equal(exit_status, 2);
	assert_null(tokens);
	// 3rd malloc failed
	input = "\"$VAR2 test\"abc";
	tokens = create_token_list();
	flags = init_flags(strlen(input));
	pos = 0;
	setup_malloc_behavior(2,1);
	exit_status = tokenize_quotes(input, &pos, &tokens, &flags);
	assert_int_equal(exit_status, 2);
	assert_null(tokens);
}

void test_handle_quotes_success(void **state)
{
	(void)state;
	// Quotes with subsequent string
	char *input = "\"$VAR2 test\"abcde";
	t_token_list *tokens = create_token_list();
	t_token_flags flags = init_flags(strlen(input));
	size_t pos = 0;
	char *quoted_string = handle_quotes(input, &pos, &tokens, &flags);
	assert_string_equal(quoted_string, "file1.txt file2.txt testabcde");
	// Quotes without subsequent string
	input = "\"$VAR2 test\" abcde";
	tokens = create_token_list();
	flags = init_flags(strlen(input));
	pos = 0;
	quoted_string = handle_quotes(input, &pos, &tokens, &flags);
	assert_string_equal(quoted_string, "file1.txt file2.txt test");
}

void test_handle_quotes_fail(void **state)
{
	(void)state;
	// Unclosed quotes
	char *input = "\"$VAR2 test";
	t_token_list *tokens = create_token_list();
	t_token_flags flags = init_flags(strlen(input));
	size_t pos = 0;
	char *quoted_string = handle_quotes(input, &pos, &tokens, &flags);
	assert_null(quoted_string);
	assert_int_equal(tokens->tail->token.type, TOKEN_ERROR);
	// flags->var malloc error
	input = "\"$VAR2 test\"abc";
	tokens = create_token_list();
	flags = init_flags(strlen(input));
	pos = 0;
	setup_malloc_behavior(1,1);
	quoted_string = handle_quotes(input, &pos, &tokens, &flags);
	assert_null(quoted_string);
	// flags->string malloc error
	input = "\"$VAR2 test\"abc";
	tokens = create_token_list();
	flags = init_flags(strlen(input));
	pos = 0;
	setup_malloc_behavior(0,1);
	quoted_string = handle_quotes(input, &pos, &tokens, &flags);
	assert_null(quoted_string);
	// last flags->var malloc error
	input = "\"$VAR2 test\"abc";
	tokens = create_token_list();
	flags = init_flags(strlen(input));
	pos = 0;
	setup_malloc_behavior(2,1);
	quoted_string = handle_quotes(input, &pos, &tokens, &flags);
	assert_null(quoted_string);
}

void test_extract_quoted_string(void **state)
{
	(void)state;
	// A non-existing variable
	char *input = "\"$VAR22\"abcde";
	size_t input_len = strlen(input);
	char *var = lookup_variable("VAR22");
	t_token_list *tokens = create_token_list();
	t_token_flags flags = init_flags(input_len);
	size_t pos = 0;
	flags.quote_type = '\"';
	flags.inside_quotes = true;
	flags.string = ft_strdup("");
	char *quoted_string = extract_quoted_string(input, &pos, &tokens, &flags);
	assert_string_equal(quoted_string, var);
	// A working variable
	input = "\"$VAR2\"abcde";
	input_len = strlen(input);
	var = lookup_variable("VAR2");
	pos = 0;
	flags = init_flags(input_len);
	flags.quote_type = '\"';
	flags.inside_quotes = true;
	flags.string = ft_strdup("");
	quoted_string = extract_quoted_string(input, &pos, &tokens, &flags);
	assert_string_equal(quoted_string, var);
	// An exit code variable
	input = "\"$?\"abcde";
	input_len = strlen(input);
	pos = 0;
	flags = init_flags(input_len);
	flags.quote_type = '\"';
	flags.inside_quotes = true;
	flags.string = ft_strdup("");
	quoted_string = extract_quoted_string(input, &pos, &tokens, &flags);
	assert_int_equal(tokens->tail->token.type, TOKEN_EXIT_CODE);
}

void test_initialize_var_string(void **state)
{
	(void)state;
	char *input = "\"echo \"abc";
	size_t len = strlen(input);
	t_token_flags flags = init_flags(len);
	const char *start = input + 0;
	flags.inside_quotes = true;
	flags.quote_type = '\"';
	if (!flags.string)
		flags.string = ft_strdup("");
	flags.var = initialize_var_string(input, 0, &flags, start);
	assert_string_equal(flags.var, "\"echo \"abc");
}