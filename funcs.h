#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COURSES 3
#define LINE 320
#define NAME 128

// courses codes
enum Courses { c_lang, comp_net, cs_f };

typedef struct Student {
	struct Student* next;
	unsigned long id;
	char* first_name;
	char* last_name;
	float marks_avrage;
	unsigned short marks[COURSES];
} Student;

typedef struct StudentList {
	Student* head;
	unsigned long len;
} StudentList;


void print_student(Student*);
void print_all_students(StudentList*);
void free_student(Student* s);
void free_all_students(StudentList*);

int find_cours_code(char* course_name);
int is_number(char* txt);
float get_student_marks_avrage(Student* student);

// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
int check_text_for_name(char* txt);
// validate request to server
int check_request(char* req);
// check details string for student structure
void check_and_process_line(char* details, char* error);


Student* create_student(char *first_name, char *last_name, int ID, int course_code, int mark);
Student* update_student(Student* student, char* first_name, char* last_name, int course_code, int mark);
Student* find_student(StudentList *list, int id);
void add_student_in_order(StudentList*, Student*);
void delete_student(StudentList*, long ID);


void help();// print documentation for the requests server
void play_requests_server();// for data requests


void print_error_and_exit(const char* error, int error_code);