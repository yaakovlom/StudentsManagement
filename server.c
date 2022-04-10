#include "main.h"

static char* courses_names[] = { [c_lang] = "C language",[comp_net] = "Computer Networks",[cs_f] = "CS Fundamentals" };
static char* request_types[] = { [quit] = "quit",[select] = "select",[set] = "set",[print] = "print",[del] = "del",[save] = "save", [help] = "help" };
static char* details[] = { "first name", "last name", "id", "course name", "mark" };

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
	while (isspace(*request)) request++;
	for (enum Requests req = quit; req < REQUEST_TYPES; req++)
		if (request && !strcmp(request_types[req], request))
			return req;
	return -1;
}

int request_switch(const char* request, StudentList* student_list)
{
	char* token;
	enum Requests request_type;
	token = strtok(request, " \n\0\t,");
	request_type = get_request_code(token);
	switch (request_type)
	{
	case quit:
	{
		quit_server(student_list);
		break;
	}
	case select:
	{
		select_studnets(request, student_list);
		break;
	}
	case set:
	{
		set_studnet(request, student_list);
		break;
	}
	case print:
	{
		print_all_students(student_list);
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
		token = strtok(NULL, " \n\0\t,");
		if (token)
			get_help(get_request_code(token));
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

}

void set_studnet(const char* request, StudentList* student_list)
{

}

void delete_studnets(const char* request, StudentList* student_list)
{

}

void save_changes(StudentList* student_list)
{

}

void quit_server(StudentList* student_list)
{

}