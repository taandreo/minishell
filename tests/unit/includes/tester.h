#ifndef TESTER_H
#define TESTER_H

#include "minishell_bonus.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

extern const struct CMUnitTest test_add_token[];
extern const struct CMUnitTest test_tokenizer_helper_functions[];
extern const struct CMUnitTest test_tokenizer_functions[];

void setup_malloc_behavior(size_t success_count, size_t fail_count);
void test_create_token_list_success(void **state);
void test_create_token_list_fail(void **state);
void test_add_token_first_malloc_fail(void **state);
void test_add_token_second_malloc_fail(void **state);
void test_add_token_success(void **state);
void test_peek_next(void **state);
void test_is_string_start(void **state);
void test_has_quotes(void **state);
void test_tokenize_operators_success(void **state);
void test_tokenize_redirections(void **state);

#endif
