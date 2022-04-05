#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "funcs.h"

int main()
{
	FILE* in_file;
	Student *temp;
	char line[LINE], error[NAME];
	int line_number = 1;

	if (!(in_file = fopen("DB_studs.csv", "r"))) print_error_and_exit("Cannot open input file", -1);
	
	do	{
		temp = NULL;
		fscanf(in_file, "%320[^'\n']", line);
		crate_student(line, &temp, &error);
		if (!temp)
			printf("Line #%d - %s: invalid detail.\n", line_number, error);
		else
			print_student(temp);
		line_number++;
	} while (!feof(in_file));
	

	fclose(in_file);
	return 0;
}