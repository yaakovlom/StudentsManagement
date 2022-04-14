#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COURSES_LEN 3
#define MAX_LEN_LINE 90
#define MAX_LEN_NAME 20
#define MAX_LEN_DETAIL 18
#define MAX_LEN_ID 10

#define FILE_NAME "DB_studs.csv"

#define FORM_HEADERS(_f_name, _l_name, _id, _c_lng, _cmp_nt, _cs_f, _avrg) "| "_id" | "_f_name" | "_l_name" | "_c_lng" | "_cmp_nt" | "_cs_f" | "_avrg" |\n"
#define SPLIT_LINE   "+-----------+------------------+-------------------+-----------+-----------+-----------+---------+\n"



enum Details { f_name = 1, l_name, id, c_lng, cmp_nt, cs_f, avrg };

typedef struct Student {
	struct Student* next;
	long id;
	char* first_name;
	char* last_name;
	float marks_average;
	short marks[COURSES_LEN];
} Student;

typedef struct StudentList {
	unsigned long len;
	Student* head;
	Student* tail;
	unsigned short update_counter;
	unsigned short delete_counter;
	unsigned short add_counter;
} StudentList;

// read the students data from the file
StudentList* read_students_data();

// print all the students into a form
void print_all_students(StudentList*);

// free all the students list memory
void free_students_list(StudentList*);

// print the error massage and exit with the given code
void print_error_and_exit(const char* error, const short error_code);


// add or update of the student details
void set_student(StudentList* student_list, const char* first_name, const char* last_name, const long id, const short course_code, const short mark);

// create a new student (update id only)
Student* create_student(long const id);

// add student in the linked list in order
StudentList* add_student_in_order(StudentList* list, Student* student);

// get pointer to a student by given id
Student* find_student(StudentList* list, long const id);

// update all the details of the student
Student* update_student(Student* student, const char* first_name, const char* last_name, const short course_code, const short mark);

// update the student first name
int update_first_name(Student* student, const char* first_name);

// update the student last name
int update_last_name(Student* student, const char* last_name);

// update single mark of the student 
void update_mark(Student* student, const short course_code, const short mark);

// remove student from the list by given id
StudentList* remove_student(StudentList* list, long const id);


// print a single student into the form
void print_student(Student* s);

// free a single student memory
void free_student(Student* s);


// read a single line from the file
void read_line(char *line[MAX_LEN_LINE], FILE* in_file);

// calculate average of the student marks
float get_student_marks_avrage(Student* student);


// find item from a given array
int find_item(char* item, char** arr, unsigned int len);

// remove the spaces from both sides of the string
void strip(const char* txt);

// print head of a form
void print_head_form();

// print bottom of a form
void print_bottom_form();