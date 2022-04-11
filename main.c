#include "main.h"

int main()
{
	StudentList* student_list = read_students_data();
	if (student_list)
	{
		if (student_list->head)
			print_form(student_list);
		else
			printf("The input file was empty..\n\n");
		run_requests_server(student_list);
		free_students_list(student_list);
	}
	else
		print_error_and_exit("", -1);
	return 0;
}