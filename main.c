#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcs.h"

int main()
{
	FILE* in_file;
	char ch = '\n';

	if (!(in_file = fopen("DB_studs.csv", "r"))) print_error_and_exit("Cannot open input file", -1);

	while (!feof(in_file) && ch)
	{
		printf("%c", ch);
		fscanf(in_file, "%c", &ch);
	}

	fclose(in_file);
	return 0;
}