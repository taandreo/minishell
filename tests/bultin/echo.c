#include "tester.h"

static void echo_simple()
{
	char *input[] = {"simple", "test", NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "simple test\n");
}

static void echo_simple_with_n()
{
	char *input[] = {"-n", "simple", "test", NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "simple test");
}

static void echo_no_params()
{
	char *input[] = {NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "\n");
}

static void echo_n()
{
	char *input[] = {"-n", NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "");
}

static void echo_simple_with_nnn()
{
	char *input[] = {"-nnn", "simple", "test", NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "simple test");
}

static void echo_simple_with_nna()
{
	char *input[] = {"-nna", "simple", "test", NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "-nna simple test\n");
}