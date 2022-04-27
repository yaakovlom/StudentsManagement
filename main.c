#include "main.h"

int main()
{
	StudentList* student_list = read_students_data();
	if (student_list != NULL)
	{
		print_all_students(student_list);
		run_queries_loop(student_list);
		free_students_list(student_list);
	}
	else
		return -1;
	return 0;
}