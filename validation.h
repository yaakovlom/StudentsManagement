#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core.h"

#define LEN_DETAILS_TYPES 7
#define MAX_ID 999999999
#define MAX_MARK 150


// validate the details in the line and put them into the value pointers
int check_line(const char** token, const char* line, char* first_name
	, char* last_name, long* id, short* course_code, short* mark);

// validate the details in the selection query and put them into the value pointers
int check_select_query(const char* querie, enum Details* detail_code, enum Operators* operater_code, void** value);

// validate the details in the set query and put them into the value pointers
int check_set_query(const char* line, char* first_name
	, char* last_name, long* id, short* course_code, short* mark);

// validate the id <1-999999999>
int check_id(const long id);

// validate the mark <0-150>
int check_mark(const short mark);

// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
int check_name(const char* txt);

// compare between tow names (insensitive)
int names_cmp(const char* name_a, const char* name_b);

// check if all the chars is in the ASCII range
int is_ascii(const char* txt);