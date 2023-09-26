/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_add_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:29:36 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:30 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.h"

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

void test_add_token_success(void **state)
{
	(void) state;
	t_token_list *tokens = create_token_list();

	int result = add_token(&tokens, TOKEN_ECHO, "echo");
	assert_int_equal(result, 0);
	t_token_node *current = tokens->head;
	assert_int_equal(current->token.type, TOKEN_ECHO);
	assert_string_equal(current->token.value, "echo");

	result = add_token(&tokens, TOKEN_CD, "cd");
	assert_int_equal(result, 0);
	current = current->next;
	assert_int_equal(current->token.type, TOKEN_CD);
	assert_string_equal(current->token.value, "cd");

	result = add_token(&tokens, TOKEN_PWD, "pwd");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_PWD);
	assert_string_equal(current->token.value, "pwd");

	result = add_token(&tokens, TOKEN_EXPORT, "export");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_EXPORT);
	assert_string_equal(current->token.value, "export");

	result = add_token(&tokens, TOKEN_UNSET, "unset");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_UNSET);
	assert_string_equal(current->token.value, "unset");

	result = add_token(&tokens, TOKEN_ENV, "env");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_ENV);
	assert_string_equal(current->token.value, "env");

	result = add_token(&tokens, TOKEN_EXIT, "exit");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_EXIT);
	assert_string_equal(current->token.value, "exit");

	result = add_token(&tokens, TOKEN_AND, "&&");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_AND);
	assert_string_equal(current->token.value, "&&");

	result = add_token(&tokens, TOKEN_OR, "||");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_OR);
	assert_string_equal(current->token.value, "||");

	result = add_token(&tokens, TOKEN_PIPE, "|");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_PIPE);
	assert_string_equal(current->token.value, "|");

	result = add_token(&tokens, TOKEN_REDIRECTION_INPUT, "<");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_REDIRECTION_INPUT);
	assert_string_equal(current->token.value, "<");

	result = add_token(&tokens, TOKEN_REDIRECTION_OUTPUT, ">");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_REDIRECTION_OUTPUT);
	assert_string_equal(current->token.value, ">");

	result = add_token(&tokens, TOKEN_REDIRECTION_APPEND, ">>");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_REDIRECTION_APPEND);
	assert_string_equal(current->token.value, ">>");

	result = add_token(&tokens, TOKEN_REDIRECTION_HEREDOC, "<<");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_REDIRECTION_HEREDOC);
	assert_string_equal(current->token.value, "<<");

	result = add_token(&tokens, TOKEN_COMMAND_NAME, "grep");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_COMMAND_NAME);
	assert_string_equal(current->token.value, "grep");

	result = add_token(&tokens, TOKEN_FILENAME, "input.txt");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_FILENAME);
	assert_string_equal(current->token.value, "input.txt");

	result = add_token(&tokens, TOKEN_STRING, "hello");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_STRING);
	assert_string_equal(current->token.value, "hello");

	result = add_token(&tokens, TOKEN_EXIT_CODE, "$?");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_EXIT_CODE);
	assert_string_equal(current->token.value, "$?");

	result = add_token(&tokens, TOKEN_SPACE, " ");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_SPACE);
	assert_string_equal(current->token.value, " ");

	result = add_token(&tokens, TOKEN_STRING, "-n");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_STRING);
	assert_string_equal(current->token.value, "-n");

	result = add_token(&tokens, TOKEN_INVALID, "&");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_INVALID);
	assert_string_equal(current->token.value, "&");

	result = add_token(&tokens, TOKEN_ERROR, "minishell: unclosed quotes.");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_ERROR);
	assert_string_equal(current->token.value, "minishell: unclosed quotes.");

	result = add_token(&tokens, TOKEN_END, "");
	current = current->next;
	assert_int_equal(result, 0);
	assert_int_equal(current->token.type, TOKEN_END);
	assert_string_equal(current->token.value, "");
}


