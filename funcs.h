#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COURSES 3
#define LINE 180
#define NAME 64
#define IN_FILE "DB_studs.csv"

#define FORM_HEADERS "|    first name    |    second name    |  C lang   | Comp Nets | CS Funds  |  Average  |\n"
#define SPLIT_LINE   "+------------------+-------------------+-----------+-----------+-----------+-----------+\n"

// courses codes
enum Courses { c_lang, comp_net, cs_f };

typedef struct Student {
	struct Student* next;
	long id;
	char* first_name;
	char* last_name;
	float marks_avrage;
	short marks[COURSES];
} Student;

typedef struct StudentList {
	Student* head;
	unsigned long len;
} StudentList;

StudentList* read_students_data();
void print_form(StudentList* student_list);
void print_all_students(StudentList*);
void free_students_list(StudentList*);

void read_line(char line[LINE], FILE* in_file);
int check_line(const char** token, const char* line, char* first_name,
	char* last_name, long* id, short* course_code, short* mark);
void set_line(StudentList* student_list, const char* first_name,
	const char* last_name, const long id, const short course_code, const short mark);
void print_student(Student*);
void free_student(Student* s);

int find_cours_code(const char* course_name);
int is_number(const char* txt);
int names_cmp(const char* name_a, const char* name_b);
float get_student_marks_avrage(Student* student);

// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
int check_text_for_name(const char* txt);
// validate request to server
int check_request(const char* req);


Student* create_student(long const id);
Student* update_student(Student* student, const char* first_name, const char* last_name, const short course_code, const short mark);
int update_first_name(Student* student, const char* first_name);
int update_last_name(Student* student, const char* last_name);
void update_mark(Student* student, const short course_code, const short mark);

Student* find_student(StudentList* list, long const id);
StudentList* add_student_in_order(StudentList* list, Student* student);
void delete_student(StudentList*, unsigned long const id);


void help();// print documentation for the requests server
void play_requests_server();// for data requests


void print_error_and_exit(const char* error, const short error_code);