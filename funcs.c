#include "main.h"

static char* detail_names[] = { [first_name] = "first name",[last_name] = "last name",[id] = "id",[c_lang] = "C language",[comp_net] = "Computer Networks",[cs_f] = "CS Fundamentals",[average] = "average" };

StudentList* read_students_data()
{
	FILE* in_file;
	StudentList* student_list = (StudentList*)malloc(sizeof(StudentList));
	int line_number = 0;
	long id;
	short course_code, mark, details_result = 0;
	char line[LINE] = { 0 }, first_name[NAME] = {0}, last_name[NAME] = {0}, * token = NULL;

	if (student_list)
	{
		student_list->head = NULL;
		student_list->len = 0;
		student_list->add_counter = 0;
		student_list->update_counter = 0;
		student_list->delete_counter = 0;

		if ((in_file = fopen(DB_FILE, "r")))
		{
			while (!feof(in_file))
			{
				line_number++;
				read_line(line, in_file);

				// check & set all the student details
				if (!strlen(line) || (details_result = check_line(&token, line, first_name, last_name, &id, &course_code, &mark)) < 5)
				{
					if (strlen(line))
						printf("Input file line %d in %s - %s: invalid detail\n", line_number, detail_names[details_result], token);
					continue;
				}
				set_student(student_list, first_name, last_name, id, course_code, mark);

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

void read_line(char line[LINE], FILE* stream)
{
	char ch;
	if (fgets(line, LINE, stream))
		if (line[strlen(line) - 1] == '\n')	line[strlen(line) - 1] = '\0';
		else while ((ch = fgetc(stream)) != '\n' && ch != EOF);
	else
		line[0] = '\0';
}

void strip(const char** txt)
{
	char* end_of_txt;
	if (*txt)
	{
		while (*txt && isspace(**txt)) (*txt)++;
		if (strlen(*txt))
		{
			end_of_txt = (*txt) + strlen(*txt);
			while (--end_of_txt && isspace(*(end_of_txt))) *end_of_txt = '\0';
		}
	}
}

int find_item(char* item, char** arr, int start, int end)
{
	strip(&item);
	for (int i = start; i < end; i++)
		if (item && !strcmp(item, arr[i]))
			return i;
	return -1;
}

int check_line(const char** token, const char* line, char* first_name,
	char* last_name, long* id, short* course_code, short* mark)
{
	if (!is_ascii(line))
		return 0;
	*token = strtok(line, ",");
	// check the first name
	if (!check_name(*token))
		return 0;
	else
		strcpy(first_name, *token);
	*token = strtok(NULL, ",");

	// check the last name
	if (!check_name(*token))
		return 1;
	else
		strcpy(last_name, *token);
	*token = strtok(NULL, ",");

	// check the id
	if (!sscanf(*token, " %ld", id) || !check_id(*id))
		return 2;
	*token = strtok(NULL, ",");

	// check the course name
	*course_code = find_item(*token, detail_names, c_lang, cs_f + 1) - c_lang;
	if (*course_code == -1)
		return 3;
	*token = strtok(NULL, ",");

	// check the course mark
	if (!sscanf(*token, " %hd", mark) || !check_mark(*mark))
		return 4;

	return 5;
}

int check_name(const char* txt)
{
	if (!txt || !strlen(txt))
		return 0;
	while (*txt)
		if (isdigit(*(txt++)) || ispunct(*txt))
			return 0;
	return 1;
}

int check_id(const long id)
{
	return (id > 0 && id < 999999999);
}

int check_mark(const short mark)
{
	return (mark >= 0 && mark < 150);
}

int is_ascii(const char* txt)
{
	while (*txt)
		if (*txt < 0 || *(txt++) > 127)
			return 0;
	return 1;
}

int is_number(const char* txt)
{
	if (!txt || !strlen(txt))
		return 0;

	strip(txt);
	while (*txt)
		if (!isdigit(*(txt++)))
			return 0;
	return 1;
}

void set_student(StudentList* student_list, const char* first_name,
	const char* last_name, const long id, const short course_code, const short mark)
{
	Student* student;

	// try to find and update this student
	student = find_student(student_list, id);
	if (student)
		update_student(student, first_name, last_name, course_code, mark);
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
	for (int i = 0; i < COURSES; i++)
		if (student->marks[i] >= 0)
		{
			courses++;
			sum += student->marks[i];
		}
	return sum / courses;
}

int names_cmp(const char* name_a, const char* name_b)
{
	while (*name_a && *name_b) {
		char cmp = tolower(*(name_a++)) - tolower(*(name_b++));
		if (cmp)
			return cmp;
	}
	return *name_a - *name_b;
}

Student* create_student(const long id)
{
	// try to locate memory space for student
	Student* student = malloc(sizeof(Student));
	if (!student)
		return NULL;
	else
	{
		student->id = id;
		student->first_name = NULL;
		student->last_name = NULL;
		student->next = NULL;
		student->marks[0] = student->marks[1] = student->marks[2] = -1;
		student->marks_average = 0;

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
	strip(&first_name);
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
	strip(&last_name);
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

void print_form(StudentList* student_list)
{
	printf(SPLIT_LINE);
	printf(FORM_HEADERS);
	printf(SPLIT_LINE);
	print_all_students(student_list);
	printf(SPLIT_LINE);
	printf("\n");
}

void print_all_students(StudentList* student_list)
{
	Student* cursor = student_list->head;
	unsigned int line = 1;
	while (cursor)
	{
		print_student(cursor, line);
		cursor = cursor->next;
		line++;
	}
}

void print_student(Student* s, unsigned int line)
{
	printf("| %-5d | %-16s | %-17s |", line, s->first_name, s->last_name);
	for (int i = 0; i < COURSES; i++)
		if (s->marks[i] != -1)
			printf("  %-8d |", s->marks[i]);
		else
			printf("  -        |");
	printf("  %-8.1f | \n", s->marks_average);
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

void print_error_and_exit(const char* error, const short error_code)
{
	printf("%s", error);
	exit(error_code);
}