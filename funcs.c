#include "funcs.h"


char* courses_names[] = { [c_lang] = "C language",[comp_net] = "Computer Networks",[cs_f] = "CS Fundamentals" };

int find_cours_code(const char* course_name)
{
	for (int i = 0; i < COURSES; i++)
		if (!strcmp(course_name, courses_names[i]))
			return i;
	return -1;
}

void print_student(Student* s)
{
	printf("| %-15s | %-15s | %-9d | %-3d  %-3d  %-3d |\n", s->first_name, s->last_name, s->id, s->marks[0], s->marks[1], s->marks[2]);
}

float get_student_marks_avrage(Student *student)
{
	float courses = 0, sum = 0;
	for (int i = 0; i < COURSES; i++)
		if (student->marks[i] >= 0)
		{
			courses++;
			sum += student->marks[i];
		}
	return sum / courses;
}

int is_number(const char* txt)
{
	if (!strlen(txt))
		return 0;
	for (int i = 0; i < strlen(txt); i++)
		if (!isdigit(txt[i]) || isspace(txt[i]))
			return 0;
	return 1;
}

int names_cmp(const char* name_a, const char* name_b)
{
	while (name_a || name_b) {
		char cmp = tolower(*name_a++) - tolower(*name_b++);
		if (cmp)
			return cmp;
	}
	return 0;
}

int check_text_for_name(const char* txt)
{
	if (!strlen(txt))
		return 0;
	for (int i = 0; i < strlen(txt); i++)
		if (isdigit(txt[i]) || ispunct(txt[i]))
			return 0;
	return 1;
}

Student* create_student(const long id)
{
	// try to locate memory space for student
	Student *student = malloc(sizeof(Student));
	if (!student)
		return NULL;
	else
	{
		student->id = id;
		student->first_name = NULL;
		student->last_name = NULL;
		student->next = NULL;
		student->marks[0] = student->marks[1] = student->marks[2] = -1;

		return student;
	}
}

StudentList* add_student_in_order(StudentList* student_list, Student* student)
{
	// if the student_list is empty
	if (!student_list->head)
		student_list->head = student;
	// if the student is the smaller then the head
	else if (names_cmp(student_list->head->last_name, student->last_name) > 0)
	{
		student->next = student_list->head;
		student_list->head = student;
	}
	else
	{
		Student* pre_cursor = student_list->head, * cursor = pre_cursor->next;

		while (cursor && names_cmp(cursor->last_name, student->last_name) < 0)
		{
			pre_cursor = cursor;
			cursor = cursor->next;
		}
		pre_cursor->next = student;
		student->next = cursor;
	}

	student_list->len++;
	return student_list;
}

Student* update_student(Student* student, const char* first_name, const char* last_name, const int course_code, const int mark)
{
	if (first_name && !update_first_name(student, first_name))
	{
		printf("Cannot relocate memory");
		return NULL;
	}

	if (last_name && !update_last_name(student, last_name))
	{
		printf("Cannot relocate memory");
		return NULL;
	}

	if (mark)
		update_mark(student, course_code, mark);

	return student;
}

int update_first_name(Student* student, const char* first_name)
{
	// try to relocate memory space for student first name
	student->first_name = (char*)realloc(student->first_name, strlen(first_name) + 1);
	if (!student->first_name)
		return 0;
	else
		strcpy(student->first_name, first_name);
	return 1;
}

int update_last_name(Student* student, const char* last_name)
{
	// try to relocate memory space for student last name
	student->last_name = (char*)realloc(student->last_name, strlen(last_name) + 1);
	if (!student->last_name)
		return 0;
	else
		strcpy(student->last_name, last_name);
	return 1;
}

void update_mark(Student* student, const int course_code, const int mark)
{
	student->marks[course_code] = mark;
	student->marks_avrage = get_student_marks_avrage(student);
}

Student* find_student(StudentList* student_list, const long id)
{
	Student* cursor = student_list->head;
	while (cursor)
	{
		if (cursor->id == id)
			return cursor;
		cursor = cursor->next;
	}
	return NULL;
}

void print_all_students(StudentList* student_list)
{
	while (student_list->head)
	{
		print_student(student_list->head);
		student_list->head = student_list->head->next;
	}
}

void free_student(Student* s)
{
	if (s->first_name)
		free(s->first_name);
	if (s->last_name)
		free(s->last_name);
	free(s);
}

void free_list(StudentList* student_list)
{
	while (student_list->head)
	{
		free_student(student_list->head);
		student_list->head = student_list->head->next;
	}
	free(student_list);
}

void print_error_and_exit(const char* error, const int error_code)
{
	printf("%s", error);
	exit(error_code);
}