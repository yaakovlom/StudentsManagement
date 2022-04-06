#include "funcs.h"


char* courses_names[] = { [c_lang] = "C language",[comp_net] = "Computer Networks",[cs_f] = "CS Fundamentals" };

void check_and_process_line(char* details, StudentList* list, char *error)
{
	int id, course_code, mark;
	char first_name[NAME], last_name[NAME];
	Student* student;
	char* token = strtok(details, ",");

	// check & set the first name
	if (!check_text_for_name(token))
	{
		strcpy(error, token);
		return;
	}
	else
		strcpy(first_name, token);
	token = strtok(NULL, ",");

	// check & set the last name
	if (!check_text_for_name(token))
	{
		strcpy(error, token);
		return;
	}
	else
		strcpy(last_name, token);
	token = strtok(NULL, ",");

	// check & set the id
	id = atoi(token);
	if (id <= 0 || id > 999999999)
	{
		strcpy(error, token);
		return;
	}
	token = strtok(NULL, ",");

	// check the course name
	course_code = find_cours_code(token);
	if (course_code == -1)
	{
		strcpy(error, token);
		return;
	}
	token = strtok(NULL, ",");

	// check & set the course mark
	mark = atoi(token);
	if (mark > 100 || mark < 0 || (!mark && !is_number(token)))
	{
		strcpy(error, token);
		return;
	}

	// try to find and update this student
	student = find_student(list, id);
	if (student)
		update_student(student, NULL, NULL, course_code, mark);
	// create a new student and add to the list
	else
	{
		student = create_student(first_name, last_name, id, course_code, mark);
		add_student_in_order(list, student);
	}
}

int is_number(char* txt)
{
	if (!strlen(txt))
		return 0;
	for (int i = 0; i < strlen(txt); i++)
		if (!isdigit(txt[i]) || isspace(txt[i]))
			return 0;
	return 1;
}

int check_text_for_name(char* txt)
{
	if (!strlen(txt))
		return 0;
	for (int i = 0; i < strlen(txt); i++)
		if(isdigit(txt[i]) || ispunct(txt[i]))
			return 0;
	return 1;
}

int find_cours_code(char* course_name)
{
	for (int i = 0; i < COURSES; i++)
		if (!strcmp(course_name, courses_names[i]))
			return i;
	return -1;
}

void print_student(Student* s)
{
	printf("First name: %s, Last name: %s, id: %d, Marks: %d, %d, %d\n", s->first_name, s->last_name, s->id, s->marks[0], s->marks[1], s->marks[2]);
}

void print_all_students(StudentList* list)
{
	while (list->head)
	{
		print_student(list->head);
		list->head = list->head->next;
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

void free_all_students(StudentList* list)
{
	while (list->head)
	{
		free_student(list->head);
		list->head = list->head->next;
	}
}

float get_student_marks_avrage(Student *student)
{
	float courses = 0, sum = 0;
	for (int i = 0; i < COURSES; i++)
		if (student->marks[i] != -1)
		{
			courses++;
			sum += student->marks[i];
		}
	return sum / courses;
}

Student* create_student(char* first_name, char* last_name, int id, int course_code, int mark)
{
	// try to locate memory space for student
	Student *student = malloc(sizeof(Student));
	if (!student)
		return NULL;
	else
	{
		// try to locate memory space for student first name
		student->first_name = (char*)malloc(strlen(first_name) + 1);
		if (!student->first_name)
			return NULL;
		else
		{
			strcpy(student->first_name, first_name);
		}

		// try to locate memory space for student last name
		student->last_name = (char*)malloc(strlen(last_name) + 1);
		if (!student->last_name)
			return NULL;
		else
		{
			strcpy(student->last_name, last_name);
		}

		student->marks[0] = student->marks[1] = student->marks[2] = 300;// invalid marks (NULL)
		student->marks[course_code] = mark;
		student->marks_avrage = (float)mark;

		student->next = NULL;

		return student;
	}
}

Student* update_student(Student* student, char* first_name, char* last_name, int course_code, int mark)
{
	if (first_name)
	{
		// try to relocate memory space for student first name
		student->first_name = (char*)realloc(student->first_name, strlen(first_name) + 1);
		if (!student->first_name)
			return NULL;
		else
		{
			strcpy(student->last_name, first_name);
		}
	}

	if (last_name)
	{
		// try to relocate memory space for student last name
		student->last_name = (char*)realloc(student->last_name, strlen(last_name) + 1);
		if (!student->last_name)
			return NULL;
		else
		{
			strcpy(student->last_name, last_name);
		}
	}

	if (mark)
	{
		student->marks[course_code] = mark;
		student->marks_avrage = get_student_marks_avrage(student);
	}
	return student;
}

void add_student_in_order(StudentList* list, Student* student)
{
	Student* pre_head;

	// if the list is empty
	if (!list->head)
		list->head = student;
	else
	{
		while (list->head && strcmp(tolower(list->head->last_name), tolower(student->last_name)) < 0)
		{
			pre_head = list->head;
			list->head = list->head->next;
		}
		student->next = list->head;
		//pre_head->next = student;
	}

	list->len++;
}

Student* find_student(StudentList* list, int id)
{
	while (list->head)
	{
		if (list->head->id == id)
			return list->head;
		list->head = list->head->next;
	}
	return NULL;
}

void print_error_and_exit(const char* error, int error_code)
{
	printf("%s", error);
	exit(error_code);
}