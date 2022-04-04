#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Course {
	char* name;
	unsigned short mark;
} Course;

typedef struct Student {
	long ID;
	Course* courses;
	char* name;
	unsigned short marks_avrage;
} Student;

typedef struct StudentList {
	Student* head_of_the_list;
	Student* cursor;
} StudentList;


void print_error_and_exit(const char* error, int error_code);
void print_all_students(StudentList*);

int chck_student_details_types(char* details);// check details string
Student* crate_student(long ID, char* name, Course* courses);
Course* create_course(char* name, unsigned short mark);

void sort_students_list(StudentList*);
void add_student_in_order(StudentList*, Student*);
void delete_student(StudentList*, long ID);

void help();// print documentation for the requests server
void play_requests_server();// for data requests


int main()
{
	FILE* in_file;
	char ch = '\n';

	if (!(in_file = fopen("DB_studs.csv", "r"))) print_error_and_exit("Cannot open input file", -1);

	while (!feof(in_file) && ch)
	{
		printf("%c", ch);
		fscanf(in_file, "%c", &ch);
	}

	fclose(in_file);
	return 0;
}

void print_error_and_exit(const char* error, int error_code)
{
	printf("%s", error);
	exit(error_code);
}