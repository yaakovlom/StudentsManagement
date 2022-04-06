#include "funcs.h"

int main()
{
	FILE* in_file;
	StudentList* student_list = (StudentList*)malloc(sizeof(StudentList));

	student_list->head = NULL;
	student_list->len = 0;

	char line[LINE], error[NAME] = "";
	int line_number = 1;

	if (!(in_file = fopen("DB_studs.csv", "r"))) print_error_and_exit("Cannot open input file", -1);
	
	do	{
		fscanf(in_file, "%320[^'\n']", line);
		check_and_process_line(line, student_list, error);
		if (strlen(error))
		{
			printf("Line #%d - %s: invalid detail.\n", line_number, error);
			strcpy(error, "");
		}

		line_number++;
		fgetc(in_file);
	} while (!feof(in_file));

	print_all_students(student_list);
	
	free_all_students(student_list);
	free(student_list);
	fclose(in_file);
	return 0;
}