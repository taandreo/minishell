#include "tester.h"

static size_t successful_mallocs = 0;
static size_t failed_mallocs = 0;
static size_t current_malloc_count = 0;

void setup_malloc_behavior(size_t success_count, size_t fail_count)
{
	successful_mallocs = success_count;
	failed_mallocs = fail_count;
	current_malloc_count = 0;
}

extern void *__real_malloc(size_t size);

void *__wrap_malloc(size_t size)
{
	if (current_malloc_count < successful_mallocs) {
		current_malloc_count++;
		return __real_malloc(size);
	}

	if (current_malloc_count < (successful_mallocs + failed_mallocs)) {
		current_malloc_count++;
		return NULL;
	}

	return __real_malloc(size);
}

void test_create_token_list_success(void **state)
{
	(void) state;
	setup_malloc_behavior(1,0);
	t_token_list *tokens = create_token_list();
	assert_non_null(tokens);
	assert_null(tokens->head);
	assert_null(tokens->tail);
	assert_null(tokens->current);
	assert_int_equal(tokens->count, 0);
}

void test_create_token_list_fail(void **state)
{
	(void) state;
	setup_malloc_behavior(0,1);
	t_token_list *tokens = create_token_list();
	assert_null(tokens);
}

void test_add_token_first_malloc_fail(void **state)
{
	(void) state;
	t_token_list *tokens = create_token_list();
	setup_malloc_behavior(0,1);
	int result = add_token(&tokens, TOKEN_COMMAND_NAME, "grep");
	assert_int_equal(result, 2);
}

void test_add_token_second_malloc_fail(void **state)
{
	(void) state;
	t_token_list *tokens = create_token_list();
	setup_malloc_behavior(1,1);
	int result = add_token(&tokens, TOKEN_COMMAND_NAME, "grep");
	assert_int_equal(result, 2);
	assert_null(tokens);
}
