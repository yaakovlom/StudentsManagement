#include "funcs.h"

int main()
{
	StudentList* student_list = read_students_data();
	if (student_list)
	{
		print_form(student_list);

		// start_server();

		free_students_list(student_list);
	}
	return 0;
}