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