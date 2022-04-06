#include "funcs.h"


char* courses_names[] = { [c_lang] = "C language",[comp_net] = "Computer Networks",[cs_f] = "CS Fundamentals" };

void crate_student(char* details, Student* student, char *error)
{
	int temp, course_code;
	char* token = strtok(details, ",");

	// check & set the first name
	if (!check_text_for_name(token))
	{
		strcpy(error, token);
		return;
	}
	else
	{
		// try to locate memory space
		student->first_name = (char*)realloc(student->first_name, strlen(token) + 1);
		if (student->first_name)
			strcpy(student->first_name, token);
		else
		{
			strcpy(error, "Unable to locate memory.");
			return;
		}
	}
	token = strtok(NULL, ",");

	// check & set the last name
	if (!check_text_for_name(token))
	{
		strcpy(error, token);
		return;
	}
	else
	{
		// try to locate memory space
		if (student->last_name)
		{
			student->last_name = (char*)realloc(student->last_name, strlen(token) + 1);
			strcpy(student->last_name, token);
		}
		else
		{
			strcpy(error, "Unable to locate memory.");
			return;
		}
	}
	token = strtok(NULL, ",");

	// check & set the ID
	temp = atoi(token);
	if (temp <= 0 || temp > 999999999)
	{
		strcpy(error, token);
		return;
	}
	else
		student->ID = temp;
	token = strtok(NULL, ",");

	// check the course
	course_code = find_cours_code(token);
	if (course_code == -1)
	{
		strcpy(error, token);
		return;
	}
	token = strtok(NULL, ",");

	// check & set the course mark
	temp = atoi(token);
	if (temp > 100 || temp < 0)
	{
		strcpy(error, token);
		return;
	}
	else if (temp || is_number(token)) // check digits if temp == 0
		student->marks[course_code] = temp;
	
	return ;
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
	printf("First name: %s, Last name: %s, ID: %d, Marks: %d, %d, %d\n", s->first_name, s->last_name, s->ID, s->marks[0], s->marks[1], s->marks[2]);
}

void free_student(Student* s)
{
	if (s->first_name)
		free(s->first_name);
	if (s->last_name)
		free(s->last_name);
	free(s);
}

//int set_bit(short num, short bit)
//{
//	short mask = 1;
//	mask <<= bit;
//	num |= mask;
//	return num;
//}
//
//int check_bit(short num, short bit)
//{
//	short mask = 1;
//	mask <<= bit;
//	return num & mask;
//}

void print_error_and_exit(const char* error, int error_code)
{
	printf("%s", error);
	exit(error_code);
}