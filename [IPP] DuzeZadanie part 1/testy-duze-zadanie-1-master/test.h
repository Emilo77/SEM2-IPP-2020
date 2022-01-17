#include <stdio.h>

#define RUN_TEST(f)				\
	fprintf(stderr, #f "\n");	\
	f()

void Testuj();
