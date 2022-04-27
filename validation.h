#define LEN_DETAILS_TYPES 7
#define MAX_ID 999999999
#define MAX_MARK 150
#define SPECIAL_NOTS "<>!=@#$%^&*()/\\'?"


// validate the details in the line and put them into the value pointers
int check_line(char** token, char* line, char* first_name
	, char* last_name, long* id, int* course_code, short* mark);

// validate the details in the selection query and put them into the value pointers
int check_select_query(char* query, enum Operators* operater_code, enum Details* detail_code, void** value);

// validate the details in the set query and put them into the value pointers
int check_set_query(char* query, char* first_name
	, char* last_name, long* id, int* course_code, short* mark);

// validate the id <1-999999999>
int check_id(const long id);

// validate the mark <0-150>
int check_mark(const short mark);

// validate the mark <0-150>
int check_average(const float average);

// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
int check_name(const char* txt);

// check if all the chars is digits
int check_number(const char* txt);

// check if all the chars is in the ASCII range
int is_ascii(const char* txt);

// compare between tow string (insensitive)
int str_low_cmp(const char* name_a, const char* name_b);


// compare between a string and a student first name (insensitive)
int f_name_cmp(Student* student, const void* value);

// compare between a string and a student last name (insensitive)
int l_name_cmp(Student* student, const void* value);

// compare between a long number and a student id
int id_cmp(Student* student, const void* value);

// compare between a short number and a student C language mark
int c_lang_cmp(Student* student, const void* value);

// compare between a short number and a student Computer Networks mark
int cmp_nt_cmp(Student* student, const void* value);

// compare between a short number and a student CS Fundamentals mark
int cs_f_cmp(Student* student, const void* value);

// compare between a floating number and a student marks average
int avrg_cmp(Student* student, const void* value);