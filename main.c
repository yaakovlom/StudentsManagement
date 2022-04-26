#include "main.h"

int main()
{
	StudentList* student_list = read_students_data();
	if (student_list)
	{
		print_all_students(student_list);
		run_queries_loop(student_list);
		free_students_list(student_list);
	}
	else
		exit(-1);
	return 0;
}