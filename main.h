#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COURSES 3
#define LINE 180
#define NAME 64
#define REQUEST 7
#define REQUEST_TYPES 8
#define DETAILS_TYPES 7
#define OPERATOR_TYPES 6

#define DB_FILE "DB_studs.csv"

#define FORM_HEADERS "|   #   |    first name    |    second name    |  C lang   | Comp Nets | CS Funds  |  Average  |\n"
#define SPLIT_LINE   "+-------+------------------+-------------------+-----------+-----------+-----------+-----------+\n"
#define HELP_MAIN \
"Enter one of the following requests:\n\
 - print:\n\
          For printing all student data.\n\
 - select:\n\
          For selecting students that meet the attached condition.\n\
            (For example - 'select first name == Jolie, or 'select average > 80').\n\
 - set:\n\
          For adding or updating student data\n\
            (For example - 'set first name=Jolie, second name=Angelina, id=300004530, C lang=80').\n\
 - save:\n\
          For saving all the changes in the data file.\n\
 - del:\n\
          For deleting all data of students that meet the attached condition (For example - 'del id==300004530).\n\
 - help:\n\
          For help and requirements for the various requests (For example - 'help select').\n\
 - quit:\n\
          For completing the server run and exiting the program.\n"
#define HELP_PRINT "For printing all student data.\n"
#define HELP_SELECT \
"For selecting students that meet the attached condition.\
 A selection request is submitted in the following format: 'select <Data Type> <Operator> <Value>'\
 (For example - 'select first name == Jolie, or 'select average > 80').\n"
#define HELP_SET \
"For adding or updating student data.\
 Set request is submitted in the following format:\
 'set first name = <value>, last name = <value>, id = <value>, <course name> = <value>'\
 (For example - 'set first name=Jolie, second name=Angelina, id=300004530, C lang=80').\n"
#define HELP_SAVE "For saving all the changes in the data file.\n"
#define HELP_DEL "For deleting all data of students that meet the attached condition\
 A selection request is submitted in the following format: 'del <Data Type> <Operator> <Value>' (For example - 'del id==300004530).\n"
#define HELP_SAVE "For help and requirements for the various requests (For example - 'help' or 'help select').\n"
#define HELP_STATE "For printing the state of the changes (add, update and delete) from the last 'save' request.\n"
#define HELP_QUIT "For completing the server run and exiting the program (includes the option to save changes).\n"


enum Details { first_name, last_name, id, c_lang, comp_net, cs_f, average };
enum Requests { quit, select, set, print, del, state, save, help };
enum Operators { eq, not_eq , biger, smaller, big_eq, sml_eq };

typedef struct Student {
	struct Student* next;
	long id;
	char* first_name;
	char* last_name;
	float marks_average;
	short marks[COURSES];
} Student;

typedef struct StudentList {
	unsigned long len;
	Student* head;
	unsigned short update_counter;
	unsigned short delete_counter;
	unsigned short add_counter;
} StudentList;

// #1 level functions

// read the students data from the file
StudentList* read_students_data();
// print all the students in a form
void print_form(StudentList* student_list);
// run the server for data requests
void run_requests_server(StudentList* student_list);
// free all the students list memory
void free_students_list(StudentList*);
// print the error massage and exit with the given code
void print_error_and_exit(const char* error, const short error_code);

// #2 level functions

void set_line(StudentList* student_list, const char* first_name
	, const char* last_name, const long id, const short course_code, const short mark);
// print all the students into the form (used by print_form function)
void print_all_students(StudentList*);
// print a single student into the form (used by print_form function)
void print_student(Student* s, unsigned int line);
// print documentation for the requests server
void get_help(short req);
// send the request to the appropriate function
int request_switch(const char* request, StudentList* student_list);
// free a single student memory
void free_student(Student* s);

// tools

// read from the file a single line
void read_line(char *line[LINE], FILE* in_file);
// check if all the string is digits
int is_number(const char* txt);
// compare between tow names (insensitive case)
int names_cmp(const char* name_a, const char* name_b);
// calculate average of the student marks
float get_student_marks_avrage(Student* student);
// get code from name
int find_item(char* item, char** arr, int start, int end);
void strip(const char* txt);

// checks
int check_line(const char** token, const char* line, char* first_name
	, char* last_name, long* id, short* course_code, short* mark);
int check_request(const char** token, const char* line, char* first_name
	, char* last_name, long* id, short* course_code, short* mark);
// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
int check_name(const char* txt);
int check_id(const long id);
int check_mark(const short mark);
int is_ascii(const char *txt);

// data management

// create a new student (update id only)
Student* create_student(long const id);
// update all the details of the student
Student* update_student(Student* student, const char* first_name, const char* last_name, const short course_code, const short mark);
// update the student first name
int update_first_name(Student* student, const char* first_name);
// update the student last name
int update_last_name(Student* student, const char* last_name);
// update single mark of the student 
void update_mark(Student* student, const short course_code, const short mark);
// get pointer to a student by given id
Student* find_student(StudentList* list, long const id);
// add student in the linked list in order
StudentList* add_student_in_order(StudentList* list, Student* student);
// delete student from the list by given id
void delete_student(StudentList*, unsigned long const id);

Student** select_studnets(const char* request, StudentList* student_list);
void set_student(StudentList* student_list, const char* first_name, const char* last_name, const long id, const short course_code, const short mark);
int set_request(const char* request, StudentList* student_list);
void delete_studnets(Student* student, StudentList* student_list);
void save_changes(StudentList* student_list);
void save_student(Student* s, FILE* out_file);
void print_state(StudentList* sl);