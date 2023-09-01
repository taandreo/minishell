#ifndef TESTER_H
#define TESTER_H

#include "minishell_bonus.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

extern const struct CMUnitTest test_add_token[];

void test_create_token_list_success(void **state);
void test_create_token_list_fail(void **state);
void test_add_token_first_malloc_fail(void **state);
void test_add_token_second_malloc_fail(void **state);

#endif
