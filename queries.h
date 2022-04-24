#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core.h"

#define LEN_OPERATOR_TYPES 6
#define LEN_QUERIES_TYPES 8
#define MAX_LEN_QUERY 90
#define QUERY_LEN 7

#define HELP_MAIN \
"Enter one of the following queries:\n\
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
          For deleting all data of students that meet the attached condition (For example - 'del id=300004530).\n\
 - help:\n\
          For help and requirements for the various queries (For example - 'help select').\n\
 - quit:\n\
          For completing the server run and exiting the program.\n"

#define HELP_PRINT "For printing all student data.\n"
#define HELP_SELECT \
"For selecting students that meet the attached condition.\
 A selection  query is submitted in the following format:\
 'select <detail> <operator> <value>'\
 Valid details: id, first name, last name, C language, Computer Networks, Computer Networks, average.\
 Valid operators: <, >, =, !=.\
 (For example - 'select first name = Jolie, or 'select average > 80').\n"
#define HELP_SET \
"For adding new student or updating student data.\
 Set  query is submitted in the following format:\
 'set <id> = <value>, <detail> = <value>, ...'\
 Valid details: id (required), first name, last name, C language, Computer Networks, Computer Networks.\
 (For example - 'set first name=Jolie, last name=Angelina, id=300004530, C language=80').\n"
#define HELP_SAVE "For saving all the changes in the data file.\n"
#define HELP_DEL "For deleting a student from the list by given id.\
 A selection  query is submitted in the following format: 'del <id>' (For example - 'del 300004530).\n"
#define HELP_STATE "For printing the state of the changes (add, update and delete) from the last 'save' query.\n"
#define HELP_QUIT "For completing the server run and exiting the program (after saving the changes).\n"


enum Queries { quit = 1, select, set, print, del, state, save, help };
enum Operators { big_eq = 1, bigger, sml_eq, smaller, not_eq , eq };


// run the loop for data queries
void run_queries_loop(StudentList* student_list);

// send the  query to the appropriate function
int query_switch(char* querie, StudentList* student_list);

// select student by value
void select_query(StudentList* student_list, const char* query);

// create or update student
void set_query(const char* query, StudentList* student_list);

// print documentation for the queries server
void help_query(enum Requests  req);

// save all the changes on the file
void save_changes(StudentList* student_list);

// print a single student on the given file
void save_student(Student* s, FILE* out_file);

// print the state of changes
void print_state(StudentList* sl);

// print a selection of students from the list in a form
void print_selection(StudentList* student_list, enum Details detail_code, enum Operators operater_code, void* value);

// find operator in a string
int find_operator(const char* query);

// delete student from the list by given id
void delete_query(const char* query, StudentList* student_list);