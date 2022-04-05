#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcs.h"


void print_error_and_exit(const char* error, int error_code)
{
	printf("%s", error);
	exit(error_code);
}