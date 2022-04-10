#include "main.h"

static char* request_names[] = { [quit] = "quit",[select] = "select",[set] = "set",[print] = "print",[del] = "del",[save] = "save",[help] = "help" };

static char* detail_names[] = { [first_name] = "first name",[last_name] = "last name",[id] = "id",[c_lang] = "C language"\
	,[comp_net] = "Computer Networks",[cs_f] = "CS Fundamentals",[average] = "average" };

static char* operator_names[] = { [eq] = "==" ,[not_eq ] = "!=" ,[biger] = ">",[smaller] = "<",[big_eq] = ">=",[sml_eq] = "<=" };

void run_requests_server(StudentList* student_list)
{
	char request[LINE];
	short not_finish;
	printf("Welcome to the Student Management Server!! for help enter 'help':\n");
	do {
		printf("~>");
		read_line(request, stdin);
		not_finish = request_switch(request, student_list);
	} while (not_finish);
}

int get_request_code(char* request)
{
	strip(&request);
	for (enum Requests req = quit; req < REQUEST_TYPES; req++)
		if (request && !strcmp(request_names[req], request))
			return req;
	return -1;
}

int request_switch(const char* request, StudentList* student_list)
{
	enum Requests request_type;
	int temp, res;
	request = strtok(request, SPACE_BREAK);
	request_type = get_request_code(request);
	switch (request_type)
	{
	case quit:
	{
		//quit_server(student_list);
		break;
	}
	case select:
	{
		request = strtok(NULL, "\n\0");
		select_studnets(request, student_list);
		break;
	}
	case set:
	{
		temp = student_list->len;
		request = strtok(NULL, "\n\0");
		res = set_request(request, student_list);
		if (student_list->len > temp && res)
			student_list->add_counter++;
		else if (res)
			student_list->update_counter++;
		break;
	}
	case print:
	{
		print_form(student_list);
		break;
	}
	case del:
	{
		delete_studnets(request, student_list);
		break;
	}
	case save:
	{
		save_changes(student_list);
		break;
	}
	case help:
	{
		request = strtok(NULL, SPACE_BREAK);
		if (request)
			get_help(get_request_code(request));
		else
			get_help(-1);
		break;
	}
	default:
		printf("Invalid request type.. for help type 'help'.\n");
	}

	return request_type;
}

void get_help(enum Requests req)
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
	case save:
	{
		printf(HELP_SAVE);
		break;
	}
	default:
		printf(HELP_MAIN);
	}
}

Student** select_studnets(const char* request, StudentList* student_list)
{
	enum Details detail_code;
	enum Operators operater_code;
	request = strtok(request, OPERATOR_BREAK);
	detail_code = find_item(request, detail_names, first_name, average + 1);

	request += strlen(request);
	request = strtok(NULL, "\n\0");
	operater_code = find_item(request, operator_names, eq, sml_eq + 1);

	printf("%d %d\n", detail_code, operater_code);
	return NULL;
}

int set_request(const char* request, StudentList* student_list)
{
	long id;
	short course_code, mark, details_result;
	char line[LINE], first_name[NAME], last_name[NAME], * token = NULL;
	if (check_request(&token, request, first_name, last_name, &id, &course_code, &mark))
		set_student(student_list, first_name, last_name, id, course_code, mark);
	else
		return 0;

	return set;
}

int check_request(const char** token, const char* request, char* f_name, char* l_name, long* _id, short* course_code, short* mark)
{
	enum Detail detail;
	long temp;
	unsigned short counter = 0, res = 1;
	char name[NAME];

	token = strtok(request, ",=\0");
	while (token && res)
	{
		if (!(counter % 2))
			detail = find_item(token, detail_names, first_name, average + 1);
		else
			switch (detail)
			{
			case first_name:
			{
				res = res && check_name(token);
				if (res)
					strcpy(f_name, token);
				break;
			}
			case last_name:
			{
				res = res && check_name(token);
				if (res)
					strcpy(l_name, token);
				break;
			}
			case id:
			{
				temp = atoi(token);
				res = res && check_id(temp);
				if (res)
					*_id = temp;
				break;
			}
			case c_lang: case comp_net: case cs_f:
			{
				temp = atoi(token);
				res = res && check_mark(temp);
				*course_code = detail - c_lang;
				*mark = temp;
				break;
			}
			default:
				res = 0;
			}
		token = strtok(NULL, ",=\0");
		counter++;
	}
	return res;
}

void delete_studnets(const char* request, StudentList* student_list)
{

}

void save_changes(StudentList* student_list)
{

}