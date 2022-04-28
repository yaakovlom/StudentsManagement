#include "main.h"

static char* detail_names[] = { [f_name] = "first name",[l_name] = "last name",[id] = "id",[c_lng] = "C language",
	[cmp_nt] = "Computer Networks",[cs_f] = "CS Fundamentals",[avrg] = "average" };

static char* operator_names[] = { [big_eq] = ">=",[bigger] = ">" ,[sml_eq] = "<=",[smaller] = "<",[not_eq ] = "!=" ,[eq] = "=" };


int check_line(char** token, char* line, char* first_name,
	char* last_name, long* _id, int* course_code, short* mark)
{
	int count = 0;

	// check if all chars is in the ASCII range
	if (!is_ascii(line))
	{
		strcpy(*token, "ASCII chars only");
		return count;
	}
	*token = strtok(line, ",");

	// check the first name
	strip(token);
	if (!check_name(*token))
		return count;
	strcpy(first_name, *token);
	*token = strtok(NULL, ",");
	count++;

	// check the last name
	strip(token);
	if (!check_name(*token))
		return count;
	strcpy(last_name, *token);
	count++;
	*token = strtok(NULL, ",");

	// check the id
	strip(token);
	if (!check_number(*token) || !sscanf(*token, " %ld", _id) || !check_id(*_id))
		return count;
	*token = strtok(NULL, ",");
	count++;

	// check the course name
	strip(token);
	*course_code = find_item(*token, detail_names + id, COURSES_LEN);
	if (!*course_code)
		return count;
	(*course_code)--;
	*token = strtok(NULL, ",");
	count++;

	// check the course mark
	strip(token);
	if (!check_number(*token) || !sscanf(*token, " %hd", mark) || !check_mark(*mark))
		return count;
	count++;

	return count;
}

int check_select_query(char* query, enum Operators* operater_code, enum Details *detail_code, void** value)
{
	char help[] = "for help type 'help select'";
	int res = 0;

	// create copy of the query
	char temp[MAX_LEN_QUERY] = { 0 }, *oprator = temp;
	strcpy(temp, query);

	// find the detail type code
	query = strtok(query, SPECIAL_NOTS);
	oprator += strlen(query);
	strip(&query);
	*detail_code = find_item(query, detail_names, LEN_DETAILS_TYPES);
	if (*detail_code)
	{
		// find the operator code
		strip(&oprator);
		*operater_code = find_operator(oprator);
		if (*operater_code)
		{
			// check and set the value
			query = strtok(NULL, SPECIAL_NOTS);
			strip(&query);
			switch (*detail_code)
			{
			case f_name: case l_name:
			{
				// locate memory
				*value = (char*)malloc(MAX_LEN_NAME);
				if (*value == NULL)
				{
					printf(MEMORY_ERROR);
					return res;
				}
				// set the value
				if (*query && *value && check_name(query))
				{
					strcpy(*value, query);
					return ++res;
				}
			}
			case id:
			{
				// locate memory
				*value = (long*)malloc(sizeof(long));
				if (*value == NULL)
				{
					printf(MEMORY_ERROR);
					return res;
				}
				// set the value
				if (*query && *value &&\
					check_number(query) &&\
					sscanf(query, "%ld", (long*)*value) &&\
					check_id(**(long**)value))
					return ++res;
			}
			case c_lng: case cmp_nt: case cs_f:
			{
				// locate memory
				*value = (short*)malloc(sizeof(short));
				if (*value == NULL)
				{
					printf(MEMORY_ERROR);
					return res;
				}
				// set the value
				if (*query && *value &&\
					check_number(query) &&\
					sscanf(query, "%hd", (short*)*value) &&\
					check_mark(**(short**)value))
					return ++res;
			}
			case avrg:
			{
				// locate memory
				*value = (float*)malloc(sizeof(float));
				if (*value == NULL)
				{
					printf(MEMORY_ERROR);
					return res;
				}
				// set the value
				if (*query && *value &&\
					check_number(query) &&\
					sscanf(query, "%f", (float*)*value) &&\
					check_average(**(float**)value))
					return ++res;
			}}
			printf("  Invalid value '%s'. %s\n", query, help);
		}
		else
			printf("  Invalid operator '%s'. %s\n", oprator, help);
	}
	else
		printf("  Invalid detail: '%s'. %s\n", query, help);

	return res;
}

int check_set_query(char* query, char* first_name, char* last_name,
	long* _id, int* course_code, short* mark)
{
	long temp_id = 0;
	enum Detail detail = 0;
	int i = 0, res = 0;
	short temp_mark = 0;
	char help[] = "for help type 'help set'", *token;

	token = strtok(query, ",=");
	while (token)
	{
		if ((++i % 2)) // if it's a detail
		{
			strip(&token);
			detail = find_item(token, detail_names, LEN_DETAILS_TYPES);
			if (!detail)
				printf("  Invalid detail '%s'. %s\n", token, help);
		}
		else // if it's a value
		{
			switch (detail)
			{
			case f_name:
			{
				strip(&token);
				if (check_name(token))
					if (strlen(first_name)) // if the first name was already set
						printf("  Double detail '%s'. %s\n", detail_names[detail], help);
					else
					{
						strcpy(first_name, token);
						res++;
					}
				else
					printf("  Invalid value '%s'. %s\n", token, help);
				break;
			}
			case l_name:
			{
				strip(&token);
				if (check_name(token))
					if (strlen(last_name)) // if the last name was already set
						printf("  Double detail '%s'. %s\n", detail_names[detail], help);
					else
					{
						strcpy(last_name, token);
						res++;
					}
				else
					printf("  Invalid value '%s'. %s\n", token, help);
				break;
			}
			case id:
			{
				if (sscanf(token, " %ld", &temp_id) && check_id(temp_id))
				{
					if (*_id) // if the id was already set
						printf("  Double detail '%s'. %s\n", detail_names[detail], help);
					else
					{
						*_id = temp_id;
						res++;
					}
				}
				else
					printf("  Invalid value '%s'. %s\n", token, help);
				break;
			}
			case c_lng: case cmp_nt: case cs_f:
			{
				if (sscanf(token, " %hd", &temp_mark) && check_mark(temp_mark))
					if (*mark != -1) // if the mark was already set
						printf("  Double detail '%s'. %s\n", detail_names[detail], help);
					else
					{
						*course_code = detail - c_lng;
						*mark = temp_mark;
						res++;
					}
				else
					printf("  Invalid value '%s'. %s\n", token, help);
			}}
		}
		token = strtok(NULL, ",=");
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

int check_number(const char* txt)
{
	if (!txt || !strlen(txt))
		return 0;
	while (*txt)
		if (!isdigit(*(txt++)))
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

int check_average(const float average)
{
	return (average >= 0 && average < MAX_MARK);
}

int is_ascii(const char* txt)
{
	while (*txt)
		if (*txt < 0 || *(txt++) > 127)
			return 0;
	return 1;
}

int str_low_cmp(const char* name_a, const char* name_b)
{
	while (*name_a && *name_b) {
		char cmp = tolower(*(name_a++)) - tolower(*(name_b++));
		if (cmp) return cmp;
	}
	return *name_a - *name_b;
}


int f_name_cmp(Student* student, const void *value)
{
	char cmp = str_low_cmp(student->first_name, (char*)value);
	return (cmp > 0) - (cmp < 0);
}

int l_name_cmp(Student* student, const void *value)
{
	char cmp = str_low_cmp(student->last_name, (char*)value);
	return (cmp > 0) - (cmp < 0);
}

int c_lang_cmp(Student* student, const void* value)
{
	short temp = student->marks[0];
	if (temp == -1)
		return -2;
	return (int)((temp > *(short*)value) - (temp < *(short*)value));
}

int cmp_nt_cmp(Student* student, const void* value)
{
	short temp = student->marks[1];
	if (temp == -1)
		return -2;
	return (int)((temp > *(short*)value) - (temp < *(short*)value));
}

int cs_f_cmp(Student* student, const void* value)
{
	short temp = student->marks[2];
	if (temp == -1)
		return -2;
	return (int)((temp > *(short*)value) - (temp < *(short*)value));
}

int id_cmp(Student* student, const void* value)
{
	long temp = student->id;
	return (int)((temp > *(long*)value) - (temp < *(long*)value));
}

int avrg_cmp(Student* student, const void* value)
{
	float temp = student->marks_average;
	return (int)((temp > *(float*)value) - (temp < *(float*)value));
}