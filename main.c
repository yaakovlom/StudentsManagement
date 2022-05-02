#include "main.h"

int main(int argc, char const* argv[])
{
	char file_name[MAX_LEN_FILE_NAME];
	if (argc > 1)
		printf("Reading data file: '%s'..\n", strcpy(file_name, argv[1]));
	else
		printf("Reading default data file: '%s'..\n", strcpy(file_name, FILE_NAME));
	
	StudentList* student_list = read_students_data(file_name);
	if (student_list == NULL)
		return -1;

	print_all_students(student_list);
	run_queries_loop(student_list, file_name);

	save_changes(student_list, file_name);
	free_students_list(student_list);
	return 0;
}