#include "validation.h"

static char* detail_names[] = { [f_name] = "first name",[l_name] = "last name",[id] = "id",[c_lng] = "C language",
	[cmp_nt] = "Computer Networks",[cs_f] = "CS Fundamentals",[avrg] = "average" };

int check_line(const char** token, const char* line, char* first_name,
	char* last_name, long* _id, short* course_code, short* mark)
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
	if (!sscanf(*token, " %ld", _id) || !check_id(*_id))
		return 2;
	*token = strtok(NULL, ",");

	// check the course name
	*course_code = find_item(*token, detail_names + id, COURSES_LEN);
	if (!(*course_code)--)
		return 3;
	*token = strtok(NULL, ",");

	// check the course mark
	if (!sscanf(*token, " %hd", mark) || !check_mark(*mark))
		return 4;

	return 5;
}

int check_select_query(const char* query, enum Details* detail_code, enum Operators* operater_code, void** value)
{
	char name[MAX_LEN_NAME];
	short mark;
	long _id;
	float _average;
	*operater_code = find_operator(query);
	if (*operater_code)
	{
		char* token = strtok(query, "<>!=");
		*detail_code = find_item(token, detail_names, LEN_DETAILS_TYPES);

		token = strtok(NULL, "<>!=");
		if (!strlen(token)) // if the operator has tow chars
			token = strtok(NULL, "<>!=");

		if (*detail_code)
		{
			switch (*detail_code)
			{
			case f_name: case l_name:
			{
				if (check_name(token))
				{
					strcpy(name, token);
					*value = name;
					return 1;
				}
				else
				{
					printf("Invalid value..\n");
				}
				break;
			}
			case id:
			{
				if (sscanf(token, " %ld", &_id) && check_id(_id))
				{
					*value = _id;
					return 1;
				}
				else
				{
					printf("Invalid value..\n");
				}
				break;
			}
			case c_lng: case cmp_nt: case cs_f:
			{
				if (sscanf(token, " %hd", &mark) && check_mark(mark))
				{
					*value = &mark;
					return 1;
				}
				else
				{
					printf("Invalid value..\n");
				}
			}
			break;
			case avrg:
			{
				if (sscanf(token, " %f", &_average) && check_mark(_average))
				{
					*value = &_average;
					return 1;
				}
				else
				{
					printf("Invalid value..\n");
				}
			}
			}

		}
		else
			printf("Invalid detail..\n");
	}
	else
		printf("Invalid operator..\n");

	return 0;
}

int check_set_query(const char** token, const char* query, char* first_name, char* last_name,
	long* _id, short* course_code, short* mark)
{
	enum Detail detail;
	long temp;
	unsigned short counter = 0, res = 0;

	token = strtok(query, ",=");
	while (token)
	{
		if (!(counter % 2))
		{
			detail = find_item(token, detail_names, LEN_DETAILS_TYPES);
			if (!detail)
				printf("Invalid detail: %s\n", (char*)token);
		}
		else
			switch (detail)
			{
			case f_name:
			{
				if (check_name(token))
					strcpy(first_name, token);
				else
					printf("Invalid detail: %s\n", (char*)token);
				break;
			}
			case l_name:
			{
				if (check_name(token))
					strcpy(last_name, token);
				else
					printf("Invalid detail: %s\n", (char*)token);
				break;
			}
			case id:
			{
				temp = atoi(token);
				if (check_id(temp))
				{
					*_id = temp;
					res = 1;
				}
				else
					printf("Invalid detail: %s\n", (char*)token);
				break;
			}
			case c_lng: case cmp_nt: case cs_f:
			{
				temp = atoi(token);
				if (check_mark(temp))
				{
					*course_code = detail - c_lng;
					*mark = temp;
				}
				else
					printf("Invalid detail: %s\n", (char*)token);
				break;
			}
			}
		token = strtok(NULL, ",=");
		counter++;
	}
	return res;
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
	return (id > 0 && id < MAX_ID);
}

int check_mark(const short mark)
{
	return (mark >= 0 && mark < MAX_MARK);
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

int names_cmp(const char* name_a, const char* name_b)
{
	while (*name_a && *name_b) {
		char cmp = tolower(*(name_a++)) - tolower(*(name_b++));
		if (cmp)
			return cmp;
	}
	return *name_a - *name_b;
}