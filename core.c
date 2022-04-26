#include "main.h"

static char* detail_names[] = { [f_name] = "first name",[l_name] = "last name",[id] = "id",[c_lng] = "C language",
	[cmp_nt] = "Computer Networks",[cs_f] = "CS Fundamentals",[avrg] = "average" };

StudentList* read_students_data()
{
	FILE* in_file;
	StudentList* student_list = (StudentList*)malloc(sizeof(StudentList));
	int line_number = 0;
	long _id = 0;
	short course_code = 0, mark = 0, details_result = 0;
	char line[MAX_LEN_LINE] = { 0 }, first_name[MAX_LEN_NAME] = { 0 }, last_name[MAX_LEN_NAME] = { 0 }, * token = NULL;

	if (student_list)
	{
		student_list->tail = student_list->head = NULL;
		student_list->len = 0;
		student_list->add_counter = 0;
		student_list->update_counter = 0;
		student_list->delete_counter = 0;

		if ((in_file = fopen(FILE_NAME, "r")))
		{
			while (!feof(in_file))
			{
				line_number++;
				read_line(line, in_file);

				// check & set all the student details
				if (!strlen(line) || (details_result = check_line(&token, line, first_name, last_name, &_id, &course_code, &mark)) < 5)
				{
					if (strlen(line))
						printf("Input file line %d in %s - %s: invalid detail\n", line_number, detail_names[details_result], token);
					continue;
				}
				set_student(student_list, first_name, last_name, _id, course_code, mark);

			}
			fclose(in_file);

			return student_list;
		}
		else
		{
			free(student_list);
			printf("Cannot open input file\n");
			return NULL;
		}
	}
	else
	{
		printf("Cannot locate memory for list\n");
		return NULL;
	}
}

void read_line(char line[MAX_LEN_LINE], FILE* stream)
{
	char ch;
	if (fgets(line, MAX_LEN_LINE, stream))
		if (line[strlen(line) - 1] == '\n')	line[strlen(line) - 1] = '\0';
		else while ((ch = fgetc(stream)) != '\n' && ch != EOF);// print
	else
		line[0] = '\0';
}

void strip(char* txt[])
{
	char* end_of_txt;
	while (*txt && isspace(**txt)) (*txt)++;
	if (*txt && strlen(*txt))
	{
		end_of_txt = (*txt) + strlen(*txt);
		while (--end_of_txt && isspace(*(end_of_txt))) *end_of_txt = '\0';
	}
}

int find_item(char* item, char** arr, unsigned int len)
{
	for (unsigned int i = 1; i < len + 1; i++)
		if (item && !strcmp(item, arr[i]))
			return i;
	return 0;
}

void set_student(StudentList* student_list, const char* first_name,
	const char* last_name, const long id, const short course_code, const short mark)
{
	short resort = 0;
	Student* student;

	// try to find and update this student
	student = find_student(student_list, id);
	if (student)
	{
		if (strcmp(last_name, student->last_name)) // re put the student in order
		{
			resort = 1;
			student = remove_student_from_list(student_list, student->id);
		}
		update_student(student, first_name, last_name, course_code, mark);
		if (resort)
			add_student_in_order(student_list, student);
	}
	// create a new student and add to the student_list
	else
	{
		student = create_student(id);
		update_student(student, first_name, last_name, course_code, mark);
		student_list = add_student_in_order(student_list, student);
	}
}

float get_student_marks_average(Student* student)
{
	float courses = 0, sum = 0;
	for (int i = 0; i < COURSES_LEN; i++)
		if (student->marks[i] >= 0)
		{
			courses++;
			sum += student->marks[i];
		}
	return sum / courses;
}

Student* create_student(const long id)
{
	// try to locate memory space for student
	Student* student = (Student*)malloc(sizeof(Student));
	if (!student)
		return NULL;
	else
	{
		student->id = id;
		student->next = NULL;
		student->last_name = student->first_name = NULL;
		student->marks[0] = student->marks[1] = student->marks[2] = -1;
		student->marks_average = 0;

		return student;
	}
}

StudentList* add_student_in_order(StudentList* student_list, Student* student)
{
	// if the student_list is empty
	if (!student_list->head)
		student_list->tail = student_list->head = student;
	// if the student is the largest
	else if (str_low_cmp(student_list->tail->last_name, student->last_name) <= 0)
	{
		student_list->tail->next = student;
		student_list->tail = student;
	}
	// if the student is the smallest
	else if (str_low_cmp(student_list->head->last_name, student->last_name) >= 0)
	{
		student->next = student_list->head;
		student_list->head = student;
	}
	// find the correct place
	else
	{
		Student* pre_cursor = student_list->head, *cursor = pre_cursor->next;

		while (cursor && cursor != student_list->tail && str_low_cmp(cursor->last_name, student->last_name) < 0)
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

Student* remove_student_from_list(StudentList* student_list, long const id)
{
	Student* pre_cursor = student_list->head, *cursor = pre_cursor->next;
	if (student_list->len < 2) // if the student list has only one student
	{
		student_list->head = student_list->tail = NULL;
		student_list->len--;
		return pre_cursor;
	}
	else if (pre_cursor->id == id) // if it's the first student on the list
	{
		student_list->head = cursor;
		student_list->len--;
		return pre_cursor;
	}
	else
		while (cursor)
		{
			if (cursor->id == id)
			{
				pre_cursor->next = cursor->next;
				student_list->len--;
				return cursor;
			}
			pre_cursor = cursor;
			cursor = cursor->next;
		}
	return NULL;
}

Student* update_student(Student* student, const char* first_name, const char* last_name, const short course_code, const short mark)
{
	if (first_name && !update_first_name(student, first_name))
	{
		printf("Cannot relocate memory for first name");
		return NULL;
	}

	if (last_name && !update_last_name(student, last_name))
	{
		printf("Cannot relocate memory fir last name");
		return NULL;
	}

	if (mark)
		update_mark(student, course_code, mark);

	return student;
}

int update_first_name(Student* student, const char* first_name)
{
	// try to relocate memory space for student first name
	char* temp = (char*)realloc(student->first_name, strlen(first_name) + 1);
	if (temp)
	{
		student->first_name = temp;
		strcpy(student->first_name, first_name);
	}
	else
		return 0;
	return 1;
}

int update_last_name(Student* student, const char* last_name)
{
	// try to relocate memory space for student last name
	char* temp = (char*)realloc(student->last_name, strlen(last_name) + 1);
	if (temp)
	{
		student->last_name = temp;
		strcpy(student->last_name, last_name);
	}
	else
		return 0;
	return 1;
}

void update_mark(Student* student, const short course_code, const short mark)
{
	student->marks[course_code] = mark;
	student->marks_average = get_student_marks_average(student);
}

void print_head_form()
{
	printf(SPLIT_LINE);
	printf(FORM_HEADERS("   first name   ", "    last name    ", "   id    ", " C Lang  ", "Comp Nets", "CS Funds ", "average"));
	printf(SPLIT_LINE);
}

void print_bottom_form()
{
	printf(SPLIT_LINE);
	printf("\n");
}

void print_all_students(StudentList* student_list)
{
	if (student_list->head)
	{
		Student* cursor = student_list->head;

		print_head_form();
		while (cursor)
		{
			print_student(cursor);
			cursor = cursor->next;
		}
		print_bottom_form();
	}
	else
		printf("There is no students in the list yet..\n\n");
}

void print_student(Student* s)
{
	printf("| %09ld | %-16s | %-17s |", s->id, s->first_name, s->last_name);
	for (int i = 0; i < COURSES_LEN; i++)
		if (s->marks[i] != -1)
			printf(" %-9d |", s->marks[i]);
		else
			printf("  -        |");
	printf(" %-7.1f | \n", s->marks_average);
}

void free_student(Student* s)
{
	if (s)
	{
		if (s->first_name)
			free(s->first_name);
		if (s->last_name)
			free(s->last_name);
		free(s);
	}
}

void free_students_list(StudentList* student_list)
{
	if (student_list && student_list->head)
	{
		Student* cursor;
		while (student_list->head)
		{
			cursor = student_list->head;
			student_list->head = student_list->head->next;
			free_student(cursor);
		}
		free(student_list);
	}
}