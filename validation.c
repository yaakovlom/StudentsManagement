#include "main.h"

static char* detail_names[] = { [f_name] = "first name",[l_name] = "last name",[id] = "id",[c_lng] = "C language",
	[cmp_nt] = "Computer Networks",[cs_f] = "CS Fundamentals",[avrg] = "average" };

short check_line(char** token, char* line, char* first_name,
	char* last_name, long* _id, short* course_code, short* mark)
{
	short count = 0;

	if (!is_ascii(line))
		return count;
	*token = strtok(line, ",");

	// check the first name
	strip(token);
	if (!check_name(*token))
		return count;
	else
		strcpy(first_name, *token);
	*token = strtok(NULL, ",");
	count++;

	// check the last name
	strip(token);
	if (!check_name(*token))
		return count;
	else
		strcpy(last_name, *token);
	count++;
	*token = strtok(NULL, ",");

	// check the id
	if (!sscanf(*token, " %ld", _id) || !check_id(*_id))
		return count;
	*token = strtok(NULL, ",");
	count++;

	// check the course name
	strip(token);
	*course_code = find_item(*token, detail_names + id, COURSES_LEN);
	if (!(*course_code)--)
		return count;
	*token = strtok(NULL, ",");
	count++;

	// check the course mark
	if (!sscanf(*token, " %hd", mark) || !check_mark(*mark))
		return count;

	return ++count;
}

short check_select_query(char* query, enum Operators* operater_code, enum Details *detail_code, void** value)
{
	char help[] = "for help type 'help select'";
	short res = 0;
	char* token;

	*operater_code = find_operator(query);
	token = strtok(query, SPECIAL_NOTS);
	strip(&token);
	*detail_code = find_item(token, detail_names, LEN_DETAILS_TYPES);

	token = strtok(NULL, SPECIAL_NOTS);
	if (token && !strlen(token)) // if the operator has tow chars
		token = strtok(NULL, SPECIAL_NOTS);

	if (*detail_code)
		if (*operater_code)
			switch (*detail_code)
			{
			case f_name: case l_name:
			{
				*value = (char*)malloc(MAX_LEN_NAME);
				if (token && *value && check_name(token))
				{
					strip(&token);
					strcpy(*value, token);
					return ++res;
				}
				else
					printf("Invalid value.. %s\n", help);
				break;
			}
			case id:
			{
				*value = (long*)malloc(sizeof(long));
				if (token && *value && sscanf(token, " %ld", (long*)*value) && check_id(**(long**)value))
					return ++res;
				else
					printf("Invalid value.. %s\n", help);
				break;
			}
			case c_lng: case cmp_nt: case cs_f:
			{
				*value = (short*)malloc(sizeof(short));
				if (token && *value && sscanf(token, " %hd", (short*)*value) && check_mark(**(short**)value))
					return ++res;
				else
					printf("Invalid value.. %s\n", help);
			}
			break;
			case avrg:
			{
				*value = (float*)malloc(sizeof(float));
				if (token && *value && sscanf(token, " %f", (float*)*value) && check_average(**(float**)value))
					return ++res;
				else
					printf("Invalid value.. %s\n", help);
			}
			}
		else
			printf("Invalid operator.. %s\n", help);
	else
		printf("Invalid detail.. %s\n", help);

	return res;
}

short check_set_query(char* query, char* first_name, char* last_name,
	long* _id, short* course_code, short* mark)
{
	char help[] = "for help type 'help set'";
	enum Detail detail;
	long temp_id = 0;
	short temp_mark = 0;
	unsigned short counter = 0, res = 0;
	char* token;

	token = strtok(query, ",=");
	while (token)
	{
		if (!(counter % 2))
		{
			strip(&token);
			detail = find_item(token, detail_names, LEN_DETAILS_TYPES);
			if (!detail)
				printf("Invalid detail: %s. %s\n", token, help);
		}
		else
		{
			res++;
			switch (detail)
			{
			case f_name:
			{
				strip(&token);
				if (check_name(token))
					if (strlen(first_name))
						printf("Double detail: %s. %s\n", detail_names[detail], help);
					else
						strcpy(first_name, token);
				else
					printf("Invalid value: %s. %s\n", token, help);
				break;
			}
			case l_name:
			{
				strip(&token);
				if (check_name(token))
					if (strlen(last_name))
						printf("Double detail: %s. %s\n", detail_names[detail], help);
					else
						strcpy(last_name, token);
				else
					printf("Invalid value: %s. %s\n", token, help);
				break;
			}
			case id:
			{
				if (sscanf(token, " %ld", &temp_id) && check_id(temp_id))
				{
					if (*_id)
						printf("Double detail: %s. %s\n", detail_names[detail], help);
					else
					{
						*_id = temp_id;
					}
				}
				else
					printf("Invalid value: %s. %s\n", token, help);
				break;
			}
			case c_lng: case cmp_nt: case cs_f:
			{
				if (sscanf(token, " %hd", &temp_mark) && check_mark(temp_mark))
					if (*mark)
						printf("Double detail: %s. %s\n", detail_names[detail], help);
					else
					{
						*course_code = detail - c_lng;
						*mark = temp_mark;
					}
				else
					printf("Invalid value: %s. %s\n", token, help);
				break;
			}
			}
		}
		token = strtok(NULL, ",=");
		counter++;
	}
	return res;
}

short check_name(const char* txt)
{
	if (!txt || !strlen(txt))
		return 0;
	while (*txt)
		if (isdigit(*(txt++)) || ispunct(*txt))
			return 0;
	return 1;
}

short check_id(const long id)
{
	return (id > 0 && id < MAX_ID);
}

short check_mark(const short mark)
{
	return (mark >= 0 && mark < MAX_MARK);
}

short check_average(const float average)
{
	return (average >= 0 && average < MAX_MARK);
}

short is_ascii(const char* txt)
{
	while (*txt)
		if (*txt < 0 || *(txt++) > 127)
			return 0;
	return 1;
}

char str_low_cmp(const char* name_a, const char* name_b)
{
	while (*name_a && *name_b) {
		char cmp = tolower(*(name_a++)) - tolower(*(name_b++));
		if (cmp) return cmp;
	}
	return *name_a - *name_b;
}


char f_name_cmp(Student* student, const void *value)
{
	char cmp = str_low_cmp(student->first_name, (char*)value);
	return (cmp > cmp) - (cmp < cmp);
}

char l_name_cmp(Student* student, const void *value)
{
	char cmp = str_low_cmp(student->last_name, (char*)value);
	return (cmp > cmp) - (cmp < cmp);
}

char c_lang_cmp(Student* student, const void* value)
{
	short temp = student->marks[0];
	if (temp == -1)
		return -2;
	return (char)((temp > *(short*)value) - (temp < *(short*)value));
}

char cmp_nt_cmp(Student* student, const void* value)
{
	short temp = student->marks[1];
	if (temp == -1)
		return -2;
	return (char)((temp > *(short*)value) - (temp < *(short*)value));
}

char cs_f_cmp(Student* student, const void* value)
{
	short temp = student->marks[2];
	if (temp == -1)
		return -2;
	return (char)((temp > *(short*)value) - (temp < *(short*)value));
}

char id_cmp(Student* student, const void* value)
{
	long temp = student->id;
	return (char)((temp > *(long*)value) - (temp < *(long*)value));
}

char avrg_cmp(Student* student, const void* value)
{
	float temp = student->marks_average;
	return (char)((temp > *(float*)value) - (temp < *(float*)value));
}