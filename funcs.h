#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COURSES 3
#define LINE 180
#define NAME 64

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


void print_student(Student*);
void print_all_students(StudentList*);
void free_student(Student* s);
void free_list(StudentList*);

int find_cours_code(const char* course_name);
int is_number(const char* txt);
int names_cmp(const char* name_a, const char* name_b);
float get_student_marks_avrage(Student* student);

// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
int check_text_for_name(const char* txt);
// validate request to server
int check_request(const char* req);


Student* create_student(long const id);
Student* update_student(Student* student, const char* first_name, const char* last_name, const int course_code, const int mark);
int update_first_name(Student* student, const char* first_name);
int update_last_name(Student* student, const char* last_name);
void update_mark(Student* student, const int course_code, const int mark);

Student* find_student(StudentList *list, long const id);
StudentList* add_student_in_order(StudentList* list, Student* student);
void delete_student(StudentList*, unsigned long const id);


void help();// print documentation for the requests server
void play_requests_server();// for data requests


void print_error_and_exit(const char* error, const int error_code);