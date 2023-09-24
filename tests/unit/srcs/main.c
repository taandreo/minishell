/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:29:25 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:37 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.h"

int main(void)
{
	int total_failures = 0;

//	const struct CMUnitTest test_add_token[] = {
//			cmocka_unit_test(test_create_token_list_success),
//			cmocka_unit_test(test_create_token_list_fail),
//			cmocka_unit_test(test_add_token_first_malloc_fail),
//			cmocka_unit_test(test_add_token_second_malloc_fail),
//			cmocka_unit_test(test_add_token_success),
//	};
//
//	const struct CMUnitTest test_tokenizer_helper_functions[] = {
//			cmocka_unit_test(test_peek_next),
//			cmocka_unit_test(test_is_string_start),
//			cmocka_unit_test(test_has_quotes),
//			cmocka_unit_test(test_initialize_var_string),
//			cmocka_unit_test(test_extract_quoted_string),
//			cmocka_unit_test(test_handle_quotes_success),
//	};
//
//	const struct CMUnitTest test_tokenizer_functions[] = {
//			cmocka_unit_test(test_tokenize_operators_success),
//			cmocka_unit_test(test_tokenize_redirections),
//			cmocka_unit_test(test_tokenize_quotes_success),
//	};

//	const struct CMUnitTest test_tokenizer_functions_fail[] = {
//			cmocka_unit_test(test_tokenize_operators_success),
//			cmocka_unit_test(test_tokenize_redirections_fail),
//			cmocka_unit_test(test_tokenize_operators_fail),
//			cmocka_unit_test(test_tokenize_parenthesis_fail),
//			cmocka_unit_test(test_tokenize_strings_fail),
//			cmocka_unit_test(test_tokenize_quotes_success),
//	};

	const struct CMUnitTest test_quotes_fail[] = {
//			cmocka_unit_test(test_tokenize_quotes_unclosed),
//			cmocka_unit_test(test_tokenize_quotes_fail),
//			cmocka_unit_test(test_tokenizer),
			cmocka_unit_test(test_printf),
//			cmocka_unit_test(test_handle_quotes_fail),
	};

//	total_failures += cmocka_run_group_tests(test_add_token, NULL, NULL);
//	total_failures += cmocka_run_group_tests(test_tokenizer_helper_functions,
//			NULL, NULL);
//	total_failures += cmocka_run_group_tests(test_tokenizer_functions, NULL, NULL);
//	total_failures += cmocka_run_group_tests(test_tokenizer_functions_fail, NULL, NULL);
	total_failures += cmocka_run_group_tests(test_quotes_fail, NULL, NULL);

	return (total_failures);
}
