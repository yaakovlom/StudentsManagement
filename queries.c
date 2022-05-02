#include "main.h"

void run_queries_loop(StudentList* student_list, char file_name[])
{
	char query[MAX_LEN_QUERY] = {0};
	enum Queries q_type = 0;
	printf("Welcome to the Student Management!! for help enter 'help':\n");

	do {
		printf("~>");
		read_line(query, stdin);
		if (strlen(query))
		{
			q_type = query_switch(query, student_list);
			if (q_type == q_save)
				save_changes(student_list, file_name);
		}
		else
			printf("  Invalid query type.. for help type 'help'.\n");
	} while (q_type != q_quit);
}

enum Queries  query_switch(char* query, StudentList* student_list)
{
	// array of query names
	static char* query_names[] = { [q_quit] = "quit",[q_select] = "select",[q_set] = "set",[q_print] = "print",
		[q_del] = "delete",[q_save] = "save",[q_help] = "help", [q_state] = "state"};
	enum Queries q_type = 0;
	char token[QUERY_LEN] = {0};

	if (!is_ascii(query))
	{
		printf("Invalid input.. (ASCII chars only)\n");
		return q_type;
	}
	if (sscanf(query, " %6s", token))
	{
		query += strlen(token);
		q_type = find_item(token, query_names, MAX_LEN_QUERIES_TYPES);
		switch (q_type)
		{
		case q_quit: case q_save:
		{
			break;
		}
		case q_select:
		{
			select_query(student_list, query);
			break;
		}
		case q_set:
		{
 			set_query(query, student_list);
			break;
		}
		case q_print:
		{
			print_all_students(student_list);
			break;
		}
		case q_del:
		{
			delete_query(query, student_list);
			break;
		}
		case q_help:
		{
			if (*query)
			{
				strip(&query);
				help_query(find_item(query, query_names, MAX_LEN_QUERIES_TYPES));
			}
			else
				help_query(0);
			break;
		}
		case q_state:
		{
			print_state(student_list);
			break;
		}
		default:
			printf("  Invalid query type.. for help type 'help'.\n");
		}
	}

	return q_type;
}

void help_query(enum Queries req)
{
	switch (req)
	{
	case q_quit:
	{
		printf(HELP_QUIT);
		break;
	}
	case q_select:
	{
		printf(HELP_SELECT);
		break;
	}
	case q_set:
	{
		printf(HELP_SET);
		break;
	}
	case q_print:
	{
		printf(HELP_PRINT);
		break;
	}
	case q_del:
	{
		printf(HELP_DEL);
		break;
	}
	case q_state:
	{
		printf(HELP_STATE);
		break;
	}
	case q_save:
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

enum Operators find_operator(char* query)
{
	// array of the operator strings 
	static char* operator_names[] = { [big_eq] = ">=", [bigger] = ">" ,[sml_eq] = "<=", [smaller] = "<", [not_eq ] = "!=" ,[eq] = "=" };

	for (size_t len = strlen(query); len-- && !ispunct(query[len - 1]); query[len - 1] = '\0'); // clear right side
	for (enum Operators i = big_eq; i <= eq; i++)
		if (!strcmp(query, operator_names[i]))
			return  i;
	return 0;
}

void set_query(char* query, StudentList* student_list)
{
	int course_code = 0, details_mount;
	long _id = 0;
	short mark = -1;
	char line[MAX_LEN_LINE] = { 0 }, first_name[MAX_LEN_NAME] = { 0 }, last_name[MAX_LEN_NAME] = { 0 };
	details_mount = check_set_query(query, first_name, last_name, &_id, &course_code, &mark);
	if (details_mount >= 2 && _id)
	{
		switch (set_student(student_list, first_name, last_name, _id, course_code, mark))
		{
		case updated:
		{
			student_list->update_counter++;
			printf("  %d details updated successfully.\n", details_mount - 1);
			break;
		}
		case added:
		{
			student_list->add_counter++;
			printf("  A new student added successfully.\n");
			break;
		}
		default:
			printf("  The query failed.\n");
		}
	}
	else
		printf("  Invalid query.. for help type help set\n");
}

void print_state(StudentList* sl)
{
	printf("  added: %d, updated = %d, deleted: %d\n", sl->add_counter, sl->update_counter, sl->delete_counter);
}

void print_selection(StudentList* student_list, enum Operators operater_code, enum Details detail, void* value)
{
	// array of pointers to compare functions
	static int (*cmp_funcs[])(Student*, const void*) = { [f_name] = f_name_cmp, [l_name] = l_name_cmp, [id] = id_cmp,
			[c_lng] = c_lang_cmp, [cmp_nt] = cmp_nt_cmp, [cs_f] = cs_f_cmp, [avrg] = avrg_cmp};

	Student* cursor = student_list->head;
	int result;
	unsigned int selection_len = 0;
	
	while (cursor)
	{
		result = 0;
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
			result = (result >= 0);
			break;
		}
		case bigger:
		{
			result = (result > 0);
			break;
		}
		case sml_eq:
		{
			result = (result <= 0);
			break;
		}
		case smaller:
		{
			result = (result < 0);
			break;
		}
		case not_eq:
		{
			result = (result != 0);
			break;
		}
		case eq:
			result = (result == 0);
		}
		
		// select or not
		if (result)
		{
			if (!selection_len) // if its the first student in the selection
				print_head_form();
			print_student(cursor);
			selection_len++;
		}
		cursor = cursor->next;
	}
	if (selection_len)
		print_bottom_form(selection_len);
	else
		printf("  No data found..\n");
	if (value != NULL)
		free(value);
}

void delete_query(char* query, StudentList* student_list)
{
	long _id;
	Student* student;
	strip(&query);
	if (*query && strlen(query) == 9 && check_number(query) && sscanf(query, "%ld", &_id) && check_id(_id))
	{
		student = remove_student_from_list(student_list, _id);
		if (student != NULL)
		{
			free_student(student);
			student_list->delete_counter++;
			printf("  The student deleted successfully.\n");
		}
		else
			printf("  Student not found..\n");
	}
	else
		printf("  Invalid id: %s. for help type 'help delete'\n", query);
}