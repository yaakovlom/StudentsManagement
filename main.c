#include "funcs.h"

int main()
{
	FILE* in_file;

	Student* student = NULL;
	StudentList* student_list = (StudentList*)malloc(sizeof(StudentList));
	student_list->head = NULL;
	student_list->len = 0;

	char line[LINE], first_name[NAME], last_name[NAME], *token;
	int line_number = 0;
	int id, course_code, mark;

	if (!(in_file = fopen("DB_studs.csv", "r"))) print_error_and_exit("Cannot open input file", -1);
	
	do	{
		line_number++;
		fscanf(in_file, "%320[^'\n']", line); // !check..
		fgetc(in_file);

		token = strtok(line, ",");
		// check & set the first name
		if (!check_text_for_name(token))
		{
			printf("Line #%d - %s: invalid detail.\n", line_number, token);
			continue;
		}
		else
			strcpy(first_name, token);
		token = strtok(NULL, ",");

		// check & set the last name
		if (!check_text_for_name(token))
		{
			printf("Line #%d - %s: invalid detail.\n", line_number, token);
			continue;
		}
		else
			strcpy(last_name, token);
		token = strtok(NULL, ",");

		// check & set the id
		id = atoi(token);
		if (id <= 0 || id > 999999999)
		{
			printf("Line #%d - %s: invalid detail.\n", line_number, token);
			continue;
		}
		token = strtok(NULL, ",");

		// check the course name
		course_code = find_cours_code(token);
		if (course_code == -1)
		{
			printf("Line #%d - %s: invalid detail.\n", line_number, token);
			continue;
		}
		token = strtok(NULL, ",");

		// check & set the course mark
		mark = atoi(token);
		if (mark > 100 || mark < 0 || (!mark && !is_number(token)))
		{
			printf("Line #%d - %s: invalid detail.\n", line_number, token);
			continue;
		}

		// try to find and update this student
		student = find_student(student_list, id);
		if (student)
			update_mark(student, course_code, mark);
		// create a new student and add to the student_list
		else
		{
			student = create_student(id);
			update_student(student, first_name, last_name, course_code, mark);
			student_list = add_student_in_order(student_list, student);
		}

	} while (!feof(in_file));
	fclose(in_file);

	print_all_students(student_list);
	
	free_list(student_list);
	return 0;
}