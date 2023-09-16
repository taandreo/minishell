#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <cmocka.h>
#include "minishell_bonus.h"

static FILE* original_stdout = NULL;
static FILE* temp_stdout = NULL;
static const char* temp_filename = "stdout_test.tmp";

void setup() {
    original_stdout = stdout;
    temp_stdout = fopen(temp_filename, "w");
    assert_non_null(temp_stdout);
    stdout = temp_stdout;
}

void teardown() {
    if (temp_stdout) {
        fclose(temp_stdout);
        stdout = original_stdout;
    }
}

static void echo_1n()
{
	setup();
	teardown();
}

int main(void){
    const struct CMUnitTest echo_tests[] = {
		cmocka_unit_test(echo_1n),
	};
    return cmocka_run_group_tests(echo_tests, NULL, NULL);
}