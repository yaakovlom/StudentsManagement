#define LEN_DETAILS_TYPES 7
#define MAX_ID 999999999
#define MAX_MARK 150
#define SPECIAL_NOTS "<>!=@#$%^&*()-+/\\'?"


// validate the details in the line and put them into the value pointers
short check_line(char** token, char* line, char* first_name
	, char* last_name, long* id, short* course_code, short* mark);

// validate the details in the selection query and put them into the value pointers
short check_select_query(char* query, enum Operators* operater_code, enum Details* detail_code, void** value);

// validate the details in the set query and put them into the value pointers
short check_set_query(char* query, char* first_name
	, char* last_name, long* id, short* course_code, short* mark);

// validate the id <1-999999999>
short check_id(const long id);

// validate the mark <0-150>
short check_mark(const short mark);

// validate the mark <0-150>
short check_average(const float average);

// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
short check_name(const char* txt);

// check if all the chars is in the ASCII range
short is_ascii(const char* txt);

// compare between tow string (insensitive)
char str_low_cmp(const char* name_a, const char* name_b);


// compare between a string and a student first name (insensitive)
char f_name_cmp(Student* student, const void* value);

// compare between a string and a student last name (insensitive)
char l_name_cmp(Student* student, const void* value);

// compare between a long number and a student id
char id_cmp(Student* student, const void* value);

// compare between a short number and a student C language mark
char c_lang_cmp(Student* student, const void* value);

// compare between a short number and a student Computer Networks mark
char cmp_nt_cmp(Student* student, const void* value);

// compare between a short number and a student CS Fundamentals mark
char cs_f_cmp(Student* student, const void* value);

// compare between a floating number and a student marks average
char avrg_cmp(Student* student, const void* value);