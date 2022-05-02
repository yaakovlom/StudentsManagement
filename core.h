#define MAX_LEN_FILE_NAME 25
#define COURSES_LEN 3
#define MAX_LEN_LINE 85
#define MAX_LEN_NAME 20
#define MAX_LEN_DETAIL 18
#define MAX_LEN_ID 10

#define FILE_NAME "DB_studs.csv"
#define MEMORY_ERROR "Cannot locate memory...\n"

#define FORM_HEADERS(_f_name, _l_name, _id, _c_lng, _cmp_nt, _cs_f, _avrg) "| "_id" | "_f_name" | "_l_name" | "_c_lng" | "_cmp_nt" | "_cs_f" | "_avrg" |\n"
#define SPLIT_LINE   "+-----------+------------------+-------------------+-----------+-----------+-----------+---------+\n"
#define SUM_LINE     "| sum of students: %-5u                                                                         |\n"
#define END_LINE     "+------------------------------------------------------------------------------------------------+\n"


enum Details { f_name = 1, l_name, id, c_lng, cmp_nt, cs_f, avrg };
enum State { failed, updated, added };

typedef struct Student {
	struct Student* next;
	long id;
	char* first_name;
	char* last_name;
	float marks_average;
	short marks[COURSES_LEN];
} Student;

typedef struct StudentList {
	Student* head;
	Student* tail;
	unsigned short update_counter;
	unsigned short delete_counter;
	unsigned short add_counter;
} StudentList;

// read the students data from the file
StudentList* read_students_data(char file_name[]);

// print all the students into a form
void print_all_students(StudentList*);

// free all the students list memory
void free_students_list(StudentList*);


// add or update of the student details
enum State set_student(StudentList* student_list, const char* first_name, const char* last_name, const long id, const int course_code, const short mark);

// create a new student (update id only)
Student* create_student(long const id);

// add student in the linked list in order
StudentList* add_student_in_order(StudentList* list, Student* student);

// get pointer to a student by given id
Student* find_student(StudentList* list, long const id);

// update all the details of the student
Student* update_student(Student* student, const char* first_name, const char* last_name, const int course_code, const short mark);

// update the student first name
int update_first_name(Student* student, const char* first_name);

// update the student last name
int update_last_name(Student* student, const char* last_name);

// update single mark of the student 
void update_mark(Student* student, const int course_code, const short mark);

// remove student from the list by given id
Student* remove_student_from_list(StudentList* student_list, long const id);


// print a single student into the form
void print_student(Student* s);

// free a single student memory
void free_student(Student* s);


// read a single line from the file
void read_line(char line[MAX_LEN_LINE], FILE* in_file);

// calculate average of the student marks
float get_student_marks_average(Student* student);


// find item from a given array
int find_item(char* item, char** arr, unsigned int len);

// remove the spaces from both sides of the string
void strip(char* txt[]);

// print head of a form
void print_head_form();

// print bottom of a form
void print_bottom_form(unsigned int len);

// save all the changes on the file
void save_changes(StudentList* student_list, char file_name[]);

// print a single student on the given file
void save_student(Student* s, FILE* out_file);