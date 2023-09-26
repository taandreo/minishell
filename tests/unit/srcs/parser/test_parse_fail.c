#include "tester.h"

void test_parse_fail(void **state)
{
	(void)state;
	char *str = strdup("((>infile cat<<EOF|less||echo ok)&&(echo bla && echo ok)>abc.txt)");
	t_token_flags flags = init_token_flags(strlen(str));
	t_token_list *tokens = tokenizer(str, &flags);
	if (tokens)
	{
		tokens->current = tokens->head;
		t_parser_state parse_state;
		setup_malloc_behavior(SUCCESS_MALLOC, 1);
		t_command *parse_tree = parse(tokens, &parse_state);
		assert_int_equal(parse_state.status, MISUSE);
		assert_null(parse_tree);
	}
}
