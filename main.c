#include "main.h"

int main(int argc, char const* argv[])
{
	StudentList* student_list = read_students_data();
	if (student_list == NULL)
		return -1;

	print_all_students(student_list);
	run_queries_loop(student_list);
	free_students_list(student_list);
	return 0;
}