#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "minishell_bonus.h"

void teardown();
void setup();
char *file_to_string(const char *filename);

int	run_export_tests(void);
int	run_echo_tests(void);

#define TEMP_FILENAME "stdout_test.tmp"