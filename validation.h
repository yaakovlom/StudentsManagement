#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core.h"

#define LEN_DETAILS_TYPES 7
#define MAX_ID 999999999
#define MAX_MARK 150
#define SPECIAL_NOTS "<>!=@#$%^&*()-+/\\'?"


// validate the details in the line and put them into the value pointers
short check_line(char** token, char* line, char* first_name
	, char* last_name, long* id, short* course_code, short* mark);

// validate the details in the selection query and put them into the value pointers
short check_select_query(char* querie, enum Details* detail_code, enum Operators* operater_code, void** value);

// validate the details in the set query and put them into the value pointers
short check_set_query(char* line, char* first_name
	, char* last_name, long* id, short* course_code, short* mark);

// validate the id <1-999999999>
short check_id(const long id);

// validate the mark <0-150>
short check_mark(const short mark);

// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
short check_name(const char* txt);

// compare between tow names (insensitive)
short names_cmp(const char* name_a, const char* name_b);

// check if all the chars is in the ASCII range
short is_ascii(const char* txt);