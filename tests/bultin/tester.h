#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <cmocka.h>
#include <stdlib.h>
#include "minishell_bonus.h"

void teardown();
void setup();
char *file_to_string(const char *filename);

static const char* temp_filename = "stdout_test.tmp";