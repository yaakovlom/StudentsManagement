#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COURSES 3
#define LINE 180
#define NAME 64
#define REQUEST 7
#define REQUEST_TYPES 7
#define IN_FILE "DB_studs.csv"

#define FORM_HEADERS "|    first name    |    second name    |  C lang   | Comp Nets | CS Funds  |  Average  |\n"
#define SPLIT_LINE   "+------------------+-------------------+-----------+-----------+-----------+-----------+\n"
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
#define HELP_QUIT "For completing the server run and exiting the program (includes the option to save changes).\n"


enum Courses { c_lang, comp_net, cs_f };
enum Requests { quit, select, set, print, del, save, help };

typedef struct Student {
	struct Student* next;
	long id;
	char* first_name;
	char* last_name;
	float marks_avrage;
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
StudentList* read_students_data();
void print_form(StudentList* student_list);
void run_requests_server(StudentList* student_list);
void free_students_list(StudentList*);
void print_error_and_exit(const char* error, const short error_code);

	// #2 level functions
void set_line(StudentList* student_list, const char* first_name
	,const char* last_name, const long id, const short course_code, const short mark);
void print_all_students(StudentList*);
void print_student(Student*);
// print documentation for the requests server
void get_help(short req);
// send the request to the appropriate function
int request_switch(const char* request, StudentList* student_list);
void free_student(Student* s);

	// tools
void read_line(char line[LINE], FILE* in_file);
//void scan_word(char *word);
int is_number(const char* txt);
int names_cmp(const char* name_a, const char* name_b);
float get_student_marks_avrage(Student* student);
int get_cours_code(char* course_name);
int get_request_code(char* request);

	// checks
int check_line(const char** token, const char* line, char* first_name
	,char* last_name, long* id, short* course_code, short* mark);
// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
int check_name(const char* txt);
// validate request to server
int check_request(const char* req);
int check_id(const long id);
int check_mark(const short mark);

	// data management
Student* create_student(long const id);
Student* update_student(Student* student, const char* first_name, const char* last_name, const short course_code, const short mark);
int update_first_name(Student* student, const char* first_name);
int update_last_name(Student* student, const char* last_name);
void update_mark(Student* student, const short course_code, const short mark);
Student* find_student(StudentList* list, long const id);
StudentList* add_student_in_order(StudentList* list, Student* student);
void delete_student(StudentList*, unsigned long const id);

Student** select_studnets(const char* request, StudentList* student_list);
void set_studnet(const char* request, StudentList* student_list);
void delete_studnets(const char* request, StudentList* student_list);
void save_changes(StudentList* student_list);
void quit_server(StudentList* student_list);