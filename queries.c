#include "main.h"

static char* query_names[] = { [quit] = "quit",[select] = "select",[set] = "set",[print] = "print",[del] = "del",[save] = "save",[help] = "help", [state] = "state"};

static char* detail_names[] = { [f_name] = "first name", [l_name] = "last name", [id] = "id", [c_lng] = "C language", 
		[cmp_nt] = "Computer Networks", [cs_f] = "CS Fundamentals", [avrg] = "average" };

static char* operator_names[] = { [big_eq] = ">=", [bigger] = ">" ,[sml_eq] = "<=", [smaller] = "<", [not_eq ] = "!=" ,[eq] = "=" };

static void* (*cmp_funcs[])(Student* student, const void* value) = { [f_name] = f_name_cmp, [l_name] = l_name_cmp, [id] = id_cmp,
		[c_lng] = c_lang_cmp, [cmp_nt] = cmp_nt_cmp, [cs_f] = cs_f_cmp, [avrg] = avrg_cmp};


void run_queries_loop(StudentList* student_list)
{
	char query[MAX_LEN_QUERY] = {0};
	enum Queries q_type = 1;
	printf("Welcome to the Student Management!! for help enter 'help':\n");
	do {
		printf("~>");
		read_line(query, stdin);
		if (!is_ascii(query))
		{
			printf("Invalid input.. (ASCII chars only)\n");
			q_type = 0;
		}
		else if (strlen(query))
			q_type = query_switch(query, student_list);
		else 
			q_type = 0;
	} while (q_type != quit);
}

int query_switch(char* query, StudentList* student_list)
{
	enum Queries q_type = 0;
	char token[QUERY_LEN] = {0};
	if (sscanf(query, " %6s", token))
	{
		query += strlen(token);
		q_type = find_item(token, query_names, LEN_QUERIES_TYPES);
		switch (q_type)
		{
		case quit:
		{
			//save_changes(student_list);
			break;
		}
		case select:
		{
			select_query(student_list, query);
			break;
		}
		case set:
		{
 			set_query(query, student_list);
			break;
		}
		case print:
		{
			print_all_students(student_list);
			break;
		}
		case del:
		{
			delete_query(query, student_list);
			break;
		}
		case save:
		{
			save_changes(student_list);
			break;
		}
		case help:
		{
			if (*query)
			{
				strip(&query);
				help_query(find_item(query, query_names, LEN_QUERIES_TYPES));
			}
			else
				help_query(0);
			break;
		}
		case state:
		{
			print_state(student_list);
			break;
		}
		default:
			printf("Invalid query type.. for help type 'help'.\n");
		}
	}

	return q_type;
}

void help_query(enum Queries req)
{
	switch (req)
	{
	case quit:
	{
		printf(HELP_QUIT);
		break;
	}
	case select:
	{
		printf(HELP_SELECT);
		break;
	}
	case set:
	{
		printf(HELP_SET);
		break;
	}
	case print:
	{
		printf(HELP_PRINT);
		break;
	}
	case del:
	{
		printf(HELP_DEL);
		break;
	}
	case state:
	{
		printf(HELP_STATE);
		break;
	}
	case save:
	{
		printf(HELP_SAVE);
		break;
	}
	default:
		printf(HELP_MAIN);
	}
}

void select_query(StudentList* student_list, char* query)
{
	enum Operators operater_code = 0;
	enum Details detail;
	void* value = NULL;
	if (check_select_query(query, &operater_code, &detail, &value))
		print_selection(student_list, operater_code, detail, value);
}

int find_operator(const char* query)
{
	for (enum Operators i = big_eq; i <= eq; i++)
		if (strstr(query, operator_names[i]))
			return  i;
	return 0;
}

void set_query(char* query, StudentList* student_list)
{
	unsigned int len;
	long id = 0;
	short course_code = 0, mark = 0, details_result = 0;
	char line[MAX_LEN_LINE] = {0}, first_name[MAX_LEN_NAME] = {0}, last_name[MAX_LEN_NAME] = {0};
	if (check_set_query(query, first_name, last_name, &id, &course_code, &mark) >= 2 && id)
	{
		len = student_list->len;
		set_student(student_list, first_name, last_name, id, course_code, mark);
		if (len == student_list->len)
			student_list->update_counter++;
		else
			student_list->add_counter++;
	}
	else if (!id)
		printf("Invalid query.. for help type help set\n");
}

void save_changes(StudentList* student_list)
{
	FILE* out_file;
	if ((out_file = fopen(FILE_NAME, "w")))
	{
		Student* cursor = student_list->head;
		while (cursor)
		{
			save_student(cursor, out_file);
			cursor = cursor->next;
		}
		fclose(out_file);
		student_list->add_counter = 0;
		student_list->update_counter = 0;
		student_list->delete_counter = 0;
	}
	else
		printf("Cannot open input file\n");
}

void print_state(StudentList* sl)
{
	printf("added: %d, updated = %d, deleted: %d\n", sl->add_counter, sl->update_counter, sl->delete_counter);
}

void save_student(Student* s, FILE* out_file)
{
	for (enum Details course = c_lng; course < c_lng + COURSES_LEN; course++)
	{
		if (s->marks[course - c_lng] != -1)
			fprintf(out_file, "%s,%s,%ld,%s,%d\n", s->first_name, s->last_name, s->id, detail_names[course], s->marks[course - c_lng]);
	}
}



void print_selection(StudentList* student_list, enum Operators operater_code, enum Details detail, void* value)
{
	Student* cursor = student_list->head;
	char result, select;
	
	print_head_form();
	while (cursor)
	{
		result = select = 0;
		// compare between the values
		result = (*cmp_funcs[detail])(cursor, value);
		if (result == -2)
		{
			cursor = cursor->next;
			continue;
		}

		// check the result by the operator
		switch (operater_code)
		{
		case big_eq:
		{
			select = result >= 0;
			break;
		}
		case bigger:
		{
			select = result > 0;
			break;
		}
		case sml_eq:
		{
			select = result <= 0;
			break;
		}
		case smaller:
		{
			select = result < 0;
			break;
		}
		case not_eq:
		{
			select = result != 0;
			break;
		}
		case eq:
			select = result == 0;
		}
		
		// select or not
		if (select)
			print_student(cursor);
		cursor = cursor->next;
	}
	print_bottom_form();
	if (value)
		free(value);
}

void delete_query(const char* query, StudentList* student_list)
{
	long _id;
	Student* student;
	if (*query && sscanf(query, " %9ld", &_id) && check_id(_id))
	{
		student = remove_student_from_list(student_list, _id);
		if (student)
			free_student(student);
		else
			printf("Student not found..\n");
	}
	else
		printf("Invalid query.. for help type 'help del'\n");
}