#include "funcs.h"

int main()
{
	FILE* in_file;
	Student *temp = malloc(sizeof(Student));
	temp->first_name = temp->last_name = NULL;
	char line[LINE], error[NAME];
	int line_number = 1;

	if (!(in_file = fopen("DB_studs.csv", "r"))) print_error_and_exit("Cannot open input file", -1);
	
	do	{
		fscanf(in_file, "%320[^'\n']", line);
		crate_student(line, temp, error);
		if (!temp)
			printf("Line #%d - %s: invalid detail.\n", line_number, error);
		else
			print_student(temp);


		line_number++;
		fgetc(in_file);
	} while (!feof(in_file));
	
	free_student(temp);
	fclose(in_file);
	return 0;
}